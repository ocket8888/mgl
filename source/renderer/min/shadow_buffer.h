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
#ifndef __SHADOW_BUFFER__
#define __SHADOW_BUFFER__

#include "geom/min/frustum.h"
#include "math/min/mat4.h"
#include "platform/min/window.h"

#include "program.h"

namespace min
{

class shadow_buffer
{
  private:
    GLuint _id;
    GLuint _depth;
    size_t _width;
    size_t _height;
    mat4<float> _pv;
    mat4<float> _v;
    mat4<float> _scale_bias;
    mat4<float> _shadow;
    float _v_width;
    float _v_height;
    float _v_near;
    float _v_far;

    inline void check_extensions() const;
    inline void set_light_view(const vec3<float>&, const vec3<float>&, const vec3<float>&);

  public:
    shadow_buffer(const size_t, const size_t);
    shadow_buffer(const shadow_buffer&) = delete;
    ~shadow_buffer();

    inline void bind() const;
    inline void bind_default(const size_t, const size_t) const;
    inline void bind_shadow_texture(const size_t) const;
    inline const mat4<float> &get_pv_matrix() const;
    inline const mat4<float> &get_shadow_matrix() const;
    inline const mat4<float> &get_v_matrix() const;
    inline void set_light(const vec3<float>&, const vec3<float>&);
    inline void set_view_width(const float);
    inline void set_view_height(const float);
    inline void set_view_near(const float);
    inline void set_view_far(const float);
    inline void set_texture_uniform(const program&, const std::string&, const size_t) const;

};
}

#endif
