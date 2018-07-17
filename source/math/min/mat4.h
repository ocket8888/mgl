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
#ifndef __MATRIX4__
#define __MATRIX4__

namespace min
{
template <typename T>
class mat3;
template <typename T>
class quat;
template <typename T>
class vec3;
}

#include <cmath>
#include "mat3.h"
#include "quat.h"
#include "vec3.h"
#include "vec4.h"

// This matrix is left handed and follows the following conventions:
// Rotation can be instantiated from a mat2<T>, mat3<T>, or a quat<T>
// Rotation takes place around the 'Y' axis
// Transformation to 3D
//          (y)  (z)
//           ^  /
//           | /
//           |/
//           ~-------- (x)
// This matrix is can be orthogonal or can be a transformation matrix in 3D

namespace min
{

template <typename T>
class mat4
{
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
    T _j;
    T _k;
    T _l;
    T _m;
    T _n;
    T _o;
    T _p;

  public:
    // constructs an identity matrix
    mat4()
        : _a(1.0), _b(0.0), _c(0.0), _d(0.0), _e(0.0), _f(1.0), _g(0.0), _h(0.0), _i(0.0), _j(0.0), _k(1.0), _l(0.0), _m(0.0), _n(0.0), _o(0.0), _p(1.0) {}

    // convenience constructor for direct loading
    mat4(T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T m, T n, T o, T p)
        : _a(a), _b(b), _c(c), _d(d), _e(e), _f(f), _g(g), _h(h), _i(i), _j(j), _k(k), _l(l), _m(m), _n(n), _o(o), _p(p) {}

    // constructs a translation matrix
    mat4(const vec3<T> &t)
        : _a(1.0), _b(0.0), _c(0.0), _d(0.0), _e(0.0), _f(1.0), _g(0.0), _h(0.0), _i(0.0), _j(0.0), _k(1.0), _l(0.0), _m(t.x), _n(t.y), _o(t.z), _p(1.0) {}

    // constructs a 3D rotation matrix
    mat4(const mat3<T> &r)
        : _a(r._a), _b(r._b), _c(r._c), _d(0.0), _e(r._d), _f(r._e), _g(r._f), _h(0.0), _i(r._g), _j(r._h), _k(r._i), _l(0.0), _m(0.0), _n(0.0), _o(0.0), _p(1.0) {}

    // constructs a matrix that first rotates then translates in 3D
    mat4(const vec3<T> &t, const mat3<T> &r)
        : _a(r._a), _b(r._b), _c(r._c), _d(0.0), _e(r._d), _f(r._e), _g(r._f), _h(0.0), _i(r._g), _j(r._h), _k(r._i), _l(0.0), _m(t.x), _n(t.y), _o(t.z), _p(1.0) {}

    // constructs a lookat matrix from 4 vectors: right(x), up(y), forward(z), and eye
    mat4(const vec3<T> &x, const vec3<T> &y, const vec3<T> &z, const vec3<T> &e)
        : _a(x.x), _b(x.y), _c(x.z), _d(-x.dot(e)), _e(y.x), _f(y.y), _g(y.z), _h(-y.dot(e)), _i(z.x), _j(z.y), _k(z.z), _l(-z.dot(e)), _m(0.0), _n(0.0), _o(0.0), _p(1.0) {}

    // constructs an orthographic projection matrix
    mat4(const T dx, const T dy, const T near, const T far)
        : _a(1.0 / dx), _b(0.0), _c(0.0), _d(0.0), _e(0.0), _f(1.0 / dy), _g(0.0), _h(0.0), _i(0.0), _j(0.0), _k(-2.0 / (far - near)), _l((far + near) / (far - near)), _m(0.0), _n(0.0), _o(0.0), _p(1.0) {}


    void one(vec4<T>&);
    void two(vec4<T>&);
    void three(vec4<T>&);
    void four(vec4<T>&);
    void w(const T);
    T w() const;
    mat4<T> operator*(const mat4<T>&) const;
    mat4<T> &operator*=(const mat4<T>&);
    vec4<T> operator*(const vec4<T>&) const;
    vec3<T> get_translation() const;
    mat4<T> &set_translation(const vec3<T>&);
    quat<T> get_rotation() const;
    mat4<T> &set_rotation(const mat3<T>&);
    vec3<T> get_scale() const;
    mat4<T> &set_scale(const vec3<T>&);
    bool invert();
    vec4<T> transform(const vec4<T>&) const;
    mat4<T> &transpose();
    mat4<T> transpose_multiply(const mat4<T>&) const;

};
}

#endif
