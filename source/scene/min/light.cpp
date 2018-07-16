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

#include "light.h"


template <typename T>
min::light<T>::light() {}

template <typename T>
min::light<T>::light(const min::vec4<T> &color, const min::vec4<T> &position, const min::vec4<T> &power) : _color(color), _position(position), _power(power) {}

template <typename T>
const min::vec4<T> &min::light<T>::get_color() const
{
    return _color;
}

template <typename T>
const min::vec4<T> &min::light<T>::get_position() const
{
    return _position;
}

template <typename T>
const min::vec4<T> &min::light<T>::get_power() const
{
    return _power;
}

template <typename T>
void min::light<T>::set_color(const min::vec4<T> &color)
{
    _color = color;
}

template <typename T>
void min::light<T>::set_position(const min::vec4<T> &p)
{
    _position = p;
}

template <typename T>
void min::light<T>::set_power(const min::vec4<T> &power)
{
    _power = power;
}
