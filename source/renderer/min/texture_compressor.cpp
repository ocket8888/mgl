/* Copyright [2013-2018] [Aaron Springstroh, Minimal Graphics Library]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "texture_compressor.h"

std::vector<uint8_t> min::texture_compressor::flip_bgr_image(const unsigned width, const unsigned height, const unsigned pixel_size, const std::vector<uint8_t> &pixel)
{
    // Check our pixel dimensions
    size_t pixel_width = pixel_size * width;
    if (pixel.size() != pixel_width * height)
    {
        throw std::runtime_error("texture_compressor: input pixel data is not bgr");
    }

    // Create output buffer
    std::vector<uint8_t> out(pixel.size(), 0);

    // OpenGL starts at (0, 0) = bottom right corner so we must vertical flip all input
    for (unsigned i = 0; i < height; i++)
    {
        // Going bottom to top
        size_t in_col_start = pixel_width * i;

        // Going top to bottom
        size_t out_col_start = pixel_width * ((height - 1) - i);

        for (unsigned j = 0; j < pixel_width; j++)
        {
            // Starts at bottom left corner
            size_t in_index = in_col_start + j;

            // Start at top left corner
            size_t out_index = out_col_start + j;

            // Flip the pixels vertically
            out[out_index] = pixel[in_index];
        }
    }

    return out;
}

min::dds min::texture_compressor::compress_bmp_dds(const unsigned width, const unsigned height, const int dxt_format, unsigned header_format, const unsigned pixel_size, const std::vector<uint8_t> &pixel) const
{
    // We must flip input image because opengl origin is (0, 0) == bottom left corner
    std::vector<uint8_t> flip_pixel = flip_bgr_image(width, height, pixel_size, pixel);

    // Generate texture
    GLuint id = generate_texture();

    // Hint that we want good quality compression
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    if (pixel_size == 3)
    {
        // bitmap is stored in GL_BGR which is the target for this function
        glTexImage2D(GL_TEXTURE_2D, 0, dxt_format, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &flip_pixel[0]);
    }
    else if (pixel_size == 4)
    {
        // bitmap is stored in GL_BGR which is the target for this function
        glTexImage2D(GL_TEXTURE_2D, 0, dxt_format, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &flip_pixel[0]);
    }
    else
    {
        throw std::runtime_error("texture_compressor: bgr8 format is not supported");
    }

    // Determine if the texture was compressed
    GLint compressed;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compressed);
    if (compressed == GL_FALSE)
    {
        delete_texture(id);
        throw std::runtime_error("texture_compressor: texture was not compressed");
    }

    // Get the choosen format and check against DXT_X
    GLint format = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
    if (format != dxt_format)
    {
        delete_texture(id);
        throw std::runtime_error("texture_compressor: texture was not compressed to the requested format of: " + std::to_string(dxt_format));
    }

    // Generate mip map levels for this texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Query width information
    GLint w = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);

    // Query height information
    GLint h = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

    unsigned mips = 1;

    // If we have mips enabled, calculate the number of mip maps
    if (_mips)
    {
        // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_non_power_of_two.txt
        // Calculate mip map levels, valid for power of two and non-power of two textures
        unsigned max_dim = std::max(w, h);
        mips += (int)std::floor(std::log2(max_dim));
    }

    // Query size information of each mip map
    unsigned size = 0;
    std::vector<GLint> level_size(mips, 0);
    std::vector<unsigned> offset(mips + 1, 0);
    for (unsigned i = 0; i < mips; i++)
    {
        // Get size of this mip level
        glGetTexLevelParameteriv(GL_TEXTURE_2D, i, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &level_size[i]);

        // Check the level size
        if (level_size[i] <= 0)
        {
            delete_texture(id);
            throw std::runtime_error("texture_compressor: mip map level size of zero detected, invalid state");
        }

        // Record size and offset for this mip level
        size += level_size[i];
        offset[i + 1] = size;
    }

    // Get the compressed pixel data for each mip map
    std::vector<uint8_t> compressed_pixel(size, 0);
    for (unsigned i = 0; i < mips; i++)
    {
        // Verify the offset is valid
        if (offset[i] > size)
        {
            delete_texture(id);
            throw std::runtime_error("texture_compressor: incorrect compressed pixel offset calculated");
        }

        // This vertical flips the input image pixels because opengl textures begin (0,0) == lower left corner
        // We flipped the input and opengl will flip it back to correct orientation
        glGetCompressedTexImage(GL_TEXTURE_2D, i, &compressed_pixel[offset[i]]);
    }

    // Delete the texture
    delete_texture(id);

    // Return the dds
    return dds(w, h, mips, header_format, compressed_pixel);
}

void min::texture_compressor::delete_texture(GLuint id) const
{
    // Delete all textures in the buffer
    glDeleteTextures(1, &id);

    // Check for opengl errors
    throw_gl_error();
}

GLuint min::texture_compressor::generate_texture() const
{
    // Generating N textures
    GLuint id;

    // Generate new texture ID
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Set texture wrap settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set min/mag filter settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    return id;
}

void min::texture_compressor::get_supported_formats()
{
    // Get the total number of supported "normal" compressed" formats
    GLint size;
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &size);

    // store and print out the supported formats
    _formats = std::vector<GLint>(size, 0);
    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, &_formats[0]);

    // Check supported formats for the DXT1/3/5 formats and flag them
    for (const auto &i : _formats)
    {
        if (i == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
        {
            _dxt1_support = true;
        }
        else if (i == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
        {
            _dxt3_support = true;
        }
        else if (i == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
        {
            _dxt5_support = true;
        }
    }
}

min::texture_compressor::texture_compressor() : _mips(true), _dxt1_support(false), _dxt3_support(false), _dxt5_support(false)
{
    // Create a list of supported "normal=cross-platform" formats
    get_supported_formats();
}
min::dds min::texture_compressor::compress_bmp_dds_dxt1(const min::bmp &b) const
{
    // Check if we have dxt1 support
    if (!_dxt1_support)
    {
        throw std::runtime_error("texture_compressor: DDS/DXT1 is not supported");
    }

    int format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
    return compress_bmp_dds(b.get_width(), b.get_height(), format, dds::DXT1, b.get_pixel_size(), b.get_pixels());
}
min::dds min::texture_compressor::compress_bmp_dds_dxt3(const min::bmp &b) const
{
    // Check if we have dxt3 support
    if (!_dxt3_support)
    {
        throw std::runtime_error("texture_compressor: DDS/DXT3 is not supported");
    }

    int format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    return compress_bmp_dds(b.get_width(), b.get_height(), format, dds::DXT3, b.get_pixel_size(), b.get_pixels());
}
min::dds min::texture_compressor::compress_bmp_dds_dxt5(const min::bmp &b) const
{
    // Check if we have dxt5 support
    if (!_dxt5_support)
    {
        throw std::runtime_error("texture_compressor: DDS/DXT5 is not supported");
    }

    int format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    return compress_bmp_dds(b.get_width(), b.get_height(), format, dds::DXT5, b.get_pixel_size(), b.get_pixels());
}
void min::texture_compressor::disable_mip_maps()
{
    _mips = false;
}
