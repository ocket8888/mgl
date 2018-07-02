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
#ifndef __ARRAY_BUFFER__
#define __ARRAY_BUFFER__

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "geom/min/mesh.h"
#include "platform/min/window.h"

namespace min
{

template <typename T, typename K, template <typename, typename, GLenum> class vertex_type, GLenum FLOAT_TYPE>
class array_buffer
{
  private:
    std::vector<std::vector<T>> _data;
    std::vector<std::vector<std::pair<size_t, size_t>>> _data_index;
    GLuint _vao;
    std::vector<GLuint> _vbo;
    mutable size_t _index;

    inline void check_extensions() const;
    void push_back_mesh(const mesh<T, K>&);
    void upload_all() const;

  public:
    array_buffer(const size_t = 1);
    array_buffer(const array_buffer&) = delete;
    ~array_buffer();

    size_t add_mesh(const mesh<T, K>&);
    inline void bind() const;
    inline void unbind() const;
    inline void bind_buffer(const size_t) const;
    inline void clear();
    inline void draw(const GLenum, const size_t) const;
    inline void draw(const GLenum, const size_t, const size_t) const;
    inline void draw_all(const GLenum) const;
    inline void draw_many(const GLenum, const size_t, const size_t) const;
    inline void reserve(const size_t, const size_t);
    inline void set_buffer(const size_t) const;
    void upload() const;
};
}

#endif
