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

#include "template_math.h"

// These functions are only useful for cross-product return types within templates
template <typename T>
inline T min::align(const T v, const min::mat2<T> &m)
{
    // Since this is a 2D cross product, the vector in world space == vector in object space!
    return v;
}

template <typename T>
inline min::vec3<T> min::align(const min::vec3<T> &v, const min::quat<T> &q)
{
    // Convert the world space vector to object space
    return q.inverse().transform(v);
}

template <typename T>
inline min::vec4<T> min::align(const min::vec4<T> &v, const min::quat<T> &q)
{
    // Convert the world space vector to object space
    return q.inverse().transform(v);
}

template <typename T>
inline T min::transform(const T v, const min::mat2<T> &m)
{
    // Since this is a 2D cross product, the vector in world space == vector in object space!
    return v;
}

template <typename T>
inline min::vec3<T> min::transform(const min::vec3<T> &v, const min::quat<T> &q)
{
    // Convert the world space vector to object space
    return q.transform(v);
}

template <typename T>
inline min::vec4<T> min::transform(const min::vec4<T> &v, const min::quat<T> &q)
{
    // Convert the world space vector to object space
    return q.transform(v);
}

template <typename T>
inline min::vec2<T> min::cross(const T w, const min::vec2<T> &r)
{
    return r.orthogonal() * w;
}

template <typename T>
inline min::vec3<T> min::cross(const min::vec3<T> &w, const min::vec3<T> &r)
{
    return w.cross(r);
}

template <typename T>
inline min::vec4<T> min::cross(const min::vec4<T> &w, const min::vec4<T> &r)
{
    return w.cross(r);
}

template <typename T>
inline T min::dot(const T v1, const T v2)
{
    return v1 * v2;
}

template <typename T>
inline T min::dot(const min::vec3<T> &v1, const min::vec3<T> &v2)
{
    return v1.dot(v2);
}

template <typename T>
inline T min::dot(const min::vec4<T> &v1, const min::vec4<T> &v2)
{
    return v1.dot(v2);
}

template <typename T>
inline T min::inverse(const T x)
{
    return 1.0 / x;
}

template <typename T>
inline min::vec3<T> min::inverse(const min::vec3<T> &v)
{
    const T x = 1.0 / v.x();
    const T y = 1.0 / v.y();
    const T z = 1.0 / v.z();

    return vec3<T>(x, y, z);
}

template <typename T>
inline min::vec4<T> min::inverse(const min::vec4<T> &v)
{
    const T x = 1.0 / v.x();
    const T y = 1.0 / v.y();
    const T z = 1.0 / v.z();

    return vec4<T>(x, y, z, 1.0);
}

// AABB
template <typename T>
inline T min::get_inertia(const min::aabbox<T, min::vec2> &box, const T mass)
{
    // Iz = (1/12) * (x^2 + y^2)
    const vec3<T> &b = box.get_extent();

    // return the local inertia
    return (b.x() * b.x() + b.y() * b.y()) * mass * 0.0833;
}

template <typename T>
inline min::vec3<T> min::get_inertia(const min::aabbox<T, min::vec3> &box, const T mass)
{
    // Ix = (1/12) * (y^2 + z^2)
    // Iy = (1/12) * (x^2 + z^2)
    // Iz = (1/12) * (x^2 + y^2)
    const vec3<T> &b = box.get_extent();
    const T x2 = b.x() * b.x();
    const T y2 = b.y() * b.y();
    const T z2 = b.z() * b.z();

    // return the local inertia
    return vec3<T>(y2 + z2, x2 + z2, x2 + y2) * mass * 0.0833;
}

template <typename T>
inline min::vec4<T> min::get_inertia(const min::aabbox<T, min::vec4> &box, const T mass)
{
    // Ix = (1/12) * (y^2 + z^2)
    // Iy = (1/12) * (x^2 + z^2)
    // Iz = (1/12) * (x^2 + y^2)
    const vec3<T> &b = box.get_extent();
    const T x2 = b.x() * b.x();
    const T y2 = b.y() * b.y();
    const T z2 = b.z() * b.z();

    // return the local inertia
    return vec3<T>(y2 + z2, x2 + z2, x2 + y2) * mass * 0.0833;
}

// OOBB
template <typename T>
inline T min::get_inertia(const min::oobbox<T, min::vec2> &box, const T mass)
{
    // Iz = (1/12) * (x^2 + y^2)
    const vec3<T> &b = box.get_extent();

    // return the local inertia
    return (b.x() * b.x() + b.y() * b.y()) * mass * 0.0833;
}

template <typename T>
inline min::vec3<T> min::get_inertia(const min::oobbox<T, min::vec3> &box, const T mass)
{
    // Ix = (1/12) * (y^2 + z^2)
    // Iy = (1/12) * (x^2 + z^2)
    // Iz = (1/12) * (x^2 + y^2)
    const vec3<T> &b = box.get_extent();
    const T x2 = b.x() * b.x();
    const T y2 = b.y() * b.y();
    const T z2 = b.z() * b.z();

    // return the local inertia
    return vec3<T>(y2 + z2, x2 + z2, x2 + y2) * mass * 0.0833;
}

template <typename T>
inline min::vec4<T> min::get_inertia(const min::oobbox<T, min::vec4> &box, const T mass)
{
    // Ix = (1/12) * (y^2 + z^2)
    // Iy = (1/12) * (x^2 + z^2)
    // Iz = (1/12) * (x^2 + y^2)
    const vec3<T> &b = box.get_extent();
    const T x2 = b.x() * b.x();
    const T y2 = b.y() * b.y();
    const T z2 = b.z() * b.z();

    // return the local inertia
    return vec4<T>(y2 + z2, x2 + z2, x2 + y2, 1.0) * mass * 0.0833;
}

// SPHERE
template <typename T>
inline T min::get_inertia(const min::sphere<T, min::vec2> &s, const T mass)
{
    // Iz = m*r^2/2

    // return the local inertia
    return s.get_square_radius() * mass * 0.25;
}

template <typename T>
inline min::vec3<T> min::get_inertia(const min::sphere<T, min::vec3> &s, const T mass)
{
    // Ixyz = (2.0/5.0)*m*r^2
    T inertia = s.get_square_radius() * mass * 0.4;

    // return the local inertia
    return vec3<T>().set_all(inertia);
}

template <typename T>
inline min::vec4<T> min::get_inertia(const min::sphere<T, min::vec4> &s, const T mass)
{
    // Ixyz = (2.0/5.0)*m*r^2
    T inertia = s.get_square_radius() * mass * 0.4;

    // return the local inertia
    return vec4<T>().set_all(inertia);
}

// Can't rotate an AABB reliably so the default implementation is empty
template <typename T>
inline void min::rotate(min::aabbox<T, min::vec2> &box, const min::mat2<T> &mat) {}

template <typename T>
inline void min::rotate(min::aabbox<T, min::vec3> &box, const min::quat<T> &q) {}

template <typename T>
inline void min::rotate(min::aabbox<T, min::vec4> &box, const min::quat<T> &q) {}

template <typename T>
inline void min::rotate(min::oobbox<T, min::vec2> &box, const min::mat2<T> &m)
{
    // Set shape rotation
    box.set_rotation(m);
}

template <typename T>
inline void min::rotate(min::oobbox<T, min::vec3> &box, const min::quat<T> &q)
{
    // Set shape rotation
    box.set_rotation(q);
}

template <typename T>
inline void min::rotate(min::oobbox<T, min::vec4> &box, const min::quat<T> &q)
{
    // Set shape rotation
    box.set_rotation(q);
}

// No need to rotate sphere shape so these are empty on purpose
template <typename T>
inline void min::rotate(min::sphere<T, min::vec2> &s, const min::mat2<T> &mat) {}

template <typename T>
inline void min::rotate(min::sphere<T, min::vec3> &s, const min::quat<T> &q) {}

template <typename T>
inline void min::rotate(min::sphere<T, min::vec4> &s, const min::quat<T> &q) {}
