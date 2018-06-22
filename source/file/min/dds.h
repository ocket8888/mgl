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
    template<class T> inline void load(const T&);


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
