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

#include "tran2.h"


template <typename T>
min::tran2<T>::tran2() : _m() {}

template <typename T>
min::tran2<T>::tran2(const min::vec2<T> &t) : _m(t) {}

template <typename T>
min::tran2<T>::tran2(const min::mat2<T> &r) : _m(r) {}

template <typename T>
min::tran2<T>::tran2(const min::vec2<T> &t, const min::mat2<T> &r) : _m(t, r) {}

template <typename T>
min::tran2<T>::tran2(const min::vec2<T> &t, const min::mat2<T> &r, const min::vec2<T> &s) : _m(t, r) { scale(s); }

template <typename T>
min::tran2<T> &min::tran2<T>::translate(const T x, const T y)
{
    return translate(vec2<T>(x, y));
}

template <typename T>
min::tran2<T> &min::tran2<T>::translate(const min::vec2<T> &t)
{
    mat3<T> translation = mat3<T>(t);
    _m *= translation;
    return *this;
}

template <typename T>
min::tran2<T> &min::tran2<T>::rotate(const min::mat2<T> &r)
{
    mat3<T> rotation = mat3<T>(r);
    _m *= rotation;
    return *this;
}

template <typename T>
min::tran2<T> &min::tran2<T>::scale(const T x, const T y)
{
    return scale(vec2<T>(x, y));
}

template <typename T>
min::tran2<T> &min::tran2<T>::scale(const min::vec2<T> &s)
{
    mat3<T> scale = mat3<T>();
    scale.set_scale(s);
    _m *= scale;
    return *this;
}

template <typename T>
min::tran2<T> &min::tran2<T>::transpose()
{
    _m.transpose();
    return *this;
}

template <typename T>
min::tran2<T> &min::tran2<T>::invert()
{
    _m.invert();
    return *this;
}

template <typename T>
min::vec3<T> min::tran2<T>::transform(const min::vec3<T> &v) const
{
    return _m * v;
}

template <typename T>
const min::mat3<T> &min::tran2<T>::m() const
{
    return _m;
}
