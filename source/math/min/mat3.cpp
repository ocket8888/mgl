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

#include "mat3.h"

// constructs a 3D rotation matrix
template <typename T>
min::mat3<T>::mat3(const min::quat<T> &r)
{
    T xx = r.x() * r.x();
    T yy = r.y() * r.y();
    T zz = r.z() * r.z();
    T xw = r.x() * r.w();
    T yw = r.y() * r.w();
    T zw = r.z() * r.w();
    T xy = r.x() * r.y();
    T xz = r.x() * r.z();
    T yz = r.y() * r.z();

    _a = 1.0 - (2.0 * (yy + zz));
    _b = 2.0 * (xy + zw);
    _c = 2.0 * (xz - yw);
    _d = 2.0 * (xy - zw);
    _e = 1.0 - (2.0 * (xx + zz));
    _f = 2.0 * (yz + xw);
    _g = 2.0 * (xz + yw);
    _h = 2.0 * (yz - xw);
    _i = 1.0 - (2.0 * (xx + yy));
}

template <typename T>
inline void min::mat3<T>::one(min::vec3<T> &v)
{
    _a = v.x();
    _b = v.y();
    _c = v.z();
}

template <typename T>
inline void min::mat3<T>::two(min::vec3<T> &v)
{
    _d = v.x();
    _e = v.y();
    _f = v.z();
}

template <typename T>
inline void min::mat3<T>::three(min::vec3<T> &v)
{
    _g = v.x();
    _h = v.y();
    _i = v.z();
}

template <typename T>
inline void min::mat3<T>::w(const T w)
{
    _i = w;
}

template <typename T>
inline T min::mat3<T>::w() const
{
    return _i;
}


template <typename T>
inline min::mat3<T> min::mat3<T>::operator*(const min::mat3<T> &A) const
{
    mat3<T> out;
    out._a = _a * A._a + _b * A._d + _c * A._g;
    out._b = _a * A._b + _b * A._e + _c * A._h;
    out._c = _a * A._c + _b * A._f + _c * A._i;
    out._d = _d * A._a + _e * A._d + _f * A._g;
    out._e = _d * A._b + _e * A._e + _f * A._h;
    out._f = _d * A._c + _e * A._f + _f * A._i;
    out._g = _g * A._a + _h * A._d + _i * A._g;
    out._h = _g * A._b + _h * A._e + _i * A._h;
    out._i = _g * A._c + _h * A._f + _i * A._i;

    return out;
}

template <typename T>
inline min::mat3<T> &min::mat3<T>::operator*=(const min::mat3<T> &A)
{
    T a = _a * A._a + _b * A._d + _c * A._g;
    T b = _a * A._b + _b * A._e + _c * A._h;
    T c = _a * A._c + _b * A._f + _c * A._i;
    T d = _d * A._a + _e * A._d + _f * A._g;
    T e = _d * A._b + _e * A._e + _f * A._h;
    T f = _d * A._c + _e * A._f + _f * A._i;
    T g = _g * A._a + _h * A._d + _i * A._g;
    T h = _g * A._b + _h * A._e + _i * A._h;
    T i = _g * A._c + _h * A._f + _i * A._i;

    _a = a;
    _b = b;
    _c = c;
    _d = d;
    _e = e;
    _f = f;
    _g = g;
    _h = h;
    _i = i;

    return *this;
}

template <typename T>
inline min::vec3<T> min::mat3<T>::operator*(const min::vec3<T> &A) const
{
    T x = _a * A.x() + _d * A.y() + _g * A.z();
    T y = _b * A.x() + _e * A.y() + _h * A.z();
    T z = _c * A.x() + _f * A.y() + _i * A.z();

    return vec3<T>(x, y, z);
}

template <typename T>
inline min::vec2<T> min::mat3<T>::get_translation() const
{
    return vec2<T>(_g, _h);
}

template <typename T>
inline min::mat3<T> &min::mat3<T>::set_translation(const min::vec2<T> &vec)
{
    _g = vec.x();
    _h = vec.y();
    return *this;
}

template <typename T>
inline min::mat2<T> min::mat3<T>::get_rotation() const
{
    return mat2<T>(_a, _b, _d, _e);
}
// Default rotation is about the Z axis

template <typename T>
inline min::mat3<T> &min::mat3<T>::set_rotation(const min::mat2<T> &rot)
{
    _a = rot._xc;
    _b = rot._ys;
    _d = rot._xs;
    _e = rot._yc;
    return *this;
}

template <typename T>
inline min::mat2<T> min::mat3<T>::get_rotation_x() const
{
    return mat2<T>(_e, _f, _h, _i);
}

template <typename T>
inline min::mat3<T> &min::mat3<T>::set_rotation_x(const min::mat2<T> &rot)
{
    _e = rot._xc;
    _f = rot._ys;
    _h = rot._xs;
    _i = rot._yc;
    return *this;
}

template <typename T>
inline min::mat2<T> min::mat3<T>::get_rotation_y() const
{
    return mat2<T>(_a, _c, _g, _i);
}

template <typename T>
inline min::mat3<T> &min::mat3<T>::set_rotation_y(const min::mat2<T> &rot)
{
    _a = rot._xc;
    _c = rot._xs;
    _g = rot._ys;
    _i = rot._yc;
    return *this;
}

template <typename T>
inline min::vec2<T> min::mat3<T>::get_scale() const
{
    return vec2<T>(_a, _e);
}

template <typename T>
inline min::mat3<T> &min::mat3<T>::set_scale(const min::vec2<T> &s)
{
    _a = s.x();
    _e = s.y();
    return *this;
}

template <typename T>
inline bool min::mat3<T>::invert()
{
    T a, b, c, d, e, f, g, h, i, det;

    a = _e * _i - _h * _f;
    b = _h * _c - _b * _i;
    c = _b * _f - _e * _c;
    d = _g * _f - _d * _i;
    e = _a * _i - _g * _c;
    f = _d * _c - _a * _f;
    g = _d * _h - _g * _e;
    h = _g * _b - _a * _h;
    i = _a * _e - _d * _b;

    det = _a * _e * _i + _b * _f * _g + _c * _d * _h - _a * _f * _h - _c * _e * _g - _b * _d * _i;
    if (std::abs(det) <= var<T>::TOL_REL)
    {
        return false;
    }

    det = 1.0 / det;
    _a = a * det;
    _b = b * det;
    _c = c * det;
    _d = d * det;
    _e = e * det;
    _f = f * det;
    _g = g * det;
    _h = h * det;
    _i = i * det;

    return true;
}

template <typename T>
inline min::vec3<T> min::mat3<T>::transform(const min::vec3<T> &v) const
{
    // This matches quat<T> API!
    return this->operator*(v);
}

template <typename T>
inline min::mat3<T> &min::mat3<T>::transpose()
{
    T swap = _b;
    _b = _d;
    _d = swap;

    swap = _c;
    _c = _g;
    _g = swap;

    swap = _f;
    _f = _h;
    _h = swap;

    return *this;
}
