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

#include "bmp.h"

inline void min::bmp::load(const std::string _file)
{
    std::ifstream file(_file, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        // Get the size of the file
        const auto size = file.tellg();

        // Adjust file pointer to beginning
        file.seekg(0, std::ios::beg);

        // Allocate space for new file
        std::vector<uint8_t> data(size);

        // Read bytes and close the file
        char *ptr = reinterpret_cast<char *>(data.data());
        file.read(ptr, size);

        // Close the file
        file.close();

        // Process the BMP file
        load<std::vector<uint8_t>>(data);
    }
    else
    {
        throw std::runtime_error("bmp: Could not load file '" + _file + "'");
    }
}

min::bmp::bmp(const std::string &file)
{
    load(file);
}

min::bmp::bmp(const mem_file &mem)
{
    load<mem_file>(mem);
}

inline uint32_t min::bmp::get_width() const
{
    return _w;
}

inline uint32_t min::bmp::get_height() const
{
    return _h;
}

inline uint32_t min::bmp::get_size() const
{
    return _size;
}

inline const std::vector<uint8_t> &min::bmp::get_pixels() const
{
    return _pixel;
}

inline uint32_t min::bmp::get_pixel_size() const
{
    return _bpp;
}
