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
#ifndef __STATICVERTEX__
#define __STATICVERTEX__

#include <cstring>

#include "geom/min/mesh.h"
#include "platform/min/window.h"

namespace min
{

template <typename T, typename K, GLenum FLOAT_TYPE>
class static_vertex
{
  private:
    // Turn the Struct of Array (SoA) data into Array of Structs (AoS)

    // These are the struct member sizes
    static constexpr size_t vertex_size = sizeof(vec4<T>);
    static constexpr size_t uv_size = sizeof(vec2<T>);
    static constexpr size_t normal_size = sizeof(vec3<T>);
    static constexpr size_t tangent_size = sizeof(vec3<T>);
    static constexpr size_t bitangent_size = sizeof(vec3<T>);

    // These are the struct member offsets in floats, not bytes
    static constexpr size_t uv_off = vertex_size / sizeof(T);
    static constexpr size_t normal_off = uv_off + (uv_size / sizeof(T));
    static constexpr size_t tangent_off = normal_off + (normal_size / sizeof(T));
    static constexpr size_t bitangent_off = tangent_off + (tangent_size / sizeof(T));

    // Compute the size of struct in bytes
    static constexpr size_t width_bytes = vertex_size + uv_size + normal_size + tangent_size + bitangent_size;

    // Compute the size of struct in floats
    static constexpr size_t width_size = width_bytes / sizeof(T);

  public:
    inline static void change_bind_buffer(const GLuint);
    inline static void create_vertex_attributes();
    inline static void create_buffer_binding(const GLuint, const GLuint);
    inline static void create(const GLuint);
    inline static void check(const mesh<T, K>&);
    inline static void copy(std::vector<T>&, const mesh<T, K>&, const size_t);
    inline static void destroy();
    inline static void disable_attributes();
    inline static void enable_attributes();
    inline static constexpr size_t width();
    inline static constexpr GLenum buffer_type();

};
}

#endif
