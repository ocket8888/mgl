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
#ifndef __STRINGTOOLS__
#define __STRINGTOOLS__

#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>
#include <vector>

namespace min
{

inline std::string &ltrim(std::string&);
inline std::string &rtrim(std::string&);
inline std::string &trim(std::string&);
inline std::vector<std::string> split(const std::string&, const std::pointer_to_unary_function<int,int>&, const size_t);
inline int isequal(int);
inline int isslash(int);
inline std::vector<std::string> split_space(const std::string&, const size_t = 0);
inline std::vector<std::string> split_equal(const std::string&, const size_t = 0);
inline std::vector<std::string> split_slash(const std::string&, const size_t = 0);
inline std::vector<std::pair<size_t, size_t>> read_lines(const std::string&, const size_t = 0);
inline std::vector<std::string> get_lines(const std::string&, const std::vector<std::pair<size_t, size_t>>&, const unsigned, size_t&);
std::string to_lower(const std::string &s);

}
#endif
