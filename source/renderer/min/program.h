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
#ifndef __PROGRAM__
#define __PROGRAM__

#include <string>
#include <vector>

#include "shader.h"

namespace min
{

class program
{
  private:
    GLuint _id;

    inline void check_extensions() const;
    inline void create_program();
    inline void link_program();
    void print_errors() const;


  public:
    program(const program&) = delete;
    program(const shader&, const shader&);
    program(const std::vector<GLuint>&);
    ~program();

    inline void use() const;
    inline GLuint id() const;

};
}

#endif
