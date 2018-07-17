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
#ifndef TEXTUREBUFFER
#define TEXTUREBUFFER

#include <algorithm>
#include <cstdint>
#include <stdexcept>

#include "file/min/bmp.h"
#include "file/min/dds.h"
#include "program.h"
#include "platform/min/window.h"

namespace min
{

class texture_buffer
{
  private:
    std::vector<GLuint> _ids;
    size_t _max_size;

    void check_extensions() const;
    void check_texture_size(const uint32_t, const uint32_t);
    std::vector<GLuint> generate_texture(const size_t, const size_t);

  public:
    texture_buffer();
    ~texture_buffer();
    texture_buffer(const texture_buffer&) = delete;

    GLuint add_bmp_texture(const bmp&, const bool= false);
    GLuint add_dds_texture(const dds&, const bool = false);
    void bind(const GLuint, const size_t) const;
    size_t get_max_texture_size() const;
    void set_texture_uniform(const program&, const std::string&, const size_t) const;

};
}

#endif
