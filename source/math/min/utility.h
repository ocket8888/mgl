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
#ifndef UTILITY
#define UTILITY

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
constexpr T rad_to_deg(const T rad){return rad * (180.0 / min::var<T>::PI);}

template <typename T>
constexpr T deg_to_rad(const T degrees){return degrees * (min::var<T>::PI / 180.0);}

template <typename T>
constexpr T deg_to_rad2(const T degrees){return degrees * (min::var<T>::PI / 360.0);}

template <typename T>
bool between(const T, const T, const T);

template <typename T>
void clamp(T&, const T, const T);

template <typename T>
T clamp_direction(T&, const T, const T);

template <typename T>
T clamp_value(const T, const T, const T, const T, const T);

template <typename T>
void extend(const T, T&, T&);

template <typename T>
T safe_inverse(const T);

template <typename T>
int sgn(const T);

template <typename T>
void swap(T&, T&);

// Typename must be an unsigned integer type
template <typename K, typename L>
class bit_flag
{
  private:
    K _row;
    K _col;
    std::vector<uint8_t> _flags;

    std::pair<L, uint_fast8_t> get_address(const L, const L) const;

  public:
    bit_flag();
    bit_flag(const L, const L);

    void clear();
    bool get(const K, const K) const;
    bool get_set_on(const K, const K);
    void set_on(const K, const K);
    void set_off(const K, const K);

};

// radix sort for unsigned integers
template <typename T, typename F>
void uint_sort(std::vector<T> &uints, std::vector<T> &copy, F &&key_function)
{
    const size_t size = uints.size();

    // Divert to std::sort for size < 128; 2*N > N*log(N)
    if (size < 128)
    {
        return std::sort(uints.begin(), uints.end(), [key_function](const size_t a, const size_t b) {
            const size_t a_key = key_function(a);
            const size_t b_key = key_function(b);
            return a_key < b_key;
        });
    }

    // Initialize copy vector
    copy.resize(size, 0);
    std::vector<T> *from = &uints;
    std::vector<T> *to = &copy;
    std::vector<T> *temp = nullptr;

    // Number of passes
    size_t counts[256];
    const int passes = sizeof(T) / sizeof(uint8_t);

    for (int i = 0; i < passes; i++)
    {
        // zero counts
        for (size_t &count : counts)
        {
            count = 0;
        }

        // count frequency
        for (const auto &ui : *from)
        {
            // Extract the key
            const uint_fast8_t key = (key_function(ui) >> 8 * i) & (0xFF);

            // Count the frequency of key
            counts[key]++;
        }

        // prefix sum
        size_t total = 0;
        for (size_t &count : counts)
        {
            const size_t old_count = count;
            count = total;
            total += old_count;
        }

        // sort
        for (const auto &ui : *from)
        {
            // Extract the key
            const uint_fast8_t key = (key_function(ui) >> 8 * i) & (0xFF);

            // perform copy sort
            (*to)[counts[key]++] = ui;
        }

        // Swap from/to for next pass
        temp = from;
        from = to;
        to = temp;
    }

    // *from was the last *to
    // Copy sorted array into output, if needed
    if (from != &uints)
    {
        for (size_t i = 0; i < size; i++)
        {
            uints[i] = (*from)[i];
        }
    }
}
};

#endif
