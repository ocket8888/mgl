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
#ifndef AABBOX
#define AABBOX

#include <cmath>
#include <cstdio>
#include <utility>
#include <vector>

#include "math/min/vec3.h"
#include "math/min/utility.h"

namespace min
{

template <typename T, template <typename> class vec>
class aabbox
{
  private:
    vec<T> _min;
    vec<T> _max;

  public:
    aabbox() : _min(), _max() {}
    aabbox(const vec<T> &min, const vec<T> &max) : _min(min), _max(max) {}
    aabbox(const std::vector<vec<T>>&);

    void add(const std::vector<vec<T>>&);
    vec<T> closest_point(const vec<T>&) const;
    vec<T> get_center() const;
    vec<T> get_extent() const;
    const vec<T> &get_min() const;
    const vec<T> &get_max() const;
    std::vector<std::pair<vec<T>, vec<T>>> grid(size_t) const;
    bool point_inside(const vec<T>&) const;
    void set_position(const vec<T>&);
    T square_distance(const vec<T>&) const;
    T square_size() const;
    std::vector<std::pair<vec<T>, vec<T>>> subdivide() const;
};
}

#endif
