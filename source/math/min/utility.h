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
#ifndef __UTILITY__
#define __UTILITY__

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <vector>

namespace min
{

template <typename T>
class var
{
  public:
    static constexpr T PI = 3.14159265358979323846;
    static constexpr T SQRT2 = 1.4142135623731;
    static constexpr T INV_SQRT2 = 0.707106781186547;
    static constexpr T SQRT3 = 1.73205080756888;
    static constexpr T INV_SQRT3 = 0.577350269189626;
    static constexpr T TOL_PHYS_EDGE = 1.0;
    static constexpr T TOL_PONE = 1.0 - 1E-6;
    static constexpr T TOL_NONE = -1.0 + 1E-6;
    static constexpr T TOL_REL = 1E-6;
    static constexpr T TOL_REL2 = 2E-6;

    // Use this for taking inverses without getting a NaN
    static constexpr T TOL_ZERO = 1E-3;
};

template <typename T>
constexpr inline T rad_to_deg(const T);

template <typename T>
constexpr inline T deg_to_rad(const T);

template <typename T>
constexpr inline T deg_to_rad2(const T);

template <typename T>
bool between(const T, const T, const T);

template <typename T>
inline void clamp(T&, const T, const T);

template <typename T>
inline T clamp_direction(T&, const T, const T);

template <typename T>
inline T clamp_value(const T, const T, const T, const T, const T);

template <typename T>
inline void extend(const T, T&, T&);

template <typename T>
inline T safe_inverse(const T);

template <typename T>
inline int sgn(const T);

template <typename T>
inline void swap(T&, T&);

// Typename must be an unsigned integer type
template <typename K, typename L>
class bit_flag
{
  private:
    K _row;
    K _col;
    std::vector<uint8_t> _flags;

    inline std::pair<L, uint_fast8_t> get_address(const L, const L) const;

  public:
    bit_flag();
    bit_flag(const L, const L);

    inline void clear();
    inline bool get(const K, const K) const;
    inline bool get_set_on(const K, const K);
    inline void set_on(const K, const K);
    inline void set_off(const K, const K);

};

template <typename T, typename F>
inline void uint_sort(std::vector<T>&, std::vector<T>&, F&&);

}

#endif
