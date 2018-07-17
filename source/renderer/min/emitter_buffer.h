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
#ifndef EMITTERBUFFER
#define EMITTERBUFFER

#include <cmath>
#include <random>
#include <stdexcept>
#include <utility>

#include "math/min/vec3.h"
#include "platform/min/window.h"

// The particle system is governed by the combination of the following equations

// Equation 1: Newton's 2nd Law of Motion
// SUM(Forces) = mass * acceleration

// Equation 2: Acceleration, velocity, distance
// vo = vi + a * dt;
// do = di + vo * dt;

namespace min
{

template <typename T, GLenum FLOAT_TYPE>
class emitter_buffer
{
  private:
    // This is the size of each particle
    static constexpr size_t particle_size = sizeof(vec3<T>);

    GLuint _vao;
    GLuint _vbo;
    size_t _emit_count;
    size_t _emit_pool_offset;
    size_t _emit_pool_size;
    T _emit_freq;
    T _emit_accum;
    T _spawn_freq;
    T _spawn_accum;
    T _inv_mass;
    T _random;
    vec3<T> _rot_axis;
    vec3<T> _grav_force;
    vec3<T> _start_pos;
    vec3<T> _start_speed;
    vec3<T> _wind_force;
    std::uniform_real_distribution<T> _dist;
    std::mt19937 _rand;
    std::vector<vec3<T>> _position;
    std::vector<vec3<T>> _speed;
    std::vector<std::pair<vec3<T>, T>> _attractors;

    void accumulate(const T);
    void check_extensions() const;
    vec3<T> compute_force(const vec3<T>&, const vec3<T>&);
    void seed(const size_t, const size_t);

  public:
    emitter_buffer(const vec3<T>&, const size_t, const size_t, const T, const T, const T);
    ~emitter_buffer();
    emitter_buffer(const emitter_buffer&) = delete;

    size_t attractor_add(const vec3<T>&, const T);
    void attractor_clear();
    void set_attractor(const vec3<T>&, const T, const size_t);
    void bind() const;
    void unbind() const;
    void draw() const;
    T get_accum() const;
    const vec3<T> &get_position() const;
    vec3<T> random();
    void reset();
    void reset_accum();
    void resize(const size_t);
    void set_mass(const T);
    void set_gravity(const vec3<T>&);
    void set_position(const vec3<T>&);
    void set_random(const T, const T);
    void set_rotation_axis(const min::vec3<T>&);
    void set_speed(const vec3<T>&);
    void set_wind(const vec3<T>&);
    void step(const T);
    template <typename F> void set(const F&, const T);
    void upload() const;
};
}

#endif
