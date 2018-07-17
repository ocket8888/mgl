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
#ifndef UNIFORMBUFFER
#define UNIFORMBUFFER

#include <cstdint>
#include <cstring>
#include <limits>
#include <stdexcept>

#include "scene/min/light.h"
#include "math/min/mat4.h"
#include "math/min/vec4.h"
#include "platform/min/window.h"
#include "program.h"

namespace min
{

template <typename T>
class uniform_buffer
{
  private:
    // size of uint
    static constexpr size_t size_bytes = sizeof(int32_t);

    // size of light uniform buffer
    static constexpr size_t sizeof_light = sizeof(light<T>);

    // size of matrix uniform buffer
    static constexpr size_t sizeof_matrix = sizeof(mat4<T>);

    // size of matrix uniform buffer
    static constexpr size_t sizeof_vector = sizeof(vec4<T>);

  private:
    std::vector<light<T>> _lights;
    std::vector<mat4<T>> _matrix;
    std::vector<vec4<T>> _vector;
    GLuint _lbo;
    GLuint _mbo;
    GLuint _vbo;
    unsigned _max_lights;
    unsigned _max_matrix;
    unsigned _max_vector;
    GLint _light_offsets[2];
    GLint _matrix_offsets[2];
    GLint _vector_offsets[2];

    void check_extensions() const;
    size_t get_light_bytes() const;
    size_t get_matrix_bytes() const;
    size_t get_vector_bytes() const;
    void set_lights(const program&) const;
    void set_matrix(const program&) const;
    void set_vector(const program&) const;
    void update_light_buffer() const;
    void update_matrix_buffer() const;
    void update_vector_buffer() const;

  public:
    uniform_buffer();
    uniform_buffer(const unsigned, const unsigned, const unsigned);
    uniform_buffer(const uniform_buffer&) = delete;
    ~uniform_buffer();

    void defer_construct(const unsigned, const unsigned, const unsigned);
    void load_buffers();
    size_t add_light(const light<T>&);
    size_t add_matrix(const mat4<T>&);
    size_t add_vector(const vec4<T>&);
    void bind() const;
    void clear_lights();
    void clear_matrix();
    void clear_vector();
    static size_t get_max_buffer_size();
    void insert_light(const std::vector<light<T>>&);
    void insert_matrix(const std::vector<mat4<T>>&);
    void insert_vector(const std::vector<vec4<T>>&);
    size_t light_size() const;
    size_t matrix_size() const;
    size_t vector_size() const;
    void reserve_lights(const size_t);
    void reserve_matrix(const size_t);
    void reserve_vector(const size_t);
    void set_light(const light<T>&, const size_t);
    void set_matrix(const mat4<T>&, const size_t);
    void set_vector(const vec4<T>&, const size_t);
    void set_program_lights(const program&) const;
    void set_program_matrix(const program&) const;
    void set_program_vector(const program&) const;
    void update() const;
    void update_lights() const;
    void update_matrix() const;
    void update_vector() const;

};
}

#endif
