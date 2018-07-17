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
#ifndef QUATERNION
#define QUATERNION

namespace min
{
template <typename T>
class vec3;
}

#include <cmath>
#include "utility.h"
#include "vec3.h"
#include <stdexcept>

namespace min
{

template <typename T>
class quat
{
  private:
    T _w;
    T _x;
    T _y;
    T _z;

    quat<T> operator+(const quat<T>&) const;
    quat<T> operator*(const T) const;
    quat<T> &operator-=(const quat<T>&);

  public:
    quat() : _w(1.0), _x(0.0), _y(0.0), _z(0.0) {}
    quat(const T w, const T x, const T y, const T z) : _w(w), _x(x), _y(y), _z(z) {}
    quat(const T x, const T y, const T z);
    quat(const vec3<T>&);
    quat(const vec3<T>&, const T);
    quat(const vec3<T>&, const vec3<T>&);

    static quat<T> from_x_axis(const T, const vec3<T>&);
    void calculate_w();
    quat<T> conjugate() const;
    T dot(const quat<T>&) const;
    quat<T> inverse() const;
    static quat<T> lerp(const quat<T>&, const quat<T>&, const T);
    static quat<T> interpolate(const quat<T>&, const quat<T>&, const T);
    T magnitude() const;
    quat<T> &normalize();
    static quat<T> slerp(const quat<T>&, const quat<T>&, const T);
    vec3<T> transform(const vec3<T>&) const;
    quat<T> operator*(const quat<T>&) const;
    quat<T> &operator*=(const quat<T>&);
    T w() const;
    T x() const;
    void x(const T);
    T y() const;
    void y(const T);
    T z() const;
    void z(const T);
};
}

#endif
