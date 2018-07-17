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

#include "utility.h"

template <typename T>
bool min::between(const T val, const T min, const T max)
{
    return (val > min) && (val < max);
}

template void min::clamp(double&, const double, const double);
template void min::clamp(float&, const float, const float);

template <typename T>
void min::clamp(T &val, const T min, const T max)
{
    // Clamps val between min and max
    if (val < min)
        val = min;
    else if (val > max)
        val = max;
}

template double min::clamp_direction(double&, const double, const double);
template float min::clamp_direction(float&, const float, const float);

template <typename T>
T min::clamp_direction(T &val, const T min, const T max)
{
    // Clamps val between min and max
    if (val < min)
    {
        val = min;
        return -1.0;
    }
    else if (val > max)
    {
        val = max;
        return -1.0;
    }

    return 1.0;
}

template double min::clamp_value(const double, const double, const double, const double, const double);
template float min::clamp_value(const float, const float, const float, const float, const float);

template <typename T>
T min::clamp_value(const T val, const T min, const T minv, const T max, const T maxv)
{
    // Clamps val between min and max and assigns either minv, maxv or zero
    if (val < min)
    {
        return minv;
    }
    else if (val > max)
    {
        return maxv;
    }

    return 0.0;
}

template void min::extend(const float, float&, float&);
template void min::extend(const double, double&, double&);

template <typename T>
void min::extend(const T val, T &min, T &max)
{
    // extends min or max to val
    if (val < min)
        min = val;
    else if (val > max)
        max = val;
}


template unsigned int min::safe_inverse(const unsigned int);
template unsigned short min::safe_inverse(const unsigned short);
template double min::safe_inverse(const double);
template float min::safe_inverse(const float);

template <typename T>
T min::safe_inverse(const T v)
{
    T out;

    // Test for division by zero
    if (std::abs(int(v)) < min::var<T>::TOL_REL)
    {
        out = std::numeric_limits<T>::max();
    }
    else
    {
        out = 1.0 / v;
    }

    return out;
}

template int min::sgn(const double);
template int min::sgn(const float);

template <typename T>
int min::sgn(const T val)
{
    return (T(0) < val) - (val < T(0));
}

template void min::swap(float&, float&);
template void min::swap(double&, double&);

template <typename T>
void min::swap(T &a, T &b)
{
    // Swaps a and b
    T _a = a;
    a = b;
    b = _a;
}

//// bit_flag ////
template <typename K, typename L>
std::pair<L, uint_fast8_t> min::bit_flag<K,L>::get_address(const L row, const L col) const
{
    // Divide by 8 to get into bytes
    const L position = (row * _col) + col;
    const L byte = position >> 3;

    // Get the offset
    // 0-7 value
    const uint_fast8_t offset = position % 8;

    // Return address
    return std::make_pair(byte, offset);
}

template <typename K, typename L>
min::bit_flag<K,L>::bit_flag() : _row(0), _col(0) {}

template <typename K, typename L>
min::bit_flag<K,L>::bit_flag(const L row, const L col) : _row(row), _col(col), _flags((row * col >> 3) + 1, 0) {}

template <typename K, typename L>
void min::bit_flag<K,L>::clear()
{
    // Zero out the bit buffer
    std::fill(_flags.begin(), _flags.end(), 0);
}

template <typename K, typename L>
bool min::bit_flag<K,L>::get(const K row, const K col) const
{
    // Get the address
    const std::pair<L, uint_fast8_t> addr = get_address(row, col);

    // Return 1 if on and zero if off
    return (_flags[addr.first] >> addr.second) & 0x1;
}

template <typename K, typename L>
bool min::bit_flag<K,L>::get_set_on(const K row, const K col)
{
    // Get the address
    const std::pair<L, uint_fast8_t> addr = get_address(row, col);

    // Cache shift for the read/write operation
    const auto shift = (0x1 << addr.second);

    // Return 1 if on and zero if off
    const bool out = _flags[addr.first] & shift;

    // Set bit on
    _flags[addr.first] |= shift;

    return out;
}

template <typename K, typename L>
void min::bit_flag<K,L>::set_on(const K row, const K col)
{
    // Get the address
    const std::pair<L, uint_fast8_t> addr = get_address(row, col);
    _flags[addr.first] |= (0x1 << addr.second);
}

template <typename K, typename L>
void min::bit_flag<K,L>::set_off(const K row, const K col)
{
    // Get the address
    const std::pair<L, uint_fast8_t> addr = get_address(row, col);
    _flags[addr.first] &= ~(0x1 << addr.second);
}

// radix sort for unsigned integers
template <typename T, typename F>
void min::uint_sort(std::vector<T> &uints, std::vector<T> &copy, F &&key_function)
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
