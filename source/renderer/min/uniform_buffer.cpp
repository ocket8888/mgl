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

#include "uniform_buffer.h"

template<typename T>
inline void min::uniform_buffer<T>::check_extensions() const
{
    const bool ubo = GLEW_ARB_uniform_buffer_object;

    // Check that we have the extensions we need
    if (!ubo)
    {
        throw std::runtime_error("uniform_buffer: minimum extensions not met");
    }
}

template<typename T>
inline size_t min::uniform_buffer<T>::get_light_bytes() const
{
    return _max_lights * sizeof_light + size_bytes;
}

template<typename T>
inline size_t min::uniform_buffer<T>::get_matrix_bytes() const
{
    return _max_matrix * sizeof_matrix + size_bytes;
}

template<typename T>
inline size_t min::uniform_buffer<T>::get_vector_bytes() const
{
    return _max_vector * sizeof_vector + size_bytes;
}

template<typename T>
void min::uniform_buffer<T>::set_lights(const min::program &p) const
{
    if (_max_lights > 0)
    {
        // Calculate the light block indices, 4-N byte alignment
        const GLchar *names[2] = {
            "lights[0].color",
            "light_size"};

        // Array to store the indices
        GLuint indices[2];

        // Get the indices from the program
        glGetUniformIndices(p.id(), 2, names, indices);

        // Check that the indices are valid
        if (indices[0] == GL_INVALID_INDEX || indices[1] == GL_INVALID_INDEX)
        {
            throw std::runtime_error("uniform_buffer: light uniform indices are invalid");
        }

        // Get the member offsets from the program
        GLint light_offsets[2];
        glGetActiveUniformsiv(p.id(), 2, indices, GL_UNIFORM_OFFSET, light_offsets);

        // Check the array offset, offset MUST BE ZERO
        if (light_offsets[0] != _light_offsets[0])
        {
            throw std::runtime_error("uniform_buffer: light_block.lights offset calculated by opengl is not std140");
        }

        // Check the size offset
        if (light_offsets[1] != _light_offsets[1])
        {
            throw std::runtime_error("uniform_buffer: light_block.size offset calculated by opengl is not std140");
        }

        const GLuint light_index = glGetUniformBlockIndex(p.id(), "light_block");

        // Set the block binding index = 0
        glUniformBlockBinding(p.id(), light_index, 0);
    }
}

template<typename T>
void min::uniform_buffer<T>::set_matrix(const min::program &p) const
{
    if (_max_matrix > 0)
    {
        // Calculate the matrix block indices, 4-N byte alignment
        const GLchar *names[2] = {
            "matrix[0]",
            "matrix_size"};

        // Array to store the indices
        GLuint indices[2];
        GLint arr_strides[2];
        GLint matrix_strides[2];

        // Get the indices from the program
        glGetUniformIndices(p.id(), 2, names, indices);

        // Check that the indices are valid
        if (indices[0] == GL_INVALID_INDEX || indices[1] == GL_INVALID_INDEX)
        {
            throw std::runtime_error("uniform_buffer: matrix uniform indices are invalid");
        }

        // Get the member offsets from the program
        GLint matrix_offsets[2];
        glGetActiveUniformsiv(p.id(), 2, indices, GL_UNIFORM_OFFSET, matrix_offsets);

        // Get the matrix array stride from the program
        glGetActiveUniformsiv(p.id(), 2, indices, GL_UNIFORM_ARRAY_STRIDE, arr_strides);

        // Get the matrix stride from the program
        glGetActiveUniformsiv(p.id(), 2, indices, GL_UNIFORM_MATRIX_STRIDE, matrix_strides);

        // Check the array offset, offset MUST BE ZERO
        if (matrix_offsets[0] != _matrix_offsets[0])
        {
            throw std::runtime_error("uniform_buffer: matrix_block.matrix offset calculated by opengl is not std140");
        }

        // Check the size offset
        if (matrix_offsets[1] != _matrix_offsets[1])
        {
            throw std::runtime_error("uniform_buffer: matrix_block.size offset calculated by opengl is not std140");
        }

        // Check the array stride, stride MUST BE 64 for float 128 for double
        if (arr_strides[0] != sizeof(mat4<T>))
        {
            throw std::runtime_error("uniform_buffer: matrix_block.matrix array stride calculated by opengl is not std140");
        }

        // Check the matrix stride, stride MUST BE 16 for float 32 for double
        if (matrix_strides[0] != sizeof(vec4<T>))
        {
            throw std::runtime_error("uniform_buffer: matrix_block.matrix matrix stride calculated by opengl is not std140");
        }

        const GLuint matrix_index = glGetUniformBlockIndex(p.id(), "matrix_block");

        // Set the block binding index = 1
        glUniformBlockBinding(p.id(), matrix_index, 1);
    }
}

template<typename T>
void min::uniform_buffer<T>::set_vector(const min::program &p) const
{
    if (_max_vector > 0)
    {
        // Calculate the vector block indices, 4-N byte alignment
        const GLchar *names[2] = {
            "vector[0]",
            "vector_size"};

        // Array to store the indices
        GLuint indices[2];

        // Get the indices from the program
        glGetUniformIndices(p.id(), 2, names, indices);

        // Check that the indices are valid
        if (indices[0] == GL_INVALID_INDEX || indices[1] == GL_INVALID_INDEX)
        {
            throw std::runtime_error("uniform_buffer: vector uniform indices are invalid");
        }

        // Get the member offsets from the program
        GLint vector_offsets[2];
        glGetActiveUniformsiv(p.id(), 2, indices, GL_UNIFORM_OFFSET, vector_offsets);

        // Check the array offset, offset MUST BE ZERO
        if (vector_offsets[0] != _vector_offsets[0])
        {
            throw std::runtime_error("uniform_buffer: vector_block.vector offset calculated by opengl is not std140");
        }

        // Check the size offset
        if (vector_offsets[1] != _vector_offsets[1])
        {
            throw std::runtime_error("uniform_buffer: vector_block.size offset calculated by opengl is not std140");
        }

        const GLuint vector_index = glGetUniformBlockIndex(p.id(), "vector_block");

        // Set the block binding index = 0
        glUniformBlockBinding(p.id(), vector_index, 2);
    }
}

template<typename T>
void min::uniform_buffer<T>::update_light_buffer() const
{
    // Check for empty matrix array and return
    if (_lights.size() == 0)
    {
        return;
    }
    else if (_lights.size() > _max_lights)
    {
        // Check if the buffer is overfilled
        throw std::runtime_error("uniform_buffer: light buffer contains more lights than allowed");
    }

    // The size of the maximum light count and the size
    // (3 * vec4 = 48) * N + (1 * int = 4)
    const size_t light_bytes = get_light_bytes();
    std::vector<uint8_t> data(light_bytes, 0);

    // Add the lights to the light uniform array
    // We can use one copy here because the array is tightly packed
    const size_t copy_bytes = _lights.size() * sizeof_light;
    std::memcpy(&data[_light_offsets[0]], &_lights[0], copy_bytes);

    // GLSL likes to use int vs uint in older drivers
    // We must check for overflow
    if (_lights.size() > std::numeric_limits<int32_t>::max())
    {
        throw std::runtime_error("uniform_buffer: light integer overflow");
    }

    // Set the size property
    const int32_t size = static_cast<int32_t>(_lights.size());
    std::memcpy(&data[_light_offsets[1]], &size, size_bytes);

    // Bind buffer and copy data into it
    glBindBuffer(GL_UNIFORM_BUFFER, _lbo);

    // Send the data to the GPU with light_bytes calculated
    glBufferData(GL_UNIFORM_BUFFER, light_bytes, &data[0], GL_DYNAMIC_DRAW);
}

template<typename T>
void min::uniform_buffer<T>::update_matrix_buffer() const
{
    // Check for empty matrix array and return
    if (_matrix.size() == 0)
    {
        return;
    }
    else if (_matrix.size() > _max_matrix)
    {
        // Check if the buffer is overfilled
        throw std::runtime_error("uniform_buffer: matrix buffer contains more matrices than allowed");
    }

    // The size of the maximum matrix count and the size
    // (4 * vec4 = 64) * N + (1 * int = 4)
    const size_t matrix_bytes = get_matrix_bytes();
    std::vector<uint8_t> data(matrix_bytes, 0);

    // Add the matrices to the matrix uniform array
    // We can use one copy here because the array is tightly packed
    const size_t copy_bytes = _matrix.size() * sizeof_matrix;
    std::memcpy(&data[_matrix_offsets[0]], &_matrix[0], copy_bytes);

    // GLSL likes to use int vs uint in older drivers
    // We must check for overflow
    if (_matrix.size() > std::numeric_limits<int32_t>::max())
    {
        throw std::runtime_error("uniform_buffer: matrix integer overflow");
    }

    // Set the size property
    const int32_t size = static_cast<int32_t>(_matrix.size());
    std::memcpy(&data[_matrix_offsets[1]], &size, size_bytes);

    // Bind buffer and copy data into it
    glBindBuffer(GL_UNIFORM_BUFFER, _mbo);

    // Send the data to the GPU with matrix_bytes calculated
    glBufferData(GL_UNIFORM_BUFFER, matrix_bytes, &data[0], GL_DYNAMIC_DRAW);
}

template<typename T>
void min::uniform_buffer<T>::update_vector_buffer() const
{
    // Check for empty vector array and return
    if (_vector.size() == 0)
    {
        return;
    }
    else if (_vector.size() > _max_vector)
    {
        // Check if the buffer is overfilled
        throw std::runtime_error("uniform_buffer: vector buffer contains more vectors than allowed");
    }

    // The size of the maximum vector count and the size
    // (vec4 = 16) * N + (1 * int = 4)
    const size_t vector_bytes = get_vector_bytes();
    std::vector<uint8_t> data(vector_bytes, 0);

    // Add the vectors to the vector uniform array
    // We can use one copy here because the array is tightly packed
    const size_t copy_bytes = _vector.size() * sizeof_vector;
    std::memcpy(&data[_vector_offsets[0]], &_vector[0], copy_bytes);

    // GLSL likes to use int vs uint in older drivers
    // We must check for overflow
    if (_vector.size() > std::numeric_limits<int32_t>::max())
    {
        throw std::runtime_error("uniform_buffer: vector integer overflow");
    }

    // Set the size property
    const int32_t size = static_cast<int32_t>(_vector.size());
    std::memcpy(&data[_vector_offsets[1]], &size, size_bytes);

    // Bind buffer and copy data into it
    glBindBuffer(GL_UNIFORM_BUFFER, _vbo);

    // Send the data to the GPU with vector_bytes calculated
    glBufferData(GL_UNIFORM_BUFFER, vector_bytes, &data[0], GL_DYNAMIC_DRAW);
}

template<typename T>
min::uniform_buffer<T>::uniform_buffer()
    : _max_lights(0), _max_matrix(0), _max_vector(0),
      _light_offsets{0, 0}, _matrix_offsets{0, 0}, _vector_offsets{0, 0}
{
    // Check that all needed extensions are present
    check_extensions();
}

template<typename T>
min::uniform_buffer<T>::uniform_buffer(const unsigned light_size, const unsigned matrix_size, const unsigned vector_size)
    : _max_lights(light_size), _max_matrix(matrix_size), _max_vector(vector_size),
      _light_offsets{0, static_cast<GLint>(get_light_bytes() - size_bytes)},
      _matrix_offsets{0, static_cast<GLint>(get_matrix_bytes() - size_bytes)},
      _vector_offsets{0, static_cast<GLint>(get_vector_bytes() - size_bytes)}
{
    // Check that all needed extensions are present
    check_extensions();

    // Load light and matrix buffers
    load_buffers();
}

template<typename T>
void min::uniform_buffer<T>::defer_construct(const unsigned light_size, const unsigned matrix_size, const unsigned vector_size)
{
    _max_lights = light_size;
    _max_matrix = matrix_size;
    _max_vector = vector_size;

    // Set the uniform offsets
    _light_offsets[0] = 0;
    _light_offsets[1] = static_cast<GLint>(get_light_bytes() - size_bytes);
    _matrix_offsets[0] = 0;
    _matrix_offsets[1] = static_cast<GLint>(get_matrix_bytes() - size_bytes);
    _vector_offsets[0] = 0;
    _vector_offsets[1] = static_cast<GLint>(get_vector_bytes() - size_bytes);

    // Load light and matrix buffers
    load_buffers();
}

template<typename T>
void min::uniform_buffer<T>::load_buffers()
{
    const size_t max_size = get_max_buffer_size();
    if (max_size < get_light_bytes() || max_size < get_matrix_bytes() || max_size < get_vector_bytes())
    {
        throw std::runtime_error("uniform_buffer: max uniform buffer size is too small");
    }

    // Create lights
    if (_max_lights > 0)
    {
        // create light uniform buffer
        glGenBuffers(1, &_lbo);
    }

    // Create matrix
    if (_max_matrix > 0)
    {
        // create matrix uniform buffer
        glGenBuffers(1, &_mbo);
    }

    // Create vector
    if (_max_vector > 0)
    {
        // create vector uniform buffer
        glGenBuffers(1, &_vbo);
    }
}

template<typename T>
min::uniform_buffer<T>::~uniform_buffer()
{
    // Unbind the uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Delete lights
    if (_max_lights > 0)
    {
        glDeleteBuffers(1, &_lbo);
    }

    // Delete matrix
    if (_max_matrix > 0)
    {
        glDeleteBuffers(1, &_mbo);
    }

    // Delete vector
    if (_max_vector > 0)
    {
        glDeleteBuffers(1, &_vbo);
    }

    // Check for opengl errors
    throw_gl_error();
}

template<typename T>
inline size_t min::uniform_buffer<T>::add_light(const min::light<T> &light)
{
    _lights.push_back(light);

    // Return light ID
    return _lights.size() - 1;
}

template<typename T>
inline size_t min::uniform_buffer<T>::add_matrix(const min::mat4<T> &mat)
{
    _matrix.push_back(mat);

    // Return matrix ID
    return _matrix.size() - 1;
}

template<typename T>
inline size_t min::uniform_buffer<T>::add_vector(const min::vec4<T> &v)
{
    _vector.push_back(v);

    // Return vector ID
    return _vector.size() - 1;
}

template<typename T>
inline void min::uniform_buffer<T>::bind() const
{
    // Bind lights
    if (_max_lights > 0)
    {
        // Connect the binding index = 0 to the buffer
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, _lbo);
    }

    // Bind matrix
    if (_max_matrix > 0)
    {
        // Connect the binding index = 1 to the buffer
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, _mbo);
    }

    // Bind vector
    if (_max_vector > 0)
    {
        // Connect the binding index = 2 to the buffer
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, _vbo);
    }
}

template<typename T>
inline void min::uniform_buffer<T>::clear_lights()
{
    _lights.clear();
}

template<typename T>
inline void min::uniform_buffer<T>::clear_matrix()
{
    _matrix.clear();
}

template<typename T>
inline void min::uniform_buffer<T>::clear_vector()
{
    _vector.clear();
}

template<typename T>
inline size_t min::uniform_buffer<T>::get_max_buffer_size()
{
    // Get the maximum uniform buffer size
    GLint size;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &size);

    return (size_t)size;
}

template<typename T>
inline void min::uniform_buffer<T>::insert_light(const std::vector<min::light<T>> &v)
{
    _lights.insert(_lights.end(), v.begin(), v.end());
}

template<typename T>
inline void min::uniform_buffer<T>::insert_matrix(const std::vector<min::mat4<T>> &v)
{
    _matrix.insert(_matrix.end(), v.begin(), v.end());
}

template<typename T>
inline void min::uniform_buffer<T>::insert_vector(const std::vector<min::vec4<T>> &v)
{
    _vector.insert(_vector.end(), v.begin(), v.end());
}

template<typename T>
inline size_t min::uniform_buffer<T>::light_size() const
{
    return _lights.size();
}

template<typename T>
inline size_t min::uniform_buffer<T>::matrix_size() const
{
    return _matrix.size();
}

template<typename T>
inline size_t min::uniform_buffer<T>::vector_size() const
{
    return _vector.size();
}

template<typename T>
inline void min::uniform_buffer<T>::reserve_lights(const size_t size)
{
    _lights.reserve(size);
}

template<typename T>
inline void min::uniform_buffer<T>::reserve_matrix(const size_t size)
{
    _matrix.reserve(size);
}

template<typename T>
inline void min::uniform_buffer<T>::reserve_vector(const size_t size)
{
    _vector.reserve(size);
}

template<typename T>
inline void min::uniform_buffer<T>::set_light(const min::light<T> &light, const size_t id)
{
    _lights[id] = light;
}

template<typename T>
inline void min::uniform_buffer<T>::set_matrix(const min::mat4<T> &mat, const size_t id)
{
    _matrix[id] = mat;
}

template<typename T>
inline void min::uniform_buffer<T>::set_vector(const min::vec4<T> &v, const size_t id)
{
    _vector[id] = v;
}

template<typename T>
inline void min::uniform_buffer<T>::set_program_lights(const min::program &p) const
{
    // set the light buffer
    set_lights(p);
}

template<typename T>
inline void min::uniform_buffer<T>::set_program_matrix(const min::program &p) const
{
    // set the matrix buffer
    set_matrix(p);
}

template<typename T>
inline void min::uniform_buffer<T>::set_program_vector(const min::program &p) const
{
    // set the vector buffer
    set_vector(p);
}

template<typename T>
inline void min::uniform_buffer<T>::update() const
{
    // Update the light buffer
    update_light_buffer();

    // Update the matrix buffer
    update_matrix_buffer();

    // Update the matrix buffer
    update_vector_buffer();
}

template<typename T>
inline void min::uniform_buffer<T>::update_lights() const
{
    // Update the light buffer
    update_light_buffer();
}

template<typename T>
inline void min::uniform_buffer<T>::update_matrix() const
{
    // Update the light buffer
    update_matrix_buffer();
}

template<typename T>
inline void min::uniform_buffer<T>::update_vector() const
{
    // Update the light buffer
    update_vector_buffer();
}
