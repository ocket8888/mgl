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
#ifndef __MEMORY_CHUNK__
#define __MEMORY_CHUNK__

#include <cstring>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "serial.h"

namespace min
{

class mem_chunk
{
  private:
    std::vector<uint8_t> _file_data;
    std::unordered_map<std::string, mem_file> _files;

    mem_file push_back_file(const std::string&);
    void load_memory_file(const std::string&);
    void save_memory_file(const std::string&) const;

  public:
    mem_chunk() {}
    mem_chunk(const std::string&);
    void add_file(const std::string&);
    void clear();
    const mem_file &get_file(const std::string&) const;
    mem_file &get_file(const std::string&);
    size_t size() const;
    void write_memory_file(const std::string&);
};
}
#endif
