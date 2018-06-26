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

#include "cubic.h"

//// bezier ////
// Assumes t is between domain [0.0, 1.0]
template <typename T, template <typename> class vec>
vec<T> min::bezier<T,vec>::interpolate(const T t) const
{
    // Precalculate time constants
    const T t2 = t * t;
    const T t3 = t2 * t;
    const T u = 1.0 - t;
    const T u2 = u * u;
    const T u3 = u2 * u;

    // Calculate blending functions
    const T b0 = u3;
    const T b1 = 3.0 * u2 * t;
    const T b2 = 3.0 * u * t2;
    const T b3 = t3;

    // return interpolated point on bezier curve
    return (_p0 * b0) + (_p1 * b1) + (_p2 * b2) + (_p3 * b3);
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bezier<T,vec>::begin() const
{
    return _p0;
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bezier<T,vec>::end() const
{
    return _p3;
}


//// bezier_deriv ////
// Assumes t is between domain [0.0, 1.0]
template <typename T, template <typename> class vec>
vec<T> min::bezier_deriv<T,vec>::interpolate(const T t) const
{
    // Precalculate time constants
    const T t2 = t * t;
    const T u = 1.0 - t;
    const T u2 = u * u;

    // Calculate blending functions
    const T b0 = -3.0 * u2;
    const T b1 = 3.0 - (12.0 * t) + (9.0 * t2);
    const T b2 = (6.0 * t) - (9.0 * t2);
    const T b3 = 3.0 * t2;

    // return interpolated point on bezier_deriv curve
    return (_p0 * b0) + (_p1 * b1) + (_p2 * b2) + (_p3 * b3);
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bezier_deriv<T,vec>::begin() const
{
    return _p0;
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bezier_deriv<T,vec>::end() const
{
    return _p3;
}


//// bspline ////
// Assumes t is between domain [0.0, 1.0]
template <typename T, template <typename> class vec>
vec<T> min::bspline<T,vec>::interpolate(const T t) const
{
    // Precalculate time constants
    const T t2 = t * t;
    const T t3 = t2 * t;
    const T u = 1.0 - t;
    const T u3 = u * u * u;

    // Calculate blending functions
    const T b0 = u3;
    const T b1 = 4.0 - (6.0 * t2) + (3.0 * t3);
    const T b2 = 1.0 + (3.0 * t) + (3.0 * t2) - (3.0 * t3);
    const T b3 = t3;

    // return interpolated point on bspline curve
    return ((_p0 * b0) + (_p1 * b1) + (_p2 * b2) + (_p3 * b3)) * 0.166667;
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bspline<T,vec>::begin() const
{
    return _p1;
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bspline<T,vec>::end() const
{
    return _p2;
}


//// bspline_deriv ////
// Assumes t is between domain [0.0, 1.0]
template <typename T, template <typename> class vec>
vec<T> min::bspline_deriv<T, vec>::interpolate(const T t) const
{
    // Precalculate time constants
    const T t2 = t * t;
    const T u = 1.0 - t;
    const T u2 = u * u;

    // Calculate blending functions
    const T b0 = -0.5 * u2;
    const T b1 = (-2.0 * t) + (1.5 * t2);
    const T b2 = 0.5 + t - (1.5 * t2);
    const T b3 = 0.5 * t2;

    // return interpolated point on bspline_deriv curve
    return ((_p0 * b0) + (_p1 * b1) + (_p2 * b2) + (_p3 * b3));
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bspline_deriv<T, vec>::begin() const
{
    return _p1;
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::bspline_deriv<T, vec>::end() const
{
    return _p2;
}

//// hermite ////
// Assumes t is between domain [0.0, 1.0]
template <typename T, template <typename> class vec>
vec<T> min::hermite<T,vec>::interpolate(const T t) const
{
    // Precalculate time constants
    const T t2 = t * t;
    const T t3 = t2 * t;
    const T u = 1.0 - t;
    const T u2 = u * u;

    // Calculate blending functions
    const T b0 = 1.0 - (3.0 * t2) + (2.0 * t3);
    const T b1 = t2 * (3.0 - (2.0 * t));
    const T b2 = t * u2;
    const T b3 = t2 * u;

    // return interpolated point on hermite curve
    return (_p0 * b0) + (_p1 * b1) + (_t0 * b2) + (_t1 * b3);
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::hermite<T,vec>::begin() const
{
    return _p0;
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::hermite<T,vec>::end() const
{
    return _p1;
}


//// hermite_deriv ////
// Assumes t is between domain [0.0, 1.0]
template <typename T, template <typename> class vec>
vec<T> min::hermite_deriv<T,vec>::interpolate(const T t) const
{
    // Precalculate time constants
    const T t2 = t * t;
    const T u = 1.0 - t;

    // Calculate blending functions
    const T b0 = (-6.0 * t) + (6.0 * t2);
    const T b1 = 6.0 * t * u;
    const T b2 = 1.0 - (4.0 * t) + (3.0 * t2);
    const T b3 = (2.0 * t) - (3.0 * t2);

    // return interpolated point on hermite_deriv curve
    return (_p0 * b0) + (_p1 * b1) + (_t0 * b2) + (_t1 * b3);
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::hermite_deriv<T,vec>::begin() const
{
    return _p0;
}

template <typename T, template <typename> class vec>
const min::vec3<float> &min::hermite_deriv<T,vec>::end() const
{
    return _p1;
}
