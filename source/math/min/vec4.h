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
#ifndef __VECTOR4__
#define __VECTOR4__

namespace min
{
template <typename T, template <typename> class vec>
class coord_sys;
}

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

#include "coord_sys.h"
#include "utility.h"

namespace min
{

template <typename T>
class vec4
{
  private:
    T _x;
    T _y;
    T _z;
    T _w;

  public:
    vec4();
    vec4(const vec3<T> &v);
    vec4(const vec3<T> &v, T w);
    vec4(const T x, const T y, const T z, const T w);

    T x() const;
    T y() const;
    T z() const;
    T w() const;
    void x(const T);
    void y(const T);
    void z(const T);
    void w(const T);
    vec4<T> &set_all(const T);
    vec3<T> xyz() const;
    vec4<T> &abs();
    bool any_zero_outside(const vec4<T>&, const vec4<T>&, const vec4<T>&) const;
    constexpr static coord_sys<T, min::vec4> axes(){return coord_sys<T, vec4>(vec4<T>(1.0, 0.0, 0.0, 1.0), vec4<T>(0.0, 1.0, 0.0, 1.0), vec4<T>(0.0, 0.0, 1.0, 1.0));}
    vec4<T> &clamp(const vec4<T>&, const vec4<T>&);
    vec4<T> clamp_direction(const vec4<T>&, const vec4<T>&);
    vec4 cross(const vec4<T>&) const;
    vec4<T> cross_x() const;
    vec4<T> cross_y() const;
    vec4<T> cross_z() const;
    T dot(const vec4<T>&) const;
    T dot_x() const;
    T dot_y() const;
    T dot_z() const;
    static std::vector<std::pair<vec4<T>, vec4<T>>> grid(const vec4<T>&, const vec4<T>&, const size_t);
    static std::vector<std::pair<vec4<T>, T>> grid_center(const vec4<T>&, const vec4<T>&, const size_t, const T);
    static std::tuple<size_t, size_t, size_t> grid_index(const vec4<T>&, const vec4<T>&, const vec4<T>&);
    static std::tuple<size_t, size_t, size_t> grid_index(const size_t index, const size_t);
    static size_t grid_key(const vec4<T>&, const vec4<T>&, const size_t, const vec4<T>&);
    static size_t grid_key(const std::tuple<size_t, size_t, size_t>&, const size_t);
    static void grid_overlap(std::vector<size_t>&, const vec4<T>&, const vec4<T>&, const size_t, const vec4<T>&, const vec4<T>&);
    static std::tuple<int, T, T, int, T, T, int, T, T> grid_ray(const vec4<T>&, const vec4<T>&, const vec4<T>&, const vec4<T>&);
    static size_t grid_ray_next(std::tuple<size_t, size_t, size_t>&, std::tuple<int, T, T, int, T, T, int, T, T>&, bool&, const size_t);
    template <typename F> static void grid_range(const vec4<T>&, const vec4<T>&, const size_t, const vec4<T>&, const vec4<T>&, const F&);
    static std::pair<vec4<T>, vec4<T>> extents(const std::vector<vec4<T>>&);
    bool inside(const vec4<T>&, const vec4<T>&) const;
    vec4<T> inverse() const;
    vec4<T> inverse_safe() const;
    static vec4<T> lerp(const vec4<T>&, const vec4<T>&, T);
    static vec4<T> interpolate(const vec4<T>&, const vec4<T>&, T);
    T magnitude() const;
    T max() const;
    T min() const;
    static std::pair<vec4<T>, vec4<T>> most_separating(const std::vector<vec4<T>>&);
    static vec4<T> normal(const vec4<T>&, const vec4<T>&, const vec4<T>&);
    vec4<T> &normalize();
    vec4<T> &normalize_safe(const vec4<T>&);
    static void order(vec4<T>&, vec4<T>&);
    vec4<T> orthogonal() const;
    vec4<T> project_point(const coord_sys<T, min::vec4>&, const vec4<T>&);
    T project_length(const coord_sys<T, min::vec4>&, const vec4<T>&);
    static bool project_sat(const coord_sys<T, min::vec4>&, const vec4<T>&, const vec4<T>&, const coord_sys<T, min::vec4>&, const vec4<T>&, const vec4<T>&);
    static std::pair<vec4<T>, T> project_sat_penetration(const coord_sys<T, vec4>&, const vec4<T>&, const vec4<T>&, const coord_sys<T, vec4>&, const vec4<T>&, const vec4<T>&, const T);
    static std::pair<vec4<T>, T> project_sat_aligned_penetration(const vec4<T>&, const vec4<T>&, const vec4<T>&, const vec4<T>&, const T);
    static vec4<T> ratio(const vec4<T>&, const vec4<T>&, const vec4<T>&);
    vec4<T> sign() const;
    uint_fast8_t subdivide_key(const T);
    static std::vector<std::pair<vec4<T>, vec4<T>>> subdivide(const vec4<T>&, const vec4<T>&);
    static std::vector<std::pair<vec4<T>, T>> subdivide_center(const vec4<T>&, const vec4<T>&, const T);
    static void subdivide_ray(std::vector<size_t>&, const vec4<T>&, const vec4<T>&, const vec4<T>&, const vec4<T>&, const vec4<T>&);
    static void sub_overlap(std::vector<uint_fast8_t>&, const vec4<T>&, const vec4<T>&, const vec4<T>&);
    constexpr static T unit_length();
    constexpr static T inverse_unit_length();
    constexpr static vec4<T> up();
    bool within(const vec4<T>&, const vec4<T>&) const;
    vec4<T> &operator+=(const T);
    vec4<T> &operator+=(const vec4<T>&);
    vec4<T> &operator-=(const T);
    vec4<T> &operator-=(const vec4<T>&);
    vec4<T> &operator*=(const T);
    vec4<T> &operator*=(const vec4<T>&);
    vec4<T> &operator/=(const T);
    vec4<T> &operator/=(const vec4<T>&);
    vec4<T> operator+(const T) const;
    vec4<T> operator+(const vec4<T>&) const;
    vec4<T> operator-(const T) const;
    vec4<T> operator-(const vec4<T>&) const;
    vec4<T> operator*(const T) const;
    vec4<T> operator*(const vec4<T>&) const;
    vec4<T> operator/(const T) const;
    vec4<T> operator/(const vec4<T>&) const;
    bool operator>(const vec4<T>&) const;
    bool operator>=(const vec4<T>&) const;
    bool operator<(const vec4<T>&) const;
    bool operator<=(const vec4<T>&) const;

};
}

#endif
