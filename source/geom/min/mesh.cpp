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

#include "mesh.h"

template <typename T, typename K>
inline void min::mesh<T,K>::calculate_normal(const size_t a, const size_t b, const size_t c)
{
    // Get references to triangle vecs
    const K ia = index[a];
    const K ib = index[b];
    const K ic = index[c];

    // Get vertices
    const vec3<T> &v0 = vertex[ia];
    const vec3<T> &v1 = vertex[ib];
    const vec3<T> &v2 = vertex[ic];

    // Calculate normal on triangle, these triangles are drawn CW, negate for CCW
    const vec3<T> n = vec3<T>::normal(v0, v1, v2);

    // Set the normals for each index
    normal[ia] += n;
    normal[ib] += n;
    normal[ic] += n;
}

template <typename T, typename K>
inline void min::mesh<T,K>::calculate_tangent(const size_t a, const size_t b, const size_t c)
{
    // Get references to triangle vecs
    const K ia = index[a];
    const K ib = index[b];
    const K ic = index[c];

    // Get vertices
    const vec4<T> &v0 = vertex[ia];
    const vec4<T> &v1 = vertex[ib];
    const vec4<T> &v2 = vertex[ic];

    // Get uvs
    const vec2<T> &uv0 = uv[ia];
    const vec2<T> &uv1 = uv[ib];
    const vec2<T> &uv2 = uv[ic];

    // Calculate delta vertices; dv1 x dv2 = normal
    const vec3<T> dv1 = v1 - v0;
    const vec3<T> dv2 = v2 - v0;
    const vec2<T> duv1 = uv1 - uv0;
    const vec2<T> duv2 = uv2 - uv0;

    // Calculate the determinant
    const T det = 1.0 / (duv1.x() * duv2.y() - duv1.y() * duv2.x());

    // Solve inverse matrix equations and calculate tangents
    const vec3<T> tan = (dv1 * duv2.y() - dv2 * duv1.y()) * det;
    const vec3<T> bit = (dv2 * duv1.x() - dv1 * duv2.x()) * det;

    add_tangents(std::make_pair(tan, bit), ia);
    add_tangents(std::make_pair(tan, bit), ib);
    add_tangents(std::make_pair(tan, bit), ic);
}

template <typename T, typename K>
inline void min::mesh<T,K>::add_tangents(const std::pair<min::vec3<T>, min::vec3<T>> &tan, const size_t in)
{
    // Get the normal for this vertex
    const vec3<T> &norm = normal[in];

    // orthogonalize the tangent to this normal
    vec3<T> otan = (tan.first - (norm * norm.dot(tan.first))).normalize();

    // Normalize the bitangents
    vec3<T> btan = tan.second;
    btan.normalize();

    // Check if the (normal x bitan) must be same orientation as tangent
    // If it is a negative angle, correct it
    if (otan.cross(norm).dot(btan) < 0.0)
    {
        otan *= -1.0;
    }

    // Add the calculated tangents to the mesh
    tangent[in] += otan;
    bitangent[in] += btan;
}

template <typename T, typename K>
void min::mesh<T,K>::calculate_normals()
{
    // Get the vertex dimension and verify not zero
    const size_t size = index.size();
    const size_t vert_size = vertex.size();

    // Check that the attribute dimensions of mesh vertices is correct
    if (size == 0 || vert_size == 0)
    {
        throw std::runtime_error("mesh.calculate_normals(): vertex buffer not valid size, invalid state");
    }

    // Verify the indices are a multiple of 3
    if (size % 3 != 0)
    {
        throw std::runtime_error("mesh.calculate_normals(): indices are not a multiple of three");
    }

    // If the model has normals skip
    if (normal.size() != vert_size)
    {
        // Clear out old normal buffer and reserve space
        normal.clear();
        normal.resize(vert_size);

        // Calculate normal for each triangle
        for (size_t j = 0; j < size; j += 3)
        {
            // Calculate normal on the plane
            calculate_normal(j, j + 1, j + 2);
        }

        // Normalize all normals
        for (auto &n : normal)
        {
            n.normalize();
        }

        // Check that the normals are same size as vertices
        if (normal.size() != vert_size)
        {
            throw std::runtime_error("mesh.calculate_normals(): failed calculating normals");
        }
    }
}

template <typename T, typename K>
void min::mesh<T,K>::calculate_tangents()
{
    // Get the vertex dimension and verify normals and uvs are the same length
    const size_t size = index.size();
    const size_t vert_size = vertex.size();

    // Check that the attribute dimensions of mesh is correct
    if (size == 0 || uv.size() != vert_size || normal.size() != vert_size || vert_size == 0)
    {
        throw std::runtime_error("mesh.calculate_tangents(): attribute buffers not valid size, invalid state");
    }

    // Verify the indices are a multiple of 3
    if (size % 3 != 0)
    {
        throw std::runtime_error("mesh.calculate_tangents(): indices are not a multiple of three");
    }

    // If the model has normals skip
    if (tangent.size() != vert_size || bitangent.size() != vert_size)
    {
        // Clear out old tangent and bitangent buffers and reserve space
        tangent.clear();
        bitangent.clear();
        tangent.resize(vert_size);
        bitangent.resize(vert_size);

        // Calculate tangents for each triangle
        for (size_t j = 0; j < size; j += 3)
        {
            // Calculate tangents (tan, bit)
            calculate_tangent(j, j + 1, j + 2);
        }

        // Normalize all tangents
        for (auto &t : tangent)
        {
            t.normalize();
        }

        // Normalize all bitangents
        for (auto &b : bitangent)
        {
            b.normalize();
        }

        // Check that the normals are same size as vertices
        if (tangent.size() != vert_size || bitangent.size() != vert_size)
        {
            throw std::runtime_error("mesh.calculate_tangents(): failed calculating normals");
        }
    }
}

template <typename T, typename K>
void min::mesh<T,K>::clear()
{
    // Clear all members
    vertex.clear();
    uv.clear();
    normal.clear();
    tangent.clear();
    bitangent.clear();
    index.clear();
    bone_index.clear();
    bone_weight.clear();
}

template <typename T, typename K>
void min::mesh<T,K>::flip_uv_x_axis()
{
    for (auto &a : uv)
    {
        a.x(1.0 - a.x());
    }
}

template <typename T, typename K>
void min::mesh<T,K>::flip_uv_y_axis()
{
    for (auto &a : uv)
    {
        a.y(1.0 - a.y());
    }
}

template <typename T, typename K>
const std::string &min::mesh<T,K>::get_name() const
{
    return _name;
}

template <typename T, typename K>
void min::mesh<T,K>::merge(const min::mesh<T, K> &m)
{
    // Find the index shift amount
    const size_t index_shift = vertex.size();

    // Preallocate, may be unnecessary
    vertex.reserve(vertex.size() + m.vertex.size());
    uv.reserve(uv.size() + m.uv.size());
    normal.reserve(normal.size() + m.normal.size());
    tangent.reserve(tangent.size() + m.tangent.size());
    bitangent.reserve(bitangent.size() + m.bitangent.size());
    index.reserve(index.size() + m.index.size());
    bone_index.reserve(bone_index.size() + m.bone_index.size());
    bone_weight.reserve(bone_weight.size() + m.bone_weight.size());

    // Insert m into this mesh
    vertex.insert(vertex.end(), m.vertex.begin(), m.vertex.end());
    uv.insert(uv.end(), m.uv.begin(), m.uv.end());
    normal.insert(normal.end(), m.normal.begin(), m.normal.end());
    tangent.insert(tangent.end(), m.tangent.begin(), m.tangent.end());
    bitangent.insert(bitangent.end(), m.bitangent.begin(), m.bitangent.end());
    bone_index.insert(bone_index.end(), m.bone_index.begin(), m.bone_index.end());
    bone_weight.insert(bone_weight.end(), m.bone_weight.begin(), m.bone_weight.end());

    // Insert shifted indices into index buffer
    for (const auto &i : m.index)
    {
        index.push_back(index_shift + i);
    }
}

template <typename T, typename K>
void min::mesh<T,K>::scale_uv(const T factor)
{
    for (auto &a : uv)
    {
        a *= factor;
    }
}

template <typename T, typename K>
template <class M>
void min::mesh<T,K>::deserialize(const M &stream)
{
    size_t next = 0;

    // Read in vertices
    vertex = read_le_vector_vec4<T>(stream, next);

    // Read in uvs
    uv = read_le_vector_vec2<T>(stream, next);

    // Read in normals
    normal = read_le_vector_vec3<T>(stream, next);

    // Read in tangents
    tangent = read_le_vector_vec3<T>(stream, next);

    // Read in tangents
    bitangent = read_le_vector_vec3<T>(stream, next);

    // Read in indices
    index = read_le_vector<K>(stream, next);

    // Read in bone index
    bone_index = read_le_vector_vec4<T>(stream, next);

    // Read in bone index
    bone_weight = read_le_vector_vec4<T>(stream, next);
}

template <typename T, typename K>
void min::mesh<T,K>::serialize(std::vector<uint8_t> &stream) const
{
    // Write out vector data to byte stream
    write_le_vector_vec4<T>(stream, vertex);
    write_le_vector_vec2<T>(stream, uv);
    write_le_vector_vec3<T>(stream, normal);
    write_le_vector_vec3<T>(stream, tangent);
    write_le_vector_vec3<T>(stream, bitangent);
    write_le_vector<K>(stream, index);
    write_le_vector_vec4<T>(stream, bone_index);
    write_le_vector_vec4<T>(stream, bone_weight);
}

template <typename T, typename K>
void min::mesh<T,K>::to_file(const std::string &file_name) const
{
    std::vector<uint8_t> stream;

    // Serialize this object into bytes
    this->serialize(stream);

    // Save bytes to file
    std::ofstream file(file_name, std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<char *>(&stream[0]), stream.size());
        file.close();
    }
    else
    {
        throw std::runtime_error("mesh: could not open file '" + file_name + "'");
    }
}

template <typename T, typename K>
void min::mesh<T,K>::from_file(const std::string &file_name)
{
    // Load file in memory
    std::vector<uint8_t> stream;

    // read bytes from file
    std::ifstream file(file_name, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        // Get the size of the file
        std::streampos size = file.tellg();

        // Reserve space for the bytes
        stream.resize(size, 0);

        // Adjust file pointer to beginning
        file.seekg(0, std::ios::beg);

        // Read bytes and close the file
        file.read(reinterpret_cast<char *>(&stream[0]), size);
        file.close();
    }
    else
    {
        throw std::runtime_error("mesh: could not open file '" + file_name + "'");
    }

    // Deserialize the stream of bytes into object
    deserialize<std::vector<uint8_t>>(stream);
}

template <typename T, typename K>
void min::mesh<T,K>::from_file(const min::mem_file &mem)
{
    // Deserialize the stream of bytes into object
    deserialize<mem_file>(mem);
}
