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
#ifndef COORDSYS
#define COORDSYS

#include "mat2.h"

namespace min
{
template <typename T>
class vec2;
template <typename T>
class vec3;
template <typename T>
class vec4;
template <typename T>
class mat2;
template <typename T>
class quat;
}

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "quat.h"

namespace min
{

template <typename T, template <typename> class vec>
class coord_sys_base
{
  protected:
    vec<T> _x;
    vec<T> _y;

  public:
    coord_sys_base(const vec<T> &x, const vec<T> &y) : _x(x), _y(y) {}
    const vec<T> &x() const;
    const vec<T> &y() const;
};

// General case for vec3/vec4
template <typename T, template <typename> class vec>
class coord_sys : public coord_sys_base<T, vec>
{
};

// Specialization for vec2
template <typename T>
class coord_sys<T, vec2> : public coord_sys_base<T, vec2>
{
  public:
    coord_sys(const vec2<T> &x, const vec2<T> &y) : coord_sys_base<T, vec2>(x, y) {}
    void rotate(const mat2<T>&);
    vec2<T> align(const vec2<T>&) const;
};

// Specialization for vec3
template <typename T>
class coord_sys<T, vec3> : public coord_sys_base<T, vec3>
{
  private:
    vec3<T> _z;

  public:
    coord_sys(const vec3<T> &x, const vec3<T> &y, const vec3<T> &z) : coord_sys_base<T, vec3>(x, y), _z(z) {}

    const min::vec3<T> &z() const;
    void rotate(const quat<T>&);
    vec3<T> align(const vec3<T>&) const;

};

// Specialization for vec4
template <typename T>
class coord_sys<T, vec4> : public coord_sys_base<T, vec4>
{
  private:
    vec4<T> _z;

  public:
    coord_sys(const vec4<T> &x, const vec4<T> &y, const vec4<T> &z) : coord_sys_base<T, vec4>(x, y), _z(z) {}

    const vec4<T> &z() const;
    void rotate(const quat<T>&);
    vec4<T> align(const vec4<T>&) const;
};
}

#endif
