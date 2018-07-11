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

#include "text_buffer.h"

//// text ////
min::text::text(const std::string &text, const min::vec2<float> &p, const size_t offset)
    : _str(text), _location(p), _line_wrap(1000.0, 50.0), _offset(offset), _size(text.size() * 6) {}

const min::vec2<float> &min::text::line_wrap() const
{
    return _line_wrap;
}

void min::text::set_line_wrap(const min::vec2<float> &line_wrap)
{
    _line_wrap = line_wrap;
}

const min::vec2<float> &min::text::location() const
{
    return _location;
}

void min::text::set_location(const min::vec2<float> &location)
{
    _location = location;
}

size_t min::text::offset() const
{
    return _offset;
}

const min::vec2<float> &min::text::pixel_size() const
{
    return _pixel;
}

void min::text::set_offset(const size_t offset)
{
    _offset = offset;
}

size_t min::text::size() const
{
    // returns the number of points to draw for this string
    // 2 tris = 6 * std::string.size()
    return _size;
}

const std::string &min::text::str() const
{
    return _str;
}

void min::text::set_pixel_size(const min::vec2<float> &pixel)
{
    _pixel = pixel;
}

void min::text::set_string(const std::string &str)
{
    _str = str;
    _size = str.size() * 6;
}


//// text_buffer ////

inline void min::text_buffer::bind_vao() const
{
    // Bind the VAO for this buffer
    // Do not call this function often as it is unneeded context switching
    glBindVertexArray(_vao);
}

inline void min::text_buffer::check_extensions() const
{
    const bool vao = GLEW_ARB_vertex_array_object;
    const bool vbo = GLEW_ARB_vertex_buffer_object;

    // Check that we have the extensions we need
    if (!vao || !vbo)
    {
        throw std::runtime_error("text_buffer: minimum extensions not met");
    }
}

void min::text_buffer::create_vertex_buffer(const size_t size)
{
    // Generate the VAO for text layout
    glGenVertexArrays(1, &_vao);

    // Bind the vao
    bind_vao();

    // Generate the vertex buffer id
    glGenBuffers(size, _vbo.data());

    // Bind the buffer to hold data
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);

    // Enable vertex attrib
    glEnableVertexAttribArray(0);

    // Specify the vertex attributes in location = 0, no offset
#ifdef MGL_VB43
    glVertexAttribFormat(0, 4, GL_FLOAT, GL_FALSE, 0);

    //  Create the buffer binding point
    glVertexAttribBinding(0, 0);

    // No offset, standard stride, binding point 0
    glBindVertexBuffer(0, _vbo[0], 0, sizeof(min::vec4<float>));
#else
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
#endif
}

void min::text_buffer::create_texture_buffer(const unsigned width, const unsigned height)
{
    // Generate texture buffer
    glGenTextures(1, &_tid);
    glBindTexture(GL_TEXTURE_2D, _tid);

    // Allow 1 byte per pixel, as OpenGL requires 4 bytes per pixel usually
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // To prevent artifacts when sampling texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Check for bad width or height values
    if (width == 0 || height == 0)
    {
        throw std::runtime_error("text_buffer: invalid texture size calculated: ");
    }

    // Allocate memory for texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
}

void min::text_buffer::calculate_texture_dimensions(const FT_Face &face)
{
    // Get a reference to the current face glyph
    FT_GlyphSlot glyph_slot = face->glyph;
    unsigned accumw = 0;
    unsigned maxh = 0;

    // For all characters
    for (unsigned i = 0; i < 256; i++)
    {
        // Load character from int id
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            throw std::runtime_error("text_buffer: could not load TTF face id: " + std::to_string(i));
        }

        // get the character data
        char_data &c = _chars[i];

        // the horizontal distance (in 1/64th pixels) from the origin to the origin of the next glyph.
        c.adv_x = glyph_slot->advance.x >> 6;
        c.adv_y = glyph_slot->advance.y >> 6;

        // image dimensions and bitmap buffer
        c.width = glyph_slot->bitmap.width;
        c.height = glyph_slot->bitmap.rows;
        c.left = glyph_slot->bitmap_left;
        c.top = glyph_slot->bitmap_top;

        // Create a new row if we exceed 1024 pixels in X
        if (accumw + c.width + 1 >= 1024)
        {
            // Calculate the width and height for this row, width transcends all rows
            _w = std::max(_w, accumw);
            _h += maxh;

            // Reset width and max height for the next row
            accumw = 0;
            maxh = 0;
        }

        // Accumulate width and max height
        accumw += c.width + 1;
        maxh = std::max(maxh, (unsigned)c.height);
    }

    // Record the width of the image
    _w = std::max(_w, accumw);

    // Add current row to the image height, starts at 0 so we need to add to get real height
    _h += maxh;
}

void min::text_buffer::upload_character_glyphs(const FT_Face &face)
{
    // Get a reference to the current face glyph
    FT_GlyphSlot glyph_slot = face->glyph;

    unsigned maxh = 0;
    unsigned offset_w = 0;
    unsigned offset_h = 0;

    // For all characters upload generated images to OpenGL
    for (unsigned i = 0; i < 256; i++)
    {
        // Load character from int id, we must loop through a second time
        // The bitmap.buffer ptr is only valid until the next ft_load_char is called
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            throw std::runtime_error("text_buffer: could not load TTF face id: " + std::to_string(i));
        }

        // get the character data
        char_data &c = _chars[i];

        // Create rows if we hit 1024 pixels in X
        if (offset_w + c.width + 1 >= 1024)
        {
            offset_h += maxh;
            offset_w = 0;
            maxh = 0;
        }

        // Upload part of the image to the opengl texture
        glTexSubImage2D(GL_TEXTURE_2D, 0, offset_w, offset_h, c.width, c.height, GL_RED, GL_UNSIGNED_BYTE, glyph_slot->bitmap.buffer);

        // calculate the offset in texture coordinates
        c.offset_x = offset_w / (float)_w;
        c.offset_y = offset_h / (float)_h;

        // increment the total width of the image
        offset_w += c.width + 1;
        maxh = std::max(maxh, (unsigned)c.height);
    }
}

void min::text_buffer::create_texture_atlas(const FT_Face &face)
{
    // Calculate dimensions of the texture atlas
    calculate_texture_dimensions(face);

    // allocate texture buffer for storing glyphs
    create_texture_buffer(_w, _h);

    // Upload all glyphs into the texture buffer
    upload_character_glyphs(face);
}

min::vec2<float> min::text_buffer::get_pixel_size(const min::text &t) const
{
    // Get the line wrap settings in pixels
    const float lwx = t.line_wrap().x();
    const float lwy = t.line_wrap().y();

    // For all characters in string
    float x = 0.0;
    float y = 0.0;
    float y_out = 0.0;
    float x_out = 0.0;
    for (const auto &ch : t.str())
    {
        // get the character data
        const unsigned index = ch;
        const char_data &c = _chars[index];

        // Advance the cursor to the start of the next character
        x += c.adv_x;

        // Get largest character on this line
        y = std::max(y, c.height);

        // Check line wrap settings and break on space
        if (x > lwx && ch == ' ')
        {
            // Get the maximum x dimension
            x_out = std::max(x, x_out);

            // Adjust vertical position downward
            y_out += (y + (y - lwy));

            // Reset x/y coord
            x = 0.0;
            y = 0.0;
        }
    }

    // Get the maximum x/y dimension
    x_out = std::max(x, x_out);
    y_out = std::max(y, y_out);

    // Return the size of the text in a box
    return vec2<float>(x_out, y_out);
}

void min::text_buffer::process_text(const min::text &t) const
{
    // Create conversion to screen coordinates using screen size
    const float scale_x = 2.0 / _screen_x;
    const float scale_y = 2.0 / _screen_y;

    // Get the location of the text
    const vec2<float> &p = t.location();
    const float lwx = t.line_wrap().x() * scale_x;
    const float lwy = t.line_wrap().y() * scale_y;

    // Start from bottom left corner
    float x0 = 0.0;
    float y0 = 0.0;
    float x = p.x();
    float y = p.y();

    // For all characters in string
    for (const auto &ch : t.str())
    {
        // Get the character data
        const unsigned index = (ch != '_') ? ch : ' ';
        const char_data &c = _chars[index];

        // Get glyph dimensions, in screen coordinates
        const float x_left = x + (c.left * scale_x);
        const float y_top = y + (c.top * scale_y);
        const float w = c.width * scale_x;
        const float h = c.height * scale_y;
        const float y_bot = y_top - h;
        const float x_right = x_left + w;

        // Advance the cursor to the start of the next character
        x0 += c.adv_x * scale_x;
        y0 -= c.adv_y * scale_y;

        // Check line wrap settings and break on space or underscore
        if ((x0 > lwx && ch == ' ') || ch == '_')
        {
            // Reset x coord
            x0 = 0.0;

            // Adjust vertical position downward
            y0 -= lwy;
        }

        // Reset the start position
        x = p.x() + x0;
        y = p.y() + y0;

        // Create triangles, 2 for each character, data format: (x_pos, y_pos, uv_x, uv_y)
        // We flip the y-uv coordinates here because of OpenGL (0,0) == bottom, left
        const float uv_bot = c.offset_y + (c.height / (float)_h);
        const float uv_top = c.offset_y;
        const float uv_left = c.offset_x;
        const float uv_right = c.offset_x + (c.width / (float)_w);

        // Triangles must be in CW
        _data.emplace_back(x_left, y_bot, uv_left, uv_bot);
        _data.emplace_back(x_left, y_top, uv_left, uv_top);
        _data.emplace_back(x_right, y_bot, uv_right, uv_bot);

        _data.emplace_back(x_right, y_bot, uv_right, uv_bot);
        _data.emplace_back(x_left, y_top, uv_left, uv_top);
        _data.emplace_back(x_right, y_top, uv_right, uv_top);
    }
}

inline min::vec2<float> min::text_buffer::to_screen_coords(const float x, const float y) const
{
    // Convert x & y to screen coordinates
    const float sx = (2.0 / _screen_x) * x - 1.0;
    const float sy = (2.0 / _screen_y) * y - 1.0;

    // Return the screen coordinates
    return vec2<float>(sx, sy);
}

inline min::vec2<float> min::text_buffer::to_pixel_coords(const min::vec2<float> &p) const
{
    // Convert x & y to pixel coordinates
    const float px = (p.x() + 1.0) * (_screen_x / 2);
    const float py = (p.y() + 1.0) * (_screen_y / 2);

    // Return the screen coordinates
    return vec2<float>(px, py);
}

inline void min::text_buffer::upload_data(const size_t buffer_index) const
{
    if (_data.size() > 0)
    {
        // Bind the text buffer to hold data
        glBindBuffer(GL_ARRAY_BUFFER, _vbo[buffer_index]);

        // Upload data to the GPU
        const size_t text_bytes = _data.size() * sizeof(vec4<float>);
        glBufferData(GL_ARRAY_BUFFER, text_bytes, &_data[0], GL_DYNAMIC_DRAW);

        // Check that the expected character count did not overflow
        if (_data.size() > 6 * _char_count)
        {
            throw std::runtime_error("text_buffer: invalid character count");
        }

        // Data is on the GPU, so we throw this away
        _data.clear();
    }
}

min::text_buffer::text_buffer(const std::string &file, const int font_height, const size_t size)
    : _w(0), _h(0), _vbo(size), _char_count(0), _screen_x(0.0), _screen_y(0.0)
{
    // Check that all needed extensions are present
    check_extensions();

    // Initialize the FreeType library
    FT_Library lib;
    if (FT_Init_FreeType(&lib))
    {
        throw std::runtime_error("text_buffer: could not init freeType library");
    }

    // Load TTF font from file
    FT_Face face;
    if (FT_New_Face(lib, file.c_str(), 0, &face))
    {
        throw std::runtime_error("text_buffer: failed to load font");
    }

    // Set the font size, width is dynamically calculated based on font height
    FT_Set_Pixel_Sizes(face, 0, font_height);

    // create the vertex buffer for storing quads
    create_vertex_buffer(size);

    // Create texture atlas
    create_texture_atlas(face);

    // Free the font face
    FT_Done_Face(face);

    // Free the freetype library
    FT_Done_FreeType(lib);
}

min::text_buffer::~text_buffer()
{
    if (_tid > 0)
    {
        // Delete the texture
        glDeleteTextures(1, &_tid);
    }
    if (_vbo.size() > 0)
    {
        // Bind the vao
        bind_vao();

        // disable the vertex attrib
        glDisableVertexAttribArray(0);

        // unbind the VBO and delete it
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(_vbo.size(), _vbo.data());
    }
    if (_vao > 0)
    {
        // Unbind the VAO and delete it
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &_vao);
    }

    // Check for opengl errors
    throw_gl_error();
}

inline size_t min::text_buffer::add_text(const std::string &str, const float x, const float y)
{
    // Store the string
    _text.emplace_back(str, to_screen_coords(x, y), _char_count * 6);

    // Record the total character count
    _char_count += str.size();

    // return the string index
    return _text.size() - 1;
}

inline void min::text_buffer::bind(const size_t layer) const
{
    // Bind the VAO for this buffer
    // Do not call this function often as it is unneeded context switching
    glBindVertexArray(_vao);

    // Activate the texture layer
    glActiveTexture(GL_TEXTURE0 + layer);

    // Bind texture to this texture target
    glBindTexture(GL_TEXTURE_2D, _tid);
}

inline void min::text_buffer::bind_buffer(const size_t buffer_index) const
{
    // Bind new buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[buffer_index]);

#ifdef MGL_VB43
    // No offset, standard stride, binding point 0
    glBindVertexBuffer(0, _vbo[buffer_index], 0, sizeof(min::vec4<float>));
#else
    // Redundantly recreate the vertex attributes
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
#endif
}

inline void min::text_buffer::unbind() const
{
    glBindVertexArray(0);
}

inline void min::text_buffer::clear()
{
    // Clears the data in this buffer, but data will remain on GPU until next upload is called
    _data.clear();

    // Clear the string and index buffer
    _text.clear();

    // Reset char count
    _char_count = 0;
}

inline void min::text_buffer::draw(const size_t index) const
{
    // Check if we have text to draw
    if (_text.size() > 0)
    {
        // Get the draw index
        const auto &t = _text[index];

        // Draw the specific string from index
        glDrawArrays(GL_TRIANGLES, t.offset(), t.size());
    }
}

inline void min::text_buffer::draw_all() const
{
    // Check if we have text to draw
    if (_text.size() > 0)
    {
        // Calculate total buffer size
        const auto &t = _text.back();
        const size_t size = t.offset() + t.size();

        // Draw all of the text in one pass
        glDrawArrays(GL_TRIANGLES, 0, size);
    }
}

inline void min::text_buffer::draw_batch(const size_t size) const
{
    // Draw all of the text in one pass
    glDrawArrays(GL_TRIANGLES, 0, size);
}

inline void min::text_buffer::draw(const size_t from, const size_t to) const
{
    // Draw object at index 'from' to index 'to'
    const auto &start = _text[from];
    const auto &stop = _text[to];

    // Calculate
    const size_t size = stop.size() + (stop.offset() - start.offset());

    // Draw all of the text from 'start' to index 'stop'
    glDrawArrays(GL_TRIANGLES, start.offset(), size);
}

inline std::pair<uint_fast16_t, uint_fast16_t> min::text_buffer::get_screen_size() const
{
    return std::make_pair(_screen_x, _screen_y);
}

inline const min::vec2<float> min::text_buffer::get_text_location(const size_t index) const
{
    return to_pixel_coords(_text[index].location());
}

inline void min::text_buffer::reserve(const size_t size)
{
    _text.reserve(size);
}

inline void min::text_buffer::set_texture_uniform(const min::program &program, const std::string &name, const size_t layer) const
{
    GLint sampler_location = glGetUniformLocation(program.id(), name.c_str());
    if (sampler_location == -1)
    {
        throw std::runtime_error("text_buffer: could not find uniform '" + name + "'");
    }

    // Use the program to set the uniform value
    program.use();

    // Set the sampler active texture
    glUniform1i(sampler_location, layer);

    // Check for opengl errors
    throw_gl_error();
}

inline void min::text_buffer::set_screen(const uint_fast16_t width, const uint_fast16_t height)
{
    _screen_x = width;
    _screen_y = height;
}

inline void min::text_buffer::set_text(const size_t index, const std::string &str)
{
    // Calculate the change in character count
    const long diff = (long)(str.size() - _text[index].str().size());

    // Update character count
    _char_count += diff;

    // Update the text
    _text[index].set_string(str);

    // If the size of text changed in buffer
    if (diff != 0)
    {
        // Reset all calculated offsets in buffer
        const size_t size = _text.size();
        size_t offset = _text[index].offset() + _text[index].size();
        for (size_t i = index + 1; i < size; i++)
        {
            // Set the changed text offset
            _text[i].set_offset(offset);

            // Update the offset for next text
            offset += _text[i].size();
        }
    }
}

inline void min::text_buffer::set_text_location(const size_t index, const float x, const float y)
{
    // Update the location
    _text[index].set_location(to_screen_coords(x, y));
}

inline void min::text_buffer::set_line_wrap(const size_t index, const float x, const float y)
{
    // Update the line wrap setting
    _text[index].set_line_wrap(vec2<float>(x, y));
}

inline void min::text_buffer::set_text(const size_t index, const std::string &text, const float x, const float y)
{
    // Set the text
    set_text(index, text);

    // Set the text location
    set_text_location(index, x, y);
}

inline void min::text_buffer::set_text_center(const size_t index, const float x, const float y)
{
    // Get the pixel size of the text
    const vec2<float> &size = _text[index].pixel_size();

    // Offset the x and y by half the box size
    const vec2<float> center = vec2<float>(x, y) - size * 0.5;

    // Update the location
    _text[index].set_location(to_screen_coords(center.x(), center.y()));
}

inline void min::text_buffer::set_text_center(const size_t index, const std::string &text, const float x, const float y)
{
    // Set the text
    set_text(index, text);

    // Get the pixel size of the text
    const vec2<float> size = get_pixel_size(_text[index]);

    // Cache the pixel size
    _text[index].set_pixel_size(size);

    // Center the text
    set_text_center(index, x, y);
}

inline size_t min::text_buffer::size() const
{
    return _text.size();
}

void min::text_buffer::upload(const size_t buffer_index) const
{
    // Reserve space for the text, 2 triangles for each character
    _data.reserve(6 * _char_count);

    // Get the number of strings to render
    const size_t size = _text.size();
    if (size == 0)
    {
        // Do nothing if no data to upload
        return;
    }

    // Add each string to the text buffer
    for (size_t i = 0; i < size; i++)
    {
        process_text(_text[i]);
    }

    // Upload the data in the data buffer
    upload_data(buffer_index);
}

size_t min::text_buffer::upload_batch(const size_t buffer_index, const std::vector<size_t> &indices) const
{
    // Reserve space for the text, 2 triangles for each character
    _data.reserve(6 * _char_count);

    // Get the number of strings to render
    const size_t size = indices.size();
    if (size == 0)
    {
        // Do nothing if no data to upload
        return 0;
    }

    // Add each string to the text buffer
    for (size_t i = 0; i < size; i++)
    {
        process_text(_text[indices[i]]);
    }

    // Get the data size
    const size_t data_size = _data.size();

    // Upload the data in the data buffer
    upload_data(buffer_index);

    // Return the size of the batch for drawing later
    return data_size;
}
