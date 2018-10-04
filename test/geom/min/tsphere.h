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
#ifndef TESTSPHERE
#define TESTSPHERE

#include <stdexcept>
#include <vector>

#include "geom/min/sphere.h"
#include "math/min/vec2.h"
#include "math/min/vec3.h"
#include "math/min/vec4.h"
#include "platform/min/test.h"

template <typename T, template <typename> class vec> bool points_inside(const min::sphere<T,vec>&, const std::vector<vec<T>>&);
bool test_sphere();

#endif
