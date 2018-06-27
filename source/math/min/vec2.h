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
#ifndef __VECTOR2__
#define __VECTOR2__

#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>
#include <utility>
#include <vector>

#include "coord_sys.h"
#include "utility.h"

namespace min
{

template <typename T, template <typename> class vec>
class coord_sys;

template <typename T>
class vec2
{
  private:
    T _x;
    T _y;

  public:
    vec2();
    vec2(const T, const T);

    inline T x() const;
    inline T y() const;
    inline void x(const T);
    inline void y(const T);
    inline vec2<T> &set_all(const T);
    inline vec2<T> &abs();
    inline bool any_zero_outside(const vec2<T>&, const vec2<T>&, const vec2<T>&) const;
    inline constexpr static coord_sys<T, vec2> axes();
    inline vec2<T> &clamp(const vec2<T>&, const vec2<T>&);
    inline vec2<T> clamp_direction(const vec2<T>&, const vec2<T>&);
    inline T cross(const vec2<T>&) const;
    inline T dot(const vec2<T>&) const;
    inline static std::pair<vec2<T>, vec2<T>> extents(const std::vector<vec2<T>>&);
    inline static std::vector<std::pair<vec2<T>, vec2<T>>> grid(const vec2<T>&, const vec2<T>&, const size_t);
    inline static std::vector<std::pair<vec2<T>, T>> grid_center(const vec2<T>&, const vec2<T>&, const size_t, const T);
    inline static std::pair<size_t, size_t> grid_index(const vec2<T>&, const vec2<T>&, const vec2<T>&);
    inline static std::pair<size_t, size_t> grid_index(const size_t, const size_t);
    inline static size_t grid_key(const vec2<T>&, const vec2<T>&, const size_t, const vec2<T>&);
    inline static size_t grid_key(const std::pair<size_t, size_t>&, const size_t);
    inline static void grid_overlap(std::vector<size_t>&, const vec2<T>&, const vec2<T>&, const size_t, const vec2<T>&, const vec2<T>&);
    inline static std::tuple<int, T, T, int, T, T> grid_ray(const vec2<T>&, const vec2<T>&, const vec2<T>&, const vec2<T>&);
    inline static size_t grid_ray_next(std::pair<size_t, size_t>&, std::tuple<int, T, T, int, T, T>&, bool&, const size_t);
    template <typename F> inline static void grid_range(const vec2<T>&, const vec2<T>&, const size_t, const vec2<T>&, const vec2<T>&, const F&);
    inline bool inside(const vec2<T>&, const vec2<T>&) const;
    inline vec2<T> inverse() const;
    inline vec2<T> inverse_safe() const;
    inline static vec2<T> lerp(const vec2<T>&, const vec2<T>&, T);
    inline static vec2<T> interpolate(const vec2<T>&, const vec2<T>&, T);
    inline T magnitude() const;
    inline T max() const;
    inline T min() const;
    inline static std::pair<vec2<T>, vec2<T>> most_separating(const std::vector<vec2<T>>&);
    inline static vec2<T> normal(const vec2<T>&, const vec2<T>&, const vec2<T>&);
    inline vec2<T> &normalize();
    inline vec2<T> &normalize_safe(const vec2<T>&);
    inline static void order(vec2<T>&, vec2<T>&);
    inline vec2<T> orthogonal() const;
    inline vec2<T> project_point(const coord_sys<T, min::vec2>&, const vec2<T>&);
    inline T project_length(const coord_sys<T, min::vec2>&, const vec2<T>&);
    static inline bool project_sat(const coord_sys<T, min::vec2>&, const vec2<T>&, const vec2<T>&, const coord_sys<T, min::vec2>&, const vec2<T>&, const vec2<T>&);
    static inline std::pair<vec2<T>, T> project_sat_penetration(const coord_sys<T, vec2>&, const vec2<T>&, const vec2<T>&, const coord_sys<T, vec2>&, const vec2<T>&, const vec2<T>&, const T);
    static inline std::pair<vec2<T>, T> project_sat_aligned_penetration(const vec2<T>&, const vec2<T>&, const vec2<T>&, const vec2<T>&, const T);
    inline static vec2<T> ratio(const vec2<T>&, const vec2<T>&, const vec2<T>&);
    inline vec2<T> sign() const;
    inline uint_fast8_t subdivide_key(const T);
    inline static std::vector<std::pair<vec2<T>, vec2<T>>> subdivide(const vec2<T>&, const vec2<T>&);
    inline static std::vector<std::pair<vec2<T>, T>> subdivide_center(const vec2<T>&, const vec2<T>&, const T);
    inline static void subdivide_ray(std::vector<size_t>&, const vec2<T>&, const vec2<T>&, const vec2<T>&, const vec2<T>&, const vec2<T>&);
    inline static void sub_overlap(std::vector<uint_fast8_t>&, const vec2<T>&, const vec2<T>&, const vec2<T>&);
    inline constexpr static T unit_length();
    inline constexpr static T inverse_unit_length();
    inline constexpr static vec2<T> up();
    inline bool within(const vec2<T>&, const vec2<T>&) const;
    inline vec2<T> &operator+=(const T);
    inline vec2<T> &operator+=(const vec2<T>&);
    inline vec2<T> &operator-=(const T);
    inline vec2<T> &operator-=(const vec2<T>&);
    inline vec2<T> &operator*=(const T);
    inline vec2<T> &operator*=(const vec2<T>&);
    inline vec2<T> &operator/=(const T);
    inline vec2<T> &operator/=(const vec2<T>&);
    inline vec2<T> operator+(const T a) const;
    inline vec2<T> operator+(const vec2<T>&) const;
    inline vec2<T> operator-(const T) const;
    inline vec2<T> operator-(const vec2<T>&) const;
    inline vec2<T> operator*(const T) const;
    inline vec2<T> operator*(const vec2<T>&) const;
    inline vec2<T> operator/(const T) const;
    inline vec2<T> operator/(const vec2<T>&) const;
    inline bool operator>(const vec2<T>&) const;
    inline bool operator>=(const vec2<T>&) const;
    inline bool operator<(const vec2<T>&) const;
    inline bool operator<=(const vec2<T>&) const;
};
}

#endif
