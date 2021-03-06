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
#ifndef CAMERA
#define CAMERA

#include <cmath>

#include "geom/min/frustum.h"
#include "math/min/mat4.h"
#include "math/min/quat.h"
#include "math/min/vec3.h"

namespace min
{

template <typename T>
class camera
{
  private:
    vec3<T> _p;
    vec3<T> _look;
    vec3<T> _forward;
    vec3<T> _up;
    frustum<T> _f;
    mat4<T> _pv;
    mat4<T> _v;
    bool _dirty;
    bool _proj_ortho;

    void update();

  public:
    camera();

    void force_update();
    const vec3<T> &get_forward() const;
    const vec3<T> &get_right() const;
    const vec3<T> &get_up() const;
    frustum<T> &get_frustum();
    const frustum<T> &get_frustum() const;
    const vec3<T> &get_look_at() const;
    const mat4<T> &get_pv_matrix();
    const mat4<T> &get_v_matrix();
    const vec3<T> &get_position() const;
    void make_dirty();
    quat<T> move_look_at(const T, const T);
    vec3<T> project_point(const T s) const;
    void set_look_at(const vec3<T>&);
    void set_orthographic();
    void set_perspective();
    void set_position(const vec3<T>&);
    void set(const vec3<T>&, const vec3<T>&);
    void set(const vec3<T>&, const vec3<T>&, const vec3<T>&);

};
}

#endif
