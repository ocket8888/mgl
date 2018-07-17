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
#ifndef MESH
#define MESH
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "file/min/mem_chunk.h"
#include "math/min/vec2.h"
#include "math/min/vec3.h"
#include "math/min/vec4.h"

namespace min
{

template <typename T, typename K>
class mesh
{
  private:
    std::string _name;

    // This method calculates the normal for each triangle plane
    // Each normal-vertex is the same as the normal is the plane normal
    // n = (v1 - v0).cross(v2 - v0).normalize()
    // This is only valid for CW triangles, needs to be negated for CCW triangles

    void calculate_normal(const size_t, const size_t, const size_t);

    // This method solves the following set of equations
    // duv1.x * T + duv1.y * B = dv1;
    // duv2.x * T + duv2.y * B = dv2;
    // duv1.x * [t.x, t.y, t.z] + duv1.y * [b.x, b.y, b.z] = [dv1.x, dv1.y, dv1.z]
    // duv2.x * [t.x, t.y, t.z] + duv2.y * [b.x, b.y, b.z] = [dv2.x, dv2.y, dv2.z]

    // Matrix style equations
    // |duv1.x duv1.y| * |<t.x t.y t.z>| = |<dv1.x dv1.y dv1.z>|
    // |duv2.x duv2.y| * |<b.x b.y b.z>| = |<dv2.x dv2.y dv2.z>|

    // A solution exists at...
    // |<t.x t.y t.z>| = |duv1.x duv1.y|^-1 * |<dv1.x dv1.y dv1.z>|
    // |<b.x b.y b.z>| = |duv2.x duv2.y|^-1 * |<dv2.x dv2.y dv2.z>|

    // Determinant = 1.0 / (duv1.x * duv2.y - duv1.y * duv2.x);
    // 2D matrix formula
    // |<t.x t.y t.z>| = | duv2.y * det -duv1.y * det| * |<dv1.x dv1.y dv1.z>|
    // |<b.x b.y b.z>| = |-duv2.x * det  duv1.x * det| * |<dv2.x dv2.y dv2.z>|

    // B x T = N
    // T X N = B
    // N X B = T
    //      Bitangent (z) (2D: y)
    //     |
    //     |                Thus bitangent.cross(tangent) == normal -- left handed coordinate system
    //     /------ Tangent (x) (2D: x)
    //    /
    //   Normal (y)

    void calculate_tangent(const size_t, const size_t, const size_t);
    void add_tangents(const std::pair<vec3<T>, vec3<T>>&, const size_t);

  public:
    mesh(const std::string &name) : _name(name) {}
    std::vector<vec4<T>> vertex;
    std::vector<vec2<T>> uv;
    std::vector<vec3<T>> normal;
    std::vector<vec3<T>> tangent;
    std::vector<vec3<T>> bitangent;
    std::vector<K> index;

    // These are for animated meshes only
    std::vector<vec4<T>> bone_index;
    std::vector<vec4<T>> bone_weight;

    void calculate_normals();
    void calculate_tangents();
    void clear();
    void flip_uv_x_axis();
    void flip_uv_y_axis();
    const std::string &get_name() const;
    void merge(const mesh<T,K>&);
    void scale_uv(const T);
    void serialize(std::vector<uint8_t>&) const;
    void to_file(const std::string&) const;
    void from_file(const std::string&);
    void from_file(const mem_file&);

    template<class M>
    void deserialize(const M&);
};
}

#endif
