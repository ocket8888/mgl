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
#ifndef __TRANSFORM3__
#define __TRANSFORM3__

#include <cmath>
#include "mat4.h"
#include "quat.h"
#include "vec3.h"

namespace min
{

template <typename T>
class tran3
{
  private:
    mat4<T> _m;

  public:
    tran3();
    tran3(const vec3<T>&);
    tran3(const quat<T>&);
    tran3(const vec3<T>&, const quat<T>&);
    tran3(const vec3<T>&, const quat<T>&, const vec3<T>&);

    inline tran3<T> &translate(const T, const T, const T);
    inline tran3<T> &translate(const vec3<T>&);
    inline tran3<T> &rotate(const quat<T>&);
    inline tran3<T> &rotate_x(const mat2<T>&);
    inline tran3<T> &rotate_y(const mat2<T>&);
    inline tran3<T> &rotate_z(const mat2<T>&);
    inline tran3<T> &scale(const T, const T, const T);
    inline tran3<T> &scale(const vec3<T>&);
    inline tran3<T> &transpose();
    inline tran3<T> &invert();
    inline vec4<T> transform(const vec4<T>&) const;
    inline const mat4<T> &m() const;

};
}

#endif
