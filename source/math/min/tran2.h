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
#ifndef __TRANSFORM2__
#define __TRANSFORM2__

#include <cmath>
#include "mat2.h"
#include "mat3.h"
#include "vec2.h"
#include "vec3.h"

namespace min
{

template <typename T>
class tran2
{
  private:
    mat3<T> _m;

  public:
    tran2();
    tran2(const vec2<T>&);
    tran2(const mat2<T>&);
    tran2(const vec2<T>&, const mat2<T>&);
    tran2(const vec2<T>&, const mat2<T>&, const vec2<T>&);
    inline tran2<T> &translate(const T, const T);
    inline tran2<T> &translate(const vec2<T>&);
    inline tran2<T> &rotate(const mat2<T>&);
    inline tran2<T> &scale(const T, const T);
    inline tran2<T> &scale(const vec2<T>&);
    inline tran2<T> &transpose();
    inline tran2<T> &invert();
    inline vec3<T> transform(const vec3<T>&) const;
    inline const mat3<T> &m() const;
};
}

#endif
