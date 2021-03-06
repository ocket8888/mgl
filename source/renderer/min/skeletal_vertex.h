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
#ifndef SKELETALVERTEX
#define SKELETALVERTEX

#include <cstring>

#include "geom/min/mesh.h"
#include "platform/min/window.h"

namespace min
{

template <typename T, typename K, GLenum FLOAT_TYPE>
class skeletal_vertex
{
  private:
    // Turn the Struct of Array (SoA) data into Array of Structs (AoS)

    // These are the struct member sizes
    static constexpr size_t vertex_size = sizeof(vec4<T>);
    static constexpr size_t uv_size = sizeof(vec2<T>);
    static constexpr size_t normal_size = sizeof(vec3<T>);
    static constexpr size_t tangent_size = sizeof(vec3<T>);
    static constexpr size_t bitangent_size = sizeof(vec3<T>);
    static constexpr size_t bone_index_size = sizeof(vec4<T>);
    static constexpr size_t bone_weight_size = sizeof(vec4<T>);

    // These are the struct member offsets in floats, not bytes
    static constexpr size_t uv_off = vertex_size / sizeof(T);
    static constexpr size_t normal_off = uv_off + (uv_size / sizeof(T));
    static constexpr size_t tangent_off = normal_off + (normal_size / sizeof(T));
    static constexpr size_t bitangent_off = tangent_off + (tangent_size / sizeof(T));
    static constexpr size_t bone_index_off = bitangent_off + (bitangent_size / sizeof(T));
    static constexpr size_t bone_weight_off = bone_index_off + (bone_index_size / sizeof(T));

    // Compute the size of struct in bytes
    static constexpr size_t width_bytes = vertex_size + uv_size + normal_size + tangent_size + bitangent_size + bone_index_size + bone_weight_size;

    // Compute the size of struct in floats
    static constexpr size_t width_size = width_bytes / sizeof(T);

  public:
    static void change_bind_buffer(const GLuint);
    static void create_vertex_attributes();
    static void create_buffer_binding(const GLuint, const GLuint);
    static void create(const GLuint);
    static void check(const mesh<T, K>&);
    static void copy(std::vector<T>&, const mesh<T, K>&, const size_t);
    static void destroy();
    static void disable_attributes();
    static void enable_attributes();
    static constexpr size_t width(){return width_size;}
    static constexpr GLenum buffer_type(){return GL_STATIC_DRAW;}

};
}

#endif
