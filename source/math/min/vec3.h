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
#ifndef __VECTOR3__
#define __VECTOR3__

#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>

#include "coord_sys.h"
#include "vec4.h"
#include "utility.h"

namespace min
{

template <typename T, template <typename> class vec>
class coord_sys;

template <typename T>
class vec4;

template <typename T>
class vec3
{
  public:
    T x;
    T y;
    T z;

    vec3();
    vec3(const T, const T, const T);
    vec3(const vec2<T>&);
    vec3(const vec4<T>&);

    constexpr static coord_sys<T, min::vec3> axes(){return coord_sys<T, vec3>(vec3<T>(1.0, 0.0, 0.0), vec3<T>(0.0, 1.0, 0.0), vec3<T>(0.0, 0.0, 1.0));}
    constexpr static T unit_length(){return var<T>::SQRT3;}
    constexpr static T inverse_unit_length() {return var<T>::INV_SQRT3;}
    constexpr static vec3<T> up() {return vec3<T>(0.0, 1.0, 0.0);}
    static std::tuple<size_t, size_t, size_t> grid_index(const vec3<T>&, const vec3<T>&, const vec3<T>&);
    static std::tuple<size_t, size_t, size_t> grid_index(const size_t, const size_t);
    static size_t grid_key(const std::tuple<size_t, size_t, size_t>&, const size_t);
    static size_t grid_key(const vec3<T>&, const vec3<T>&, const size_t, const vec3<T>&);
    static void grid_overlap(std::vector<size_t>&, const vec3<T>&, const vec3<T>&, const size_t, const vec3<T>&, const vec3<T>&);
    static size_t grid_ray_next(std::tuple<size_t, size_t, size_t>&, std::tuple<int, T, T, int, T, T, int, T, T>&, bool&, const size_t);
    static void order(vec3<T>&, vec3<T>&);
    static bool project_sat(const coord_sys<T, min::vec3>&, const vec3<T>&, const vec3<T>&, const coord_sys<T, min::vec3>&, const vec3<T>&, const vec3<T>&);
    static std::pair<vec3<T>, vec3<T>> extents(const std::vector<vec3<T>>&);
    static void subdivide_ray(std::vector<size_t>&, const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
    static void sub_overlap(std::vector<uint_fast8_t>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
    bool any_zero_outside(const vec3<T>&, const vec3<T>&, const vec3<T>&) const;
    template <typename F> static void grid_range(const vec3<T>&, const vec3<T>&, const size_t, const vec3<T>&, const vec3<T>&, const F&);
    bool inside(const vec3<T>&, const vec3<T>&) const;
    uint_fast8_t subdivide_key(const T);
    bool within(const vec3<T>&, const vec3<T>&) const;
    bool operator>(const vec3<T>&) const;
    bool operator>=(const vec3<T>&) const;
    bool operator<(const vec3<T>&) const;
    bool operator<=(const vec3<T>&) const;
    vec3<T> &operator+=(const T);
    vec3<T> &operator+=(const vec3<T>&);
    vec3<T> &operator-=(const T);
    vec3<T> &operator-=(const vec3<T>&);
    vec3<T> &operator*=(const T);
    vec3<T> &operator*=(const vec3<T>&);
    vec3<T> &operator/=(const T);
    vec3<T> &operator/=(const vec3<T>&);
    vec3<T> operator+(const T) const;
    vec3<T> operator+(const vec3<T>&) const;
    vec3<T> operator-(const T) const;
    vec3<T> operator-(const vec3<T>&) const;
    vec3<T> operator*(const T) const;
    vec3<T> operator*(const vec3<T>&) const;
    vec3<T> operator/(const T) const;
    vec3<T> operator/(const vec3<T>&) const;
    vec3<T> &set_all(const T);
    vec3<T> &abs();
    vec3<T> &clamp(const vec3<T>&, const vec3<T>&);
    vec3<T> clamp_direction(const vec3<T>&, const vec3<T>&);
    T dot(const vec3<T>&) const;
    T dot_x() const;
    T dot_y() const;
    T dot_z() const;
    vec3<T> cross(const vec3<T>&) const;
    vec3<T> cross_x() const;
    vec3<T> cross_y() const;
    vec3<T> cross_z() const;
    static std::vector<std::pair<vec3<T>, vec3<T>>> grid(const vec3<T>&, const vec3<T>&, const size_t);
    static std::vector<std::pair<vec3<T>, T>> grid_center(const vec3<T>&, const vec3<T>&, const size_t, const T);
    static std::tuple<int, T, T, int, T, T, int, T, T> grid_ray(const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
    vec3<T> inverse() const;
    vec3<T> inverse_safe() const;
    static vec3<T> lerp(const vec3<T>&, const vec3<T>&, T);
    static vec3<T> interpolate(const vec3<T>&, const vec3<T>&, T);
    T magnitude() const;
    T max() const;
    T min() const;
    static std::pair<vec3<T>, vec3<T>> most_separating(const std::vector<vec3<T>>&);
    static vec3<T> normal(const vec3<T>&, const vec3<T>&, const vec3<T>&);
    vec3<T> &normalize();
    vec3<T> &normalize_safe(const vec3<T>&);
    vec3<T> orthogonal() const;
    vec3<T> project_point(const coord_sys<T, min::vec3>&, const vec3<T>&);
    T project_length(const coord_sys<T, min::vec3>&, const vec3<T>&);
    static std::pair<vec3<T>, T> project_sat_penetration(const coord_sys<T, vec3>&, const vec3<T>&, const vec3<T>&, const coord_sys<T, vec3>&, const vec3<T>&, const vec3<T>&, const T);
    static std::pair<vec3<T>, T> project_sat_aligned_penetration(const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&, const T);
    static vec3<T> ratio(const vec3<T>&, const vec3<T>&, const vec3<T>&);
    vec3<T> sign() const;
    static std::vector<std::pair<vec3<T>, vec3<T>>> subdivide(const vec3<T>&, const vec3<T>&);
    static std::vector<std::pair<vec3<T>, T>> subdivide_center(const vec3<T>&, const vec3<T>&, const T);


};
}

#endif
