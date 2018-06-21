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

#include "ogg.h"

size_t min::fake_read_ogg(void *const dest, const size_t byte_size, const size_t size_to_read, void *const fake)
{
    // Get the fake_file handle
    fake_file *const ff = reinterpret_cast<fake_file *const>(fake);

    // Size of the read
    const size_t read_length = byte_size * size_to_read;

    // Don't read past the end of the file
    char *read_end = ff->_head + read_length;

    // Calculate the size of the read, avoiding reading past EOF
    const size_t length = (read_end > ff->_end) ? ff->_end - ff->_head : read_length;

    // Copy the data
    std::memcpy(dest, ff->_head, length);

    // Increment the head
    ff->_head += length;

    // Return the read length
    return length;
}

int min::fake_seek_ogg(void *const fake, const ogg_int64_t to, const int type)
{
    // Get the fake_file handle
    fake_file *const ff = reinterpret_cast<fake_file *const>(fake);

    // Implement the seek operation in the fake file
    switch (type)
    {
    case SEEK_CUR:
        ff->_head += to;
        break;
    case SEEK_END:
        ff->_head = ff->_end - to;
        break;
    case SEEK_SET:
        ff->_head = ff->_start + to;
        break;
    default:
        return -1;
    }

    // If head is before the start of file
    if (ff->_head < ff->_start)
    {
        ff->_head = ff->_start;
    }

    // If head is past the end of file
    if (ff->_head > ff->_end)
    {
        ff->_head = ff->_end;
    }

    return 0;
}

int min::fake_close_ogg(void *const fake)
{
    // THE FILE IS FAKE!
    return 0;
}

long min::fake_tell_ogg(void *const fake)
{
    // Get the fake_file handle
    fake_file *const ff = reinterpret_cast<fake_file *const>(fake);

    // Return the location from the start of the file
    return (ff->_head - ff->_start);
}


// min::ogg member functions
inline void min::ogg::load(const std::string _file)
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
        load_little_endian_16<std::vector<uint8_t>>(data);
    }
    else
    {
        throw std::runtime_error("wave: Could not load file '" + _file + "'");
    }
}

min::ogg::ogg(const std::string &file)
{
    load(file);
}

min::ogg::ogg(const min::mem_file &mem)
{
    load_little_endian_16<min::mem_file>(mem);
}

void min::ogg::clear()
{
    // Delete WAV data and reset WAV
    _data.clear();

    // Zero out fields so we dont try to use it later
    _num_channels = 0;
    _sample_rate = 0;
    _bits_per_sample = 0;
}

bool min::ogg::is_mono() const
{
    return _num_channels == 1;
}

bool min::ogg::is_stereo() const
{
    return _num_channels > 1;
}

const std::vector<uint8_t> &min::ogg::data() const
{
    return _data;
}

uint32_t min::ogg::get_bits_per_sample() const
{
    return _bits_per_sample;
}

size_t min::ogg::get_data_samples() const
{
    // Calculate number of samples in data buffer
    return (_data.size() * 8) / _bits_per_sample;
}

uint32_t min::ogg::get_sample_rate() const
{
    return _sample_rate;
}
