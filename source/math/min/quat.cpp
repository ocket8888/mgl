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

#include "quat.h"

template <typename T>
inline min::quat<T> min::quat<T>::operator+(const min::quat<T> &q) const
{
    const T w = _w + q.w();
    const T x = _x + q.x();
    const T y = _y + q.y();
    const T z = _z + q.z();
    return quat<T>(w, x, y, z);
}

template <typename T>
inline min::quat<T> min::quat<T>::operator*(const T a) const
{
    const T w = _w * a;
    const T x = _x * a;
    const T y = _y * a;
    const T z = _z * a;
    return quat<T>(w, x, y, z);
}

template <typename T>
inline min::quat<T> &min::quat<T>::operator-=(const min::quat<T> &q)
{
    _w -= q.w();
    _x -= q.x();
    _y -= q.y();
    _z -= q.z();
    return *this;
}


template <typename T>
min::quat<T>::quat(const T x, const T y, const T z) : _w(1.0), _x(x), _y(y), _z(z)
{
    normalize();
}

template <typename T>
min::quat<T>::quat(const min::vec3<T> &v) : _w(1.0), _x(v.x()), _y(v.y()), _z(v.z())
{
    normalize();
}

template <typename T>
min::quat<T>::quat(const min::vec3<T> &vector, const T degAngle)
{
    const T angle_2 = deg_to_rad(degAngle) * 0.5;
    const T cos_2 = cos(angle_2);
    const T sin_2 = sin(angle_2);
    _w = cos_2;
    _x = vector.x() * sin_2;
    _y = vector.y() * sin_2;
    _z = vector.z() * sin_2;

    // Normalize
    normalize();
}

// Must be normalized vectors
template <typename T>
min::quat<T>::quat(const min::vec3<T> &v1, const min::vec3<T> &v2)
{
    const T cos_theta = v1.dot(v2);
    if (cos_theta < var<T>::TOL_NONE)
    {
        vec3<T> c = v1.cross_y();
        if (std::abs(v1.y()) > std::abs(v1.x()))
        {
            c = v1.cross_x();
        }
        c.normalize();
        _w = 0.0;
        _x = c.x();
        _y = c.y();
        _z = c.z();
    }
    else if (cos_theta > var<T>::TOL_PONE)
    {
        _w = 1.0;
        _x = 0.0;
        _y = 0.0;
        _z = 0.0;
    }
    else
    {
        const T s = std::sqrt((1.0 + cos_theta) * 2.0);
        const T inv_s = 1.0 / s;
        const vec3<T> c = v1.cross(v2);
        _w = s * 0.5;
        _x = c.x() * inv_s;
        _y = c.y() * inv_s;
        _z = c.z() * inv_s;
        normalize();
    }
}

// Special case quaternion between X-axis and vector
template <typename T>
inline min::quat<T> min::quat<T>::from_x_axis(const T x, const min::vec3<T> &v1)
{
    quat<T> q;

    const T cos_theta = x * v1.dot_x();
    if (cos_theta < var<T>::TOL_NONE)
    {
        vec3<T> c = v1.cross_x().normalize();
        q._w = 0.0;
        q._x = 0.0;
        q._y = c.y() * x;
        q._z = c.z() * x;
    }
    else if (cos_theta > var<T>::TOL_PONE)
    {
        q._w = 1.0;
        q._x = 0.0;
        q._y = 0.0;
        q._z = 0.0;
    }
    else
    {
        const T s = std::sqrt((1.0 + cos_theta) * 2.0);
        const T inv_s = 1.0 / s;
        const vec3<T> c = v1.cross_x();
        q._w = s * 0.5;
        q._x = 0.0;
        q._y = c.y() * inv_s;
        q._z = c.z() * inv_s;
        q.normalize();
    }

    return q;
}

template <typename T>
inline void min::quat<T>::calculate_w()
{
    const T t = 1.0 - (_x * _x) - (_y * _y) - (_z * _z);
    if (t < 0.0)
    {
        _w = 0.0;
    }
    else
    {
        _w = sqrt(t);
    }

    // Normalize the quaternion to ensure unit length
    normalize();
}

template <typename T>
inline min::quat<T> min::quat<T>::conjugate() const
{
    const T w = _w;
    const T x = -_x;
    const T y = -_y;
    const T z = -_z;
    return quat<T>(w, x, y, z);
}

template <typename T>
inline T min::quat<T>::dot(const min::quat<T> &q) const
{
    return _w * q.w() + _x * q.x() + _y * q.y() + _z * q.z();
}

template <typename T>
inline min::quat<T> min::quat<T>::inverse() const
{
    return conjugate();
}
// Linear interpolation

template <typename T>
inline min::quat<T> min::quat<T>::lerp(const min::quat<T> &v0, const min::quat<T> &v1, const T interpolation)
{
    const T w = v0.w() + (v1.w() - v0.w()) * interpolation;
    const T x = v0.x() + (v1.x() - v0.x()) * interpolation;
    const T y = v0.y() + (v1.y() - v0.y()) * interpolation;
    const T z = v0.z() + (v1.z() - v0.z()) * interpolation;
    return quat<T>(w, x, y, z).normalize();
}

template <typename T>
inline min::quat<T> min::quat<T>::interpolate(const min::quat<T> &v0, const min::quat<T> &v1, const T t)
{
    return slerp(v0, v1, t);
}

template <typename T>
inline T min::quat<T>::magnitude() const
{
    return sqrt(_w * _w + _x * _x + _y * _y + _z * _z);
}

template <typename T>
inline min::quat<T> &min::quat<T>::normalize()
{
    const T mag = magnitude();
    _w /= mag;
    _x /= mag;
    _y /= mag;
    _z /= mag;

    return *this;
}
// Spherical interpolation

template <typename T>
inline min::quat<T> min::quat<T>::slerp(const min::quat<T> &v0, const min::quat<T> &v1, const T t)
{
    quat<T> v2 = v1;
    T cos_theta = v0.dot(v1);

    // If two quaternions are too close,
    if (cos_theta > 0.95)
    {
        return quat<T>::lerp(v0, v1, t);
    }
    // If dot product is negative reverse the quaternion
    // to take shortest path
    else if (cos_theta < 0.0)
    {
        v2 = v1.conjugate();
        cos_theta = -cos_theta;
    }

    clamp<T>(cos_theta, -1.0, 1.0);
    T theta = std::acos(cos_theta) * t;

    // v1 - v0 * cos(theta)
    v2 -= v0 * cos_theta;
    v2.normalize();

    return (v0 * std::cos(theta)) + (v2 * std::sin(theta));
}

template <typename T>
inline min::vec3<T> min::quat<T>::transform(const min::vec3<T> &p) const
{
    // Get the pure quaternion P
    const quat<T> P = quat<T>(0.0, p.x(), p.y(), p.z());

    // Transform to Q
    const quat<T> Q = (*this) * P * conjugate();

    // Return the rotated vector part
    return vec3<T>(Q.x(), Q.y(), Q.z());
}

template <typename T>
inline min::quat<T> min::quat<T>::operator*(const min::quat<T> &b) const
{
    const T w = b.w() * _w - b.x() * _x - b.y() * _y - b.z() * _z;
    const T x = b.w() * _x + b.x() * _w - b.y() * _z + b.z() * _y;
    const T y = b.w() * _y + b.x() * _z + b.y() * _w - b.z() * _x;
    const T z = b.w() * _z - b.x() * _y + b.y() * _x + b.z() * _w;
    return quat<T>(w, x, y, z);
}

template <typename T>
inline min::quat<T> &min::quat<T>::operator*=(const min::quat<T> &b)
{
    const T w = b.w() * _w - b.x() * _x - b.y() * _y - b.z() * _z;
    const T x = b.w() * _x + b.x() * _w - b.y() * _z + b.z() * _y;
    const T y = b.w() * _y + b.x() * _z + b.y() * _w - b.z() * _x;
    const T z = b.w() * _z - b.x() * _y + b.y() * _x + b.z() * _w;

    _w = w;
    _x = x;
    _y = y;
    _z = z;

    return *this;
}

template <typename T>
inline T min::quat<T>::w() const
{
    return _w;
}

template <typename T>
inline T min::quat<T>::x() const
{
    return _x;
}

template <typename T>
inline void min::quat<T>::x(const T x)
{
    _x = x;
}

template <typename T>
inline T min::quat<T>::y() const
{
    return _y;
}

template <typename T>
inline void min::quat<T>::y(const T y)
{
    _y = y;
}

template <typename T>
inline T min::quat<T>::z() const
{
    return _z;
}

template <typename T>
inline void min::quat<T>::z(const T z)
{
    _z = z;
}