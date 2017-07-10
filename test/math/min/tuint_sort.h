/* Copyright [2013-2016] [Aaron Springstroh, Minimal Graphics Library]

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
#ifndef __TESTUINTSORT__
#define __TESTUINTSORT__

#include <cstdint>
#include <min/test.h>
#include <min/utility.h>
#include <stdexcept>

bool test_uint_sort()
{
    bool out = true;
    std::vector<uint64_t> uints = {100000, 70000, 130000, 3, 1, 5000000};
    min::uint_sort<uint64_t>(uints, [](const size_t a) {
        return a;
    });

    // Check array is sorted
    out = out && compare(1, uints[0], 1E-4);
    out = out && compare(3, uints[1], 1E-4);
    out = out && compare(70000, uints[2], 1E-4);
    out = out && compare(100000, uints[3], 1E-4);
    out = out && compare(130000, uints[4], 1E-4);
    out = out && compare(5000000, uints[5], 1E-4);
    if (!out)
    {
        throw std::runtime_error("Failed uint radix sort");
    }

    return out;
}

#endif