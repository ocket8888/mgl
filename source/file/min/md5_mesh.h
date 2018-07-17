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
#ifndef __MD5_MESH__
#define __MD5_MESH__

#include <array>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "md5_anim.h"
#include "mem_chunk.h"
#include "strtoken.h"

#include "math/min/mat4.h"
#include "geom/min/mesh.h"
#include "math/min/quat.h"
#include "math/min/vec3.h"
// weights and vertex_weights are only needed to compute the bind pose vertex
// after this they can be safely thrown away
// this is why they no longer reside as a mesh property.

namespace min
{

template <class T>
class md5_joint
{
  private:
    std::string _name;
    int _id;
    vec3<T> _position;
    quat<T> _rotation;

  public:
    md5_joint(const std::string&, const int, const vec3<T>&, const quat<T>&);
    int get_id() const;
    const vec3<T> &get_position() const;
    const quat<T> &get_rotation() const;

};

template <class T>
class weight
{
  private:
    unsigned _id;
    T _bias;
    vec3<T> _position;

  public:
    weight(const unsigned, const T, const vec3<T>&);
    unsigned get_id() const;
    T get_bias() const;
    const vec3<T> &get_position() const;
};

class vertex_weight
{
  private:
    unsigned _start;
    unsigned _count;

  public:
    vertex_weight(const unsigned, const unsigned);
    unsigned get_start() const;
    unsigned get_count() const;
};

template <typename T, typename K>
class md5_mesh
{
  private:
    std::vector<mesh<T, K>> _mesh;
    std::vector<md5_joint<T>> _joints;
    std::vector<weight<T>> _weights;
    std::vector<vertex_weight> _vertex_weights;

    void load_file(const std::string);
    void load(const std::string&);
    void process_joints(const std::vector<std::string>&);
    void process_vertices(const std::vector<std::string>&);
    void process_triangles(const std::vector<std::string>&);
    void process_weights(const std::vector<std::string>&);
    void process_mesh();

  public:
    md5_mesh(const std::string&);
    md5_mesh(const mem_file&);
    const std::vector<md5_joint<T>> &get_joints() const;
    const std::vector<mesh<T,K>> &get_meshes() const;
    std::vector<mesh<T, K>> &get_meshes();
};
}

#endif
