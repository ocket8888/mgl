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

#include "tran3.h"


template <typename T>
min::tran3<T>::tran3() : _m() {}

template <typename T>
min::tran3<T>::tran3(const min::vec3<T> &t) : _m(t) {}

template <typename T>
min::tran3<T>::tran3(const min::quat<T> &r) : _m(r) {}

template <typename T>
min::tran3<T>::tran3(const min::vec3<T> &t, const min::quat<T> &r) : _m(t, r) {}

template <typename T>
min::tran3<T>::tran3(const min::vec3<T> &t, const min::quat<T> &r, const min::vec3<T> &s) : _m(t, r) { scale(s); }

template <typename T>
min::tran3<T> &min::tran3<T>::translate(const T x, const T y, const T z)
{
    return translate(vec3<T>(x, y, z));
}

template <typename T>
min::tran3<T> &min::tran3<T>::translate(const min::vec3<T> &t)
{
    mat4<T> translation = mat4<T>(t);
    _m *= translation;
    return *this;
}

template <typename T>
min::tran3<T> &min::tran3<T>::rotate(const min::quat<T> &r)
{
    mat4<T> rotation = mat4<T>(r);
    _m *= rotation;
    return *this;
}

template <typename T>
min::tran3<T> &min::tran3<T>::rotate_x(const min::mat2<T> &r)
{
    mat4<T> rotation = mat4<T>(mat3<T>().set_rotation_x(r));
    _m *= rotation;
    return *this;
}

template <typename T>
min::tran3<T> &min::tran3<T>::rotate_y(const min::mat2<T> &r)
{
    mat4<T> rotation = mat4<T>(mat3<T>().set_rotation_y(r));
    _m *= rotation;
    return *this;
}
// Default rotation is about the Z axis

template <typename T>
min::tran3<T> &min::tran3<T>::rotate_z(const min::mat2<T> &r)
{
    mat4<T> rotation = mat4<T>(mat3<T>(r));
    _m *= rotation;
    return *this;
}

template <typename T>
min::tran3<T> &min::tran3<T>::scale(const T x, const T y, const T z)
{
    return scale(vec3<T>(x, y, z));
}

template <typename T>
min::tran3<T> &min::tran3<T>::scale(const min::vec3<T> &s)
{
    mat4<T> scale = mat4<T>();
    scale.set_scale(s);
    _m *= scale;
    return *this;
}

template <typename T>
min::tran3<T> &min::tran3<T>::transpose()
{
    _m.transpose();
    return *this;
}

template <typename T>
min::tran3<T> &min::tran3<T>::invert()
{
    _m.invert();
    return *this;
}

template <typename T>
min::vec4<T> min::tran3<T>::transform(const min::vec4<T> &v) const
{
    return _m * v;
}

template <typename T>
const min::mat4<T> &min::tran3<T>::m() const
{
    return _m;
}
