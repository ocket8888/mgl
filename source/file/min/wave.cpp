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
#include "wave.h"

using namespace min;

inline void wave::load(const std::string _file)
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

        // Process the WAVE file
        load<std::vector<uint8_t>>(data);
    }
    else
    {
        throw std::runtime_error("wave: Could not load file '" + _file + "'");
    }
}


wave::wave(const std::string &file)
{
    load(file);
}

wave::wave(const mem_file &mem)
{
    load<mem_file>(mem);
}

void wave::clear()
{
    // Delete WAV data and reset WAV
    _data.clear();

    // Zero out fields so we dont try to use it later
    _num_channels = 0;
    _sample_rate = 0;
    _bits_per_sample = 0;
}

bool wave::is_mono() const
{
    return _num_channels == 1;
}

bool wave::is_stereo() const
{
    return _num_channels > 1;
}

const std::vector<uint8_t> &wave::data() const
{
    return _data;
}

uint32_t wave::get_bits_per_sample() const
{
    return _bits_per_sample;
}

size_t wave::get_data_samples() const
{
    // Calculate number of samples in data buffer
    return (_data.size() * 8) / _bits_per_sample;
}

uint32_t wave::get_sample_rate() const
{
    return _sample_rate;
}

