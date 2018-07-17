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
#ifndef SHADER
#define SHADER

#include <fstream>
#include <string>

#include "platform/min/window.h"
#include "file/min/mem_chunk.h"

namespace min
{

class shader
{
  private:
    GLuint _id;
    GLenum _type;

    void check_extensions() const;
    void load_file(const std::string, const GLenum);
    void load(const std::string&, const GLenum);
    void print_errors() const;

  public:
    shader(const std::string&, const GLenum);
    shader(const mem_file&, const GLenum);
    ~shader();

    shader(const shader&) = delete;

    GLuint id() const;
};
}

#endif
