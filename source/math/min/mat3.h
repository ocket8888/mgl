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
#ifndef __MATRIX3__
#define __MATRIX3__

namespace min
{
template <typename T>
class mat2;
template <typename T>
class mat4;
template <typename T>
class quat;
template <typename T>
class vec2;
template <typename T>
class vec3;
}

#include <cmath>
#include "mat2.h"
#include "mat4.h"
#include "quat.h"
#include "vec2.h"
#include "vec3.h"

// This matrix is left handed and follows the following conventions:
// x-axis: (+) angle -> CW
// x-axis: (-) angle -> CCW
// y-axis: (+) angle -> CW
// y-axis: (-) angle -> CCW
// z-axis: (+) angle -> CCW
// z-axis: (-) angle -> CW
// Rotation takes place around the 'Y' axis
// Transformation to 3D
//          (y)  (z)
//           ^  /
//           | /
//           |/
//           ~-------- (x)
// This matrix is can be orthogonal or can be a transformation matrix in 2D

namespace min
{

template <typename T>
class mat3
{
    friend class mat4<T>;

  private:
    T _a;
    T _b;
    T _c;
    T _d;
    T _e;
    T _f;
    T _g;
    T _h;
    T _i;

  public:
    // constructs an identity matrix
    mat3()
        : _a(1.0), _b(0.0), _c(0.0), _d(0.0), _e(1.0), _f(0.0), _g(0.0), _h(0.0), _i(1.0) {}

    // constructs a translation matrix
    mat3(const vec2<T> &t)
        : _a(1.0), _b(0.0), _c(0.0), _d(0.0), _e(1.0), _f(0.0), _g(t.x()), _h(t.y()), _i(1.0) {}

    // constructs an orthogonal 3D rotation matrix around the Z axis
    mat3(const mat2<T> &r)
        : _a(r._xc), _b(r._ys), _c(0.0), _d(r._xs), _e(r._yc), _f(0.0), _g(0.0), _h(0.0), _i(1.0) {}

    // constructs a matrix that first rotates then translates in 2D
    mat3(const vec2<T> &t, const mat2<T> &r)
        : _a(r._xc), _b(r._ys), _c(0.0), _d(r._xs), _e(r._yc), _f(0.0), _g(t.x()), _h(t.y()), _i(1.0) {}

    // constructs a 3D rotation matrix
    mat3(const quat<T>&);


    void one(vec3<T>&);
    void two(vec3<T>&);
    void three(vec3<T>&);
    void w(const T);
    T w() const;
    mat3<T> operator*(const mat3<T>&) const;
    mat3<T> &operator*=(const mat3<T>&);
    vec3<T> operator*(const vec3<T>&) const;
    vec2<T> get_translation() const;
    mat3<T> &set_translation(const vec2<T>&);
    mat2<T> get_rotation() const;
    mat3<T> &set_rotation(const mat2<T>&);
    mat2<T> get_rotation_x() const;
    mat3<T> &set_rotation_x(const mat2<T>&);
    mat2<T> get_rotation_y() const;
    mat3<T> &set_rotation_y(const mat2<T>&);
    vec2<T> get_scale() const;
    mat3<T> &set_scale(const vec2<T>&);
    bool invert();
    vec3<T> transform(const vec3<T>&) const;
    mat3<T> &transpose();
};
}

#endif
