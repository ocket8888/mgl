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
#ifndef __DDS__
#define __DDS__

#include <algorithm>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "mem_chunk.h"

namespace min
{

class dds
{
  public:
    static constexpr uint32_t DXT1 = 0x31545844;
    static constexpr uint32_t DXT3 = 0x33545844;
    static constexpr uint32_t DXT5 = 0x35545844;

  private:
    static constexpr uint8_t DDS_HEADER_SIZE = 128;
    uint32_t _w;
    uint32_t _h;
    uint32_t _size;
    uint32_t _bpp;
    uint32_t _mips;
    uint32_t _format;
    std::vector<uint8_t> _pixel;

    inline uint32_t calculate_size() const;

    inline void check_size();

    inline void load(const std::string);

    template <class T>
    inline void load(const T &data)
    {
        // Check that nothing funky is going on with char and uint8_t
        static_assert(sizeof(char) == 1, "Size of char is not 1");
        static_assert(sizeof(uint8_t) == 1, "Size of uint8_t is not 1");
        static_assert(std::is_same<std::uint8_t, unsigned char>::value,
                      "std::uint8_t must be implemented as unsigned char");

        // Get the size of the file and check against header size
        const size_t size = data.size();
        if (size < DDS_HEADER_SIZE)
        {
            throw std::runtime_error("dds: File not large enough to be dds file");
        }

        // Read the DDS_HEADER_SIZE byte header
        if (data[0] != 'D' && data[1] != 'D' && data[2] != 'S' && data[3] != ' ')
        {
            throw std::runtime_error("dds: Invalid dds header");
        }

        // 4 bytes the height of the image
        size_t next = 12;
        _h = read_le<uint32_t>(data, next);

        // 4 bytes the width of the image
        next = 16;
        _w = read_le<uint32_t>(data, next);

        // 4 bytes the linear size of the image
        next = 20;
        _size = read_le<uint32_t>(data, next);

        // 4 bytes the number of mip maps in this image
        next = 28;
        _mips = read_le<uint32_t>(data, next);

        // 4 bytes the fourCC value from the data
        next = 84;
        _format = read_le<uint32_t>(data, next);

        // Check format
        if (_format != DXT1 && _format != DXT3 && _format != DXT5)
        {
            throw std::runtime_error("dds: Unsupported DXT format value of '" + std::to_string(_format) + "'");
        }

        // If _format == DXT1 channels = 3, else 4
        _bpp = (_format == DXT1) ? 3 : 4;

        // Verify dds has correct size
        check_size();

        // Check that we have data
        if (_size == 0)
        {
            throw std::runtime_error("dds: image has zero pixel data");
        }

        // Check the file size against image size
        if (size < DDS_HEADER_SIZE + _size)
        {
            throw std::runtime_error("dds: File image size is corrupted, possibly missing data");
        }

        // Read the pixel data
        _pixel.resize(_size);
        std::memcpy(&_pixel[0], &data[DDS_HEADER_SIZE], _size);
    }

  public:
    dds(const std::string&);
    dds(const mem_file&);
    dds(const uint32_t, const uint32_t, const uint32_t, const uint32_t, const std::vector<uint8_t>&);
    uint32_t get_format() const;
    uint32_t get_mips() const;
    uint32_t get_width() const;
    uint32_t get_height() const;
    uint32_t get_size() const;
    const std::vector<uint8_t> &get_pixels() const;
    std::vector<uint8_t> to_file() const;
};
}
#endif
