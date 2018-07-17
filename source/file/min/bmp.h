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
#ifndef BMP
#define BMP

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

        void load(const std::string);
        template<class T> void load(const T&);

public:
    bmp(const std::string &file);
    bmp(const mem_file &mem);
    uint32_t get_width() const;
    uint32_t get_height() const;
    uint32_t get_size() const;
    const std::vector<uint8_t> &get_pixels() const;
    uint32_t get_pixel_size() const;
};

}

#endif
