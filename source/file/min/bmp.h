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
#ifndef __BMP__
#define __BMP__

#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "mem_chunk.h"

namespace min
{

class bmp
{
    private:
        uint32_t _w;
        uint32_t _h;
        uint32_t _size;
        uint16_t _bpp;
        uint32_t _data;
        uint32_t _dib;
        std::vector<uint8_t> _pixel;

        inline void load(const std::string);

        // This one needs a full definition because templates
        template <class T>
        inline void load(const T &data)
        {
            // Check that nothing funky is going on with char and uint8_t
            static_assert(sizeof(char) == 1, "Size of char is not 1");
            static_assert(sizeof(uint8_t) == 1, "Size of uint8_t is not 1");
            static_assert(std::is_same<std::uint8_t, unsigned char>::value,
                          "std::uint8_t must be implemented as unsigned char");

            // File size in bytes
            const size_t size = data.size();
            if (size < 18)
            {
                throw std::runtime_error("bmp: File not large enough to be a BMP file");
            }

            // Two bytes indicating the BMP version field
            if (data[0] != 'B' && data[1] != 'M')
            {
                throw std::runtime_error("bmp: Invalid bmp header");
            }

            // 4 bytes indicating the size of the BMP file in bytes
            size_t next = 2;
            _size = read_le<uint32_t>(data, next);

            // 4 bytes indicating the starting offset where the bitmap image pixel data can be found
            next = 10;
            _data = read_le<uint32_t>(data, next);

            // 4 bytes the DIB header size, should be 40
            // 108 and 124 are allowed since they extend the BITMAPINFOHEADER capabilities
            // BITMAPV4HEADER == 108
            // BITMAPV5HEADER == 124
            next = 14;
            _dib = read_le<uint32_t>(data, next);
            if (_dib != 40 && _dib != 108 && _dib != 124)
            {
                throw std::runtime_error("bmp: expected dib size of 40, 108, or 124 got '" + std::to_string(_dib) + "'");
            }

            // 4 bytes the width of the image in pixels
            next = 18;
            _w = read_le<uint32_t>(data, next);

            // 4 bytes the height of the image in pixels
            next = 22;
            _h = read_le<uint32_t>(data, next);

            // Get the number of image planes
            next = 26;
            const uint16_t plane = read_le<uint16_t>(data, next);
            if (plane != 1)
            {
                throw std::runtime_error("bmp: expected 1 color plane got '" + std::to_string(plane) + "'");
            }

            // Get the number of bits per pixel
            next = 28;
            _bpp = read_le<uint16_t>(data, next);
            if (_bpp != 24 && _bpp != 32)
            {
                throw std::runtime_error("bmp: expected bits per pixel of 24/32 '" + std::to_string(_bpp) + "'");
            }

            // Convert to bytes
            _bpp /= 8;

            // Get the image compression
            next = 30;
            const uint32_t compress = read_le<uint32_t>(data, next);

            // Verify 32 bit RGBA format and uses the extended dib header
            if (_bpp == 4 && compress == 3 && _dib >= 108)
            {
                next = 54;
                const uint32_t red_mask = read_le<uint32_t>(data, next);
                const uint32_t green_mask = read_le<uint32_t>(data, next);
                const uint32_t blue_mask = read_le<uint32_t>(data, next);
                const uint32_t alpha_mask = read_le<uint32_t>(data, next);

                // Check the bit mask values
                if (alpha_mask != 0xFF)
                {
                    throw std::runtime_error("bmp: unsupported alpha channel mask expected 0xFF got " + std::to_string(alpha_mask));
                }
                else if (blue_mask != 0xFF00)
                {
                    throw std::runtime_error("bmp: unsupported blue channel mask expected 0xFF00 got " + std::to_string(blue_mask));
                }
                else if (green_mask != 0xFF0000)
                {
                    throw std::runtime_error("bmp: unsupported green channel mask expected 0xFF0000 got " + std::to_string(green_mask));
                }
                else if (red_mask != 0xFF000000)
                {
                    throw std::runtime_error("bmp: unsupported red channel mask expected 0xFF000000 got " + std::to_string(red_mask));
                }
            }
            else if (_bpp == 4)
            {
                throw std::runtime_error("bmp: expected 32 bit pixel data, with BI_BITFIELD compression, and with DIB header of BITMAPV4HEADER or greater, unsupported format");
            }
            else if (compress != 0)
            {
                throw std::runtime_error("bmp: expected BI_RGB image compession, got unsupported format of " + std::to_string(compress));
            }

            // Size of the raw bitmap data (including padding)
            next = 34;
            _size = read_le<uint32_t>(data, next);
            const uint32_t expected = _bpp * _w * _h;
            if (_size < expected)
            {
                throw std::runtime_error("bmp: expected image size of atleast, ''" + std::to_string(expected) + "' got '" + std::to_string(_size) + "'");
            }

            // Check that we have data
            if (_size == 0)
            {
                throw std::runtime_error("bmp: image has zero pixel data");
            }

            // Check the file size
            if (size < _data + _size)
            {
                throw std::runtime_error("bmp: Image size is corrupted, possibly missing data");
            }

            // Read the image data starting at data offset
            _pixel.resize(_size);
            std::memcpy(&_pixel[0], &data[_data], _size);
        }

public:
    bmp(const std::string &file);
    bmp(const mem_file &mem);
    inline uint32_t get_width() const;
    inline uint32_t get_height() const;
    inline uint32_t get_size() const;
    inline const std::vector<uint8_t> &get_pixels() const;
    inline uint32_t get_pixel_size() const;
};

}

#endif
