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
#ifndef OOBBOX
#define OOBBOX

#include <cmath>
#include <vector>

#include "math/min/coord_sys.h"
#include "math/min/utility.h"

namespace min
{

template <typename T, template <typename> class vec, template <typename> class rot>
class oobbox_base
{
  private:
    coord_sys<T, vec> _axes;
    vec<T> _center;
    vec<T> _half_extent;
    rot<T> _rotation;

    void add(const std::vector<vec<T>>&);
    const vec<T> get_local_min() const;
    const vec<T> get_local_max() const;

  public:
    oobbox_base() : _axes(vec<T>::axes()) {}
    oobbox_base(const vec<T> &min, const vec<T> &max)
        : _axes(vec<T>::axes()), _center((max + min) * 0.5), _half_extent((max - min) * 0.5) {}
    oobbox_base(const std::vector<vec<T>>&);

    vec<T> align(const vec<T>&) const;
    vec<T> closest_point(const vec<T>&) const;
    const coord_sys<T, vec> &get_axes() const;
    const vec<T> &get_center() const;
    const vec<T> &get_half_extent() const;
    vec<T> get_extent() const;
    const vec<T> get_min() const;
    const vec<T> get_max() const;
    const rot<T> &get_rotation() const;
    std::vector<std::pair<vec<T>, vec<T>>> grid(size_t) const;
    bool point_inside(const vec<T>&) const;
    void set_position(const vec<T>&);
    void set_rotation(const rot<T>&);
    T square_distance(const vec<T>&) const;
    T square_size() const;
    std::vector<std::pair<vec<T>, vec<T>>> subdivide() const;

};

template <typename T, template <typename> class vec>
class oobbox : public oobbox_base<T, vec, quat>
{
};

// Partial specialization for resolving the type of rotation for vec2 = mat<T>
template <typename T>
class oobbox<T, vec2> : public oobbox_base<T, vec2, mat2>
{
  public:
    oobbox() : oobbox_base<T, vec2, mat2>() {}
    oobbox(const vec2<T> &min, const vec2<T> &max) : oobbox_base<T, vec2, mat2>(min, max) {}
    oobbox(const std::vector<vec2<T>> &verts) : oobbox_base<T, vec2, mat2>(verts) {}
};

// Partial specialization for resolving the type of rotation for vec3 = quat<T>
template <typename T>
class oobbox<T, vec3> : public oobbox_base<T, vec3, quat>
{
  public:
    oobbox() : oobbox_base<T, vec3, quat>() {}
    oobbox(const vec3<T> &min, const vec3<T> &max) : oobbox_base<T, vec3, quat>(min, max) {}
    oobbox(const std::vector<vec3<T>> &verts) : oobbox_base<T, vec3, quat>(verts) {}
};

// Partial specialization for resolving the type of rotation for vec4 = quat<T>
template <typename T>
class oobbox<T, vec4> : public oobbox_base<T, vec4, quat>
{
  public:
    oobbox() : oobbox_base<T, vec4, quat>() {}
    oobbox(const vec4<T> &min, const vec4<T> &max) : oobbox_base<T, vec4, quat>(min, max) {}
    oobbox(const std::vector<vec4<T>> &verts) : oobbox_base<T, vec4, quat>(verts) {}
};
}

#endif
