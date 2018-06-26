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
#ifndef __FRUSTUM__
#define __FRUSTUM__

#include <cmath>

#include "math/min/mat4.h"
#include "math/min/vec3.h"

#include "plane.h"

namespace min
{

template <class T>
class frustum
{
  private:
    T _fov; // Screen dimensions
    T _ratio;
    T _zoom;
    vec3<T> _near; // Z planes
    vec3<T> _far;
    vec3<T> _center; // Orientation
    vec3<T> _right;
    mat4<T> _proj; // Matricies
    mat4<T> _view;
    plane<T, vec3> _plane[6]; // Planes
    bool _dirty;              // Needs updating

    // If the plane is facing in the negative direction then the excluding corner
    // is the maximum corner in the plane normal direction else use the minimum corner
    inline bool not_between_plane(const vec3<T>&, const vec3<T> &max, const int) const;
    inline bool outside_plane(const vec3<T>&, const int, const T) const;
    inline void orthographic_frustum();
    inline void perspective_frustum();
    inline void update();

  public:
    frustum() : _fov(45.0), _ratio(1.33), _zoom(1.0), _near(0.0, 0.0, 0.1), _far(0.0, 0.0, 200.0), _dirty(true) {}
    frustum(const T ratio, const T fov, const T near, const T far) : _fov(fov), _ratio(ratio), _zoom(1.0), _near(0.0, 0.0, near), _far(0.0, 0.0, far), _dirty(true) {}

    inline bool between(const vec3<T>&, const vec3<T>&) const;
    inline vec3<T> closest_point(const vec3<T>&) const;
    inline const vec3<T> &get_center() const;
    inline const vec3<T> &get_right() const;
    const mat4<T> &orthographic();
    const mat4<T> &perspective();
    const mat4<T> &look_at(const vec3<T>&, const vec3<T>&, vec3<T>&);
    inline void make_dirty();
    inline bool point_inside(const vec3<T>&) const;
    inline bool point_within(const vec3<T>&, const T) const;
    inline void set_aspect_ratio(const T, const T);
    inline void set_fov(const T);
    inline void set_near(const T);
    inline void set_far(const T);
    inline void zoom(const T);
};
}

#endif
