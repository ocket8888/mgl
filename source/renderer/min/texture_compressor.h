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
#ifndef TEXTURECOMPRESSOR
#define TEXTURECOMPRESSOR

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "file/min/bmp.h"
#include "file/min/dds.h"
#include "platform/min/window.h"

namespace min
{

class texture_compressor
{
  private:
    std::vector<GLint> _formats;
    bool _mips;
    bool _dxt1_support;
    bool _dxt3_support;
    bool _dxt5_support;

    static std::vector<uint8_t> flip_bgr_image(const unsigned, const unsigned, const unsigned, const std::vector<uint8_t>&);
    dds compress_bmp_dds(const unsigned, const unsigned, const int, unsigned, const unsigned, const std::vector<uint8_t>&) const;
    void delete_texture(GLuint) const;
    GLuint generate_texture() const;
    void get_supported_formats();

  public:
    texture_compressor();

    dds compress_bmp_dds_dxt1(const bmp&) const;
    dds compress_bmp_dds_dxt3(const bmp&) const;
    dds compress_bmp_dds_dxt5(const bmp&) const;
    void disable_mip_maps();

};
}

#endif
