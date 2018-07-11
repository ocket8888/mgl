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

#include "static_vertex.h"


template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::change_bind_buffer(const GLuint vbo)
{
#ifdef MGL_VB43
    // No offset, standard stride, binding point 0
    glBindVertexBuffer(0, vbo, 0, width_bytes);
#else
    // Redundantly recreate the vertex attributes
    create_vertex_attributes();
#endif
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::create_vertex_attributes()
{
#ifdef MGL_VB43
    // Specify the vertex attributes in location = 0, no offset
    glVertexAttribFormat(0, 4, FLOAT_TYPE, GL_FALSE, 0);
    // Specify the uv attributes in location = 1, offset is in bytes
    glVertexAttribFormat(1, 2, FLOAT_TYPE, GL_FALSE, uv_off * sizeof(T));
    // Specify the normal attributes in location = 2, offset is in bytes
    glVertexAttribFormat(2, 3, FLOAT_TYPE, GL_FALSE, normal_off * sizeof(T));
    // Specify the tangent attributes in location = 3, offset is in bytes
    glVertexAttribFormat(3, 3, FLOAT_TYPE, GL_FALSE, tangent_off * sizeof(T));
    // Specify the bitangent attributes in location = 4, offset is in bytes
    glVertexAttribFormat(4, 3, FLOAT_TYPE, GL_FALSE, bitangent_off * sizeof(T));
#else
    // Specify the vertex attributes in location = 0, no offset
    glVertexAttribPointer(0, 4, FLOAT_TYPE, GL_FALSE, width_bytes, nullptr);
    // Specify the uv attributes in location = 1, offset is in bytes
    glVertexAttribPointer(1, 2, FLOAT_TYPE, GL_FALSE, width_bytes, (GLvoid *)(uv_off * sizeof(T)));
    // Specify the normal attributes in location = 2, offset is in bytes
    glVertexAttribPointer(2, 3, FLOAT_TYPE, GL_FALSE, width_bytes, (GLvoid *)(normal_off * sizeof(T)));
    // Specify the tangent attributes in location = 3, offset is in bytes
    glVertexAttribPointer(3, 3, FLOAT_TYPE, GL_FALSE, width_bytes, (GLvoid *)(tangent_off * sizeof(T)));
    // Specify the bitangent attributes in location = 4, offset is in bytes
    glVertexAttribPointer(4, 3, FLOAT_TYPE, GL_FALSE, width_bytes, (GLvoid *)(bitangent_off * sizeof(T)));
#endif
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::create_buffer_binding(const GLuint vbo, const GLuint bind_point)
{
#ifdef MGL_VB43
    //  Create the buffer binding point
    glVertexAttribBinding(0, bind_point);
    glVertexAttribBinding(1, bind_point);
    glVertexAttribBinding(2, bind_point);
    glVertexAttribBinding(3, bind_point);
    glVertexAttribBinding(4, bind_point);

    // No offset, standard stride, binding point 0
    glBindVertexBuffer(bind_point, vbo, 0, width_bytes);
#endif
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::create(const GLuint vbo)
{
    // Enable the attributes
    enable_attributes();

    // Create the vertex attributes
    create_vertex_attributes();

#ifdef MGL_VB43
    // Create the buffer binding point
    create_buffer_binding(vbo, 0);
#endif
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::check(const min::mesh<T, K> &m)
{
    // Verify normal, tangent and bitangent sizes
    const auto attr_size = m.vertex.size();
    if (m.uv.size() != attr_size || m.normal.size() != attr_size || m.tangent.size() != attr_size || m.bitangent.size() != attr_size)
    {
        throw std::runtime_error("static_vertex: uv, normals or tangents invalid length");
    }
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::copy(std::vector<T> &data, const min::mesh<T, K> &m, const size_t mesh_offset)
{
    const auto attr_size = m.vertex.size();
    for (size_t i = 0, j = mesh_offset; i < attr_size; i++, j += width_size)
    {
        // Copy the vertex data, 4 floats
        std::memcpy(&data[j], &m.vertex[i], vertex_size);

        // Copy the uv data, 2 floats, offset is in number of floats
        std::memcpy(&data[j + uv_off], &m.uv[i], uv_size);

        // Copy the normal data, 3 floats, offset is in number of floats
        std::memcpy(&data[j + normal_off], &m.normal[i], normal_size);

        // Copy the tangent data, 3 floats, offset is in number of floats
        std::memcpy(&data[j + tangent_off], &m.tangent[i], tangent_size);

        // Copy the bitangent data, 3 floats, offset is in number of floats
        std::memcpy(&data[j + bitangent_off], &m.bitangent[i], bitangent_size);
    }
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::destroy()
{
    // Disable the vertex attributes before destruction
    disable_attributes();
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::disable_attributes()
{
    // Disable the vertex attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline void min::static_vertex<T,K,FLOAT_TYPE>::enable_attributes()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline constexpr size_t min::static_vertex<T,K,FLOAT_TYPE>::width()
{
    return width_size;
}

template <typename T, typename K, GLenum FLOAT_TYPE>
inline constexpr GLenum min::static_vertex<T,K,FLOAT_TYPE>::buffer_type()
{
    return GL_STATIC_DRAW;
}
