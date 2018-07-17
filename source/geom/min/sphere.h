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
#ifndef SPHERE
#define SPHERE

#include <cmath>
#include <cstdio>
#include <utility>
#include <vector>

namespace min
{

template <typename T, template <typename> class vec>
class sphere
{
  private:
    vec<T> _center;
    T _radius;
    T _radius2;

    // Calculate an aabbox using the most separating axis
    // Center the sphere at this location
    void fit(const std::vector<vec<T>>&);

  public:
    sphere() : _radius(0.0), _radius2(0.0) {}
    sphere(const vec<T> &c, const T r) : _center(c), _radius(r), _radius2(r * r) {}
    sphere(const vec<T>&, const vec<T>&);
    sphere(const std::vector<vec<T>>&);

    void add(const std::vector<vec<T>>&);
    vec<T> closest_point(const vec<T>&) const;
    const vec<T> &get_center() const;
    vec<T> get_extent() const;
    vec<T> get_min() const;
    vec<T> get_max() const;
    T get_radius() const;
    T get_square_radius() const;
    std::vector<std::pair<vec<T>, T>> grid(size_t) const;
    vec<T> normal(const vec<T>&, T&, const T) const;
    bool point_inside(const vec<T>&) const;
    void set_position(const vec<T>&);
    T square_distance(const vec<T>&) const;
    T square_size() const;
    std::vector<std::pair<vec<T>, T>> subdivide() const;
};
}

#endif
