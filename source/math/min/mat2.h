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
#ifndef __MATRIX2__
#define __MATRIX2__

namespace min
{
template <typename T>
class vec2;
template <typename T>
class mat3;
}

#include <cmath>
#include "mat3.h"
#include "utility.h"
#include "vec2.h"

// This matrix is left handed and follows the following conventions:
// (+) angle -> CCW
// (-) angle -> CW
// y == z-axis
// x == x-axis
// Rotation takes place around the 'Y' axis perpendicular to the 'screen'
// Transformation to 3D
//          (y)  (z)
//           ^  /
//           | /
//           |/
//           ~-------- (x)
// This matrix is always orthogonal

namespace min
{

template <typename T>
class mat2
{
    friend class mat3<T>;

  private:
    T _xc; // xcos(0)
    T _ys; // -ysin(0)
    T _xs; // xsin(0)
    T _yc; // ycos(0)
  public:
    // constructs an identity matrix
    mat2()
        : _xc(1.0), _ys(0.0), _xs(0.0), _yc(1.0) {}

    // convenience constructor for direct loading
    mat2(T xc, T ys, T xs, T yc)
        : _xc(xc), _ys(ys), _xs(xs), _yc(yc) {}

    mat2(const T);


    inline mat2<T> &operator*=(const mat2<T>&);
    inline mat2<T> operator*(const mat2<T>&) const;
    inline vec2<T> operator*(const vec2<T>&) const;
    inline mat2<T> inverse() const;
    inline mat2<T> &invert();
    inline vec2<T> transform(const vec2<T>&) const;
    inline mat2<T> &transpose();
};
}

#endif
