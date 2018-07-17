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
#ifndef VERTEXBUFFER
#define VERTEXBUFFER

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "geom/min/mesh.h"
#include "platform/min/window.h"

namespace min
{

template <typename T, typename K, template <typename, typename, GLenum> class vertex_type, GLenum FLOAT_TYPE, GLenum INT_TYPE>
class vertex_buffer
{
  private:
    mutable std::vector<size_t> _data_bytes;
    mutable std::vector<size_t> _element_bytes;
    std::vector<std::vector<T>> _data;
    std::vector<std::vector<K>> _element;
    std::vector<std::vector<std::pair<size_t, size_t>>> _data_index;
    std::vector<std::vector<std::pair<size_t, size_t>>> _element_index;
    GLuint _vao;
    std::vector<GLuint> _vbo;
    std::vector<GLuint> _ibo;
    mutable size_t _index;

    void check_extensions() const;
    void push_back_mesh(const mesh<T, K>&);
    void swap_mesh(const mesh<T, K>&, const size_t);
    bool reallocate_buffer() const;
    void upload_all() const;
    void upload(const size_t) const;
    void upload_last() const;

  public:
    vertex_buffer(const size_t= 1);
    ~vertex_buffer();
    vertex_buffer(const vertex_buffer &sb) = delete;

    size_t add_mesh(const mesh<T, K>&);
    void bind() const;
    void unbind() const;
    void bind_buffer(const size_t) const;
    void clear();
    void draw(const GLenum, const size_t) const;
    void draw(const GLenum, const size_t, const size_t) const;
    void draw_all(const GLenum) const;
    void draw_all_after(const GLenum, const size_t) const;
    void draw_many(const GLenum, const size_t, const size_t) const;
    void reserve(const size_t, const size_t, const size_t);
    void set_buffer(const size_t) const;
    void upload() const;
    void upload_mesh(const mesh<T, K>&);
    void upload_mesh(const mesh<T, K>&, const size_t);
};
}

#endif
