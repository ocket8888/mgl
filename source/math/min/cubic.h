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
#ifndef __CUBIC__
#define __CUBIC__

#include "vec3.h"

namespace min
{

template <typename T, template <typename> class vec>
class bezier
{
  private:
    vec<T> _p0;
    vec<T> _p1;
    vec<T> _p2;
    vec<T> _p3;

  public:
    bezier() {}
    bezier(const vec<T> &p0, const vec<T> &p1, const vec<T> &p2, const vec<T> &p3)
        : _p0(p0), _p1(p1), _p2(p2), _p3(p3) {}

    vec<T> interpolate(const T) const;
    const vec3<float> &begin() const;
    const vec3<float> &end() const;
};

template <typename T, template <typename> class vec>
class bezier_deriv
{
  private:
    vec<T> _p0;
    vec<T> _p1;
    vec<T> _p2;
    vec<T> _p3;

  public:
    bezier_deriv() {}
    bezier_deriv(const vec<T> &p0, const vec<T> &p1, const vec<T> &p2, const vec<T> &p3)
        : _p0(p0), _p1(p1), _p2(p2), _p3(p3) {}

    vec<T> interpolate(const T) const;
    const vec3<float> &begin() const;
    const vec3<float> &end() const;
};

template <typename T, template <typename> class vec>
class bspline
{
  private:
    vec<T> _p0;
    vec<T> _p1;
    vec<T> _p2;
    vec<T> _p3;

  public:
    bspline() {}
    // Points p1 & p2 define the curve endpoints; p0 and p3 are control points
    bspline(const vec<T> &p0, const vec<T> &p1, const vec<T> &p2, const vec<T> &p3)
        : _p0(p0), _p1(p1), _p2(p2), _p3(p3) {}

    vec<T> interpolate(const T) const;
    const vec3<float> &begin() const;
    const vec3<float> &end() const;
};

template <typename T, template <typename> class vec>
class bspline_deriv
{
  private:
    vec<T> _p0;
    vec<T> _p1;
    vec<T> _p2;
    vec<T> _p3;

  public:
    bspline_deriv() {}
    // Points p1 & p2 define the curve endpoints; p0 and p3 are control points
    bspline_deriv(const vec<T> &p0, const vec<T> &p1, const vec<T> &p2, const vec<T> &p3)
        : _p0(p0), _p1(p1), _p2(p2), _p3(p3) {}

    vec<T> interpolate(const T) const;
    const vec3<float> &begin() const;
    const vec3<float> &end() const;
};

template <typename T, template <typename> class vec>
class hermite
{
  private:
    vec<T> _p0;
    vec<T> _p1;
    vec<T> _t0;
    vec<T> _t1;

  public:
    hermite() {}
    // Points p0 & p1 and tangents at those points t0, t1
    hermite(const vec<T> &p0, const vec<T> &p1, const vec<T> &t0, const vec<T> &t1)
        : _p0(p0), _p1(p1), _t0(t0), _t1(t1) {}

    vec<T> interpolate(const T) const;
    const vec3<float> &begin() const;
    const vec3<float> &end() const;
};

template <typename T, template <typename> class vec>
class hermite_deriv
{
  private:
    vec<T> _p0;
    vec<T> _p1;
    vec<T> _t0;
    vec<T> _t1;

  public:
    hermite_deriv() {}
    // Points p0 & p1 and tangents at those points t0, t1
    hermite_deriv(const vec<T> &p0, const vec<T> &p1, const vec<T> &t0, const vec<T> &t1)
        : _p0(p0), _p1(p1), _t0(t0), _t1(t1) {}

    vec<T> interpolate(const T) const;
    const vec3<float> &begin() const;
    const vec3<float> &end() const;
};
}

#endif
