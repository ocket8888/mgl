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
#ifndef __OGG__
#define __OGG__

#include <cstring>
#include <vector>
#include <vorbis/vorbisfile.h>

#include "mem_chunk.h"

namespace min
{

// This fakes a file interface to read files from memory
struct fake_file
{
  public:
    char *_head;
    char *const _start;
    size_t _size;
    char *const _end;

    fake_file(char *const head, char *const start, const size_t size)
        : _head(head), _start(start), _size(size), _end(start + size) {}
};

size_t fake_read_ogg(void* const, const size_t, const size_t, void* const);
int fake_seek_ogg(void* const, const ogg_int64_t, const int);
int fake_close_ogg(void* const);
long fake_tell_ogg(void* const);

class ogg
{
  private:
    uint16_t _num_channels;
    uint32_t _sample_rate;
    uint32_t _bits_per_sample;
    std::vector<uint8_t> _data;

    inline void load(const std::string);
    template<class T> inline void load_little_endian_16(const T&);

  public:
    ogg(const std::string&);
    ogg(const mem_file&);
    void clear();
    bool is_mono() const;
    bool is_stereo() const;
    const std::vector<uint8_t> &data() const;
    uint32_t get_bits_per_sample() const;
    size_t get_data_samples() const;
    uint32_t get_sample_rate() const;
};
}
#endif
