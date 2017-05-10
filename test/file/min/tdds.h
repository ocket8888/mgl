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
#ifndef __TESTDDS__
#define __TESTDDS__

#include <min/test.h>
#include <min/dds.h>
#include <stdexcept>

bool test_dds()
{
    bool out = true;

    // Local variables
    int s;
    int w;
    int h;
    std::vector<uint8_t> data;
    const min::dds image = min::dds("data/texture/cube.dds");
    s = image.get_size();
    w = image.get_width();
    h = image.get_height();
    out = out && compare(183, w);
    out = out && compare(275, h);
    out = out && compare(25392, s);
    if (!out)
    {
        throw std::runtime_error("Failed dds image constructor properties");
    }

    data = image.get_pixels();
    out = out && compare(25392, data.size());
    if (!out)
    {
        throw std::runtime_error("Failed dds image size");
    }

    return out;
}

#endif