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
#ifndef WAVE
#define WAVE

#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "mem_chunk.h"
#include "serial.h"

// Reads a RIFF WAV File with two subchunks, 'fmt' and 'data'
// chunk_id (RIFF) - 4B - offset 0 == 'RIFF'
// chunk_size      - 4B - offset 4 == file_size - 8
// format (WAVE)   - 4B - offset 8 == 'WAVE'
// subchunk1_id    - 4B - offset 12 == "fmt "
// subchunk1_size  - 4B - offset 16 == 16 for PCM
// audio_format    - 2B - offset 20 == 1 (Linear Uncompressed PCM)
// num_channels    - 2B - offset 22 == Mono = 1, Stereo = 2
// sample_rate     - 4B - offset 24 == 44100 Hz (CD Quality)
// byte_rate       - 4B - offset 28 == sample_rate * num_channels * bytes_per_sample
// block_align     - 2B - offset 32 == num_channels * bytes_per_sample
// bits_per_sample - 2B - offset 34 == 'N' bits
// subchunk2_id    - 4B - offset 36 == "data"
// subchunk2_size  - 4B - offset 40 == data_size
// data            - ^^ - offset 44 == start of sound data

namespace min
{

class wave
{
  private:
    uint16_t _num_channels;
    uint32_t _sample_rate;
    uint32_t _bits_per_sample;
    std::vector<uint8_t> _data;

    void load(const std::string);
    template <class T> void load(const T&);

  public:
    wave(const std::string&);
    wave(const mem_file&);

    void clear();
    bool is_mono() const;
    bool is_stereo() const;
    const std::vector<uint8_t> &data() const;
    uint32_t get_bits_per_sample() const;
    size_t  get_data_samples() const;
    uint32_t get_sample_rate() const;

};
}
#endif
