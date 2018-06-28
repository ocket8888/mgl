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
  private:
    T _x;
    T _y;
    T _z;

  public:
    vec3();
    vec3(const T, const T, const T);
    vec3(const vec2<T>&);
    vec3(const vec4<T>&);

    T x() const;
    inline T y() const;
    inline T z() const;
    inline void x(const T);
    inline void y(const T);
    inline void z(const T);
    inline vec3<T> &set_all(const T);
    inline vec3<T> &abs();
    inline bool any_zero_outside(const vec3<T>&, const vec3<T>&, const vec3<T>&) const;
    inline constexpr static coord_sys<T, min::vec3> axes();
    inline vec3<T> &clamp(const vec3<T>&, const vec3<T>&);
    inline vec3<T> clamp_direction(const vec3<T>&, const vec3<T>&);
    inline vec3<T> cross(const vec3<T>&) const;
    inline vec3<T> cross_x() const;
    inline vec3<T> cross_y() const;
    inline vec3<T> cross_z() const;
    inline T dot(const vec3<T>&) const;
    inline T dot_x() const;
    inline T dot_y() const;
    inline T dot_z() const;
    inline static std::vector<std::pair<vec3<T>, vec3<T>>> grid(const vec3<T>&, const vec3<T>&, const size_t);
    inline static std::vector<std::pair<vec3<T>, T>> grid_center(const vec3<T>&, const vec3<T>&, const size_t, const T);
    inline static std::tuple<size_t, size_t, size_t> grid_index(const vec3<T>&, const vec3<T>&, const vec3<T>&);
    inline static std::tuple<size_t, size_t, size_t> grid_index(const size_t, const size_t);
    inline static size_t grid_key(const vec3<T>&, const vec3<T>&, const size_t, const vec3<T>&);
    inline static size_t grid_key(const std::tuple<size_t, size_t, size_t>&, const size_t);
    inline static void grid_overlap(std::vector<size_t>&, const vec3<T>&, const vec3<T>&, const size_t, const vec3<T>&, const vec3<T>&);
    inline static std::tuple<int, T, T, int, T, T, int, T, T> grid_ray(const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
    inline static size_t grid_ray_next(std::tuple<size_t, size_t, size_t>&, std::tuple<int, T, T, int, T, T, int, T, T>&, bool&, const size_t);
    template <typename F> inline static void grid_range(const vec3<T>&, const vec3<T>&, const size_t, const vec3<T>&, const vec3<T>&, const F&);
    inline static std::pair<vec3<T>, vec3<T>> extents(const std::vector<vec3<T>>&);
    inline bool inside(const vec3<T>&, const vec3<T>&) const;
    inline vec3<T> inverse() const;
    inline vec3<T> inverse_safe() const;
    inline static vec3<T> lerp(const vec3<T>&, const vec3<T>&, T);
    inline static vec3<T> interpolate(const vec3<T>&, const vec3<T>&, T);
    inline T magnitude() const;
    inline T max() const;
    inline T min() const;
    inline static std::pair<vec3<T>, vec3<T>> most_separating(const std::vector<vec3<T>>&);
    inline static vec3<T> normal(const vec3<T>&, const vec3<T>&, const vec3<T>&);
    inline vec3<T> &normalize();
    inline vec3<T> &normalize_safe(const vec3<T>&);
    inline static void order(vec3<T>&, vec3<T>&);
    inline vec3<T> orthogonal() const;
    inline vec3<T> project_point(const coord_sys<T, min::vec3>&, const vec3<T>&);
    inline T project_length(const coord_sys<T, min::vec3>&, const vec3<T>&);
    static inline bool project_sat(const coord_sys<T, min::vec3>&, const vec3<T>&, const vec3<T>&, const coord_sys<T, min::vec3>&, const vec3<T>&, const vec3<T>&);
    static inline std::pair<vec3<T>, T> project_sat_penetration(const coord_sys<T, vec3>&, const vec3<T>&, const vec3<T>&, const coord_sys<T, vec3>&, const vec3<T>&, const vec3<T>&, const T);
    static inline std::pair<vec3<T>, T> project_sat_aligned_penetration(const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&, const T);
    inline static vec3<T> ratio(const vec3<T>&, const vec3<T>&, const vec3<T>&);
    inline vec3<T> sign() const;
    inline uint_fast8_t subdivide_key(const T);
    inline static std::vector<std::pair<vec3<T>, vec3<T>>> subdivide(const vec3<T>&, const vec3<T>&);
    inline static std::vector<std::pair<vec3<T>, T>> subdivide_center(const vec3<T>&, const vec3<T>&, const T);
    inline static void subdivide_ray(std::vector<size_t>&, const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
    inline static void sub_overlap(std::vector<uint_fast8_t>&, const vec3<T>&, const vec3<T>&, const vec3<T>&);
    inline constexpr static T unit_length();
    inline constexpr static T inverse_unit_length();
    inline constexpr static vec3<T> up();
    inline bool within(const vec3<T>&, const vec3<T>&) const;
    inline vec3<T> &operator+=(const T);
    inline vec3<T> &operator+=(const vec3<T>&);
    inline vec3<T> &operator-=(const T);
    inline vec3<T> &operator-=(const vec3<T>&);
    inline vec3<T> &operator*=(const T);
    inline vec3<T> &operator*=(const vec3<T>&);
    inline vec3<T> &operator/=(const T);
    inline vec3<T> &operator/=(const vec3<T>&);
    inline vec3<T> operator+(const T) const;
    inline vec3<T> operator+(const vec3<T>&) const;
    inline vec3<T> operator-(const T) const;
    inline vec3<T> operator-(const vec3<T>&) const;
    inline vec3<T> operator*(const T) const;
    inline vec3<T> operator*(const vec3<T>&) const;
    inline vec3<T> operator/(const T) const;
    inline vec3<T> operator/(const vec3<T>&) const;
    inline bool operator>(const vec3<T>&) const;
    inline bool operator>=(const vec3<T>&) const;
    inline bool operator<(const vec3<T>&) const;
    inline bool operator<=(const vec3<T>&) const;


};
}

#endif
