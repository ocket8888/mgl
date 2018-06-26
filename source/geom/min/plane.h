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
#ifndef __PLANE__
#define __PLANE__

// This class solves the following plane equation:
// n · (x - x0) = d = 0;
// n · x - n · x0 = d = 0;
// n · x - c = d = 0;

namespace min
{

template <typename T, template <typename> class vec>
class plane
{
  private:
    vec<T> _normal;
    T _constant;

  public:
    // Default plane == xz plane
    plane() : _normal(vec<T>::up()), _constant(0.0) {}
    plane(const vec<T>&, const vec<T>&);
    plane(const vec<T>&, const vec<T>&, const vec<T>&);

    inline vec<T> get_closest_point(const vec<T>&) const;
    inline T get_distance(const vec<T>&) const;
    inline const vec<T> &get_normal() const;
    inline vec<T> get_point(const vec<T>&, const T) const;
};
}

#endif
