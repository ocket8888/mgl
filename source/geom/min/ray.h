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
#ifndef __RAY__
#define __RAY__

#include "math/min/utility.h"

namespace min
{

template <typename T, template <typename> class vec>
class ray
{
  private:
    vec<T> _origin;
    vec<T> _dir;
    vec<T> _inv;

  public:
    ray() : _origin(), _dir(), _inv() {}
    ray(const vec<T>&, const vec<T>&);

    inline T set(const vec<T>&, const vec<T>&);
    inline const vec<T> &get_origin() const;
    inline const vec<T> &get_direction() const;
    inline const vec<T> &get_inverse() const;
    inline vec<T> interpolate(const T) const;
};
}

#endif
