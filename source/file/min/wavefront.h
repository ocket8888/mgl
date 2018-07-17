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
#ifndef WAVEFRONT
#define WAVEFRONT

#include <array>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "geom/min/mesh.h"

#include "mem_chunk.h"
#include "strtoken.h"

// This class offers limited support for reading WaveFront .obj files
// The file will be parsed into a list of vertices, uv texture coordinates,
// normals and index buffers.

// Support is limited to fully triangulated meshes, that have uv and normals
// defined for each vertex

namespace min
{

template <typename T, typename K>
class wavefront
{
  private:
    std::vector<mesh<T, K>> _mesh;
    std::vector<vec4<T>> _v;
    std::vector<vec2<T>> _uv;
    std::vector<vec3<T>> _n;
    std::vector<K> _i;
    bool _invert;

    void flush();
    void load_file(const std::string _file);
    void load(const std::string&);
    void process_mesh(mesh<T, K>&);
    void process_object(const std::string&);
    void process_vertex(const std::string&);
    void process_uv(const std::string&);
    void process_normal(const std::string&);
    void process_face(const std::string&);


  public:
    wavefront(const std::string&, const bool = false);
    wavefront(const mem_file&, const bool = false);

    const std::vector<mesh<T,K>> &get_meshes() const;
    std::vector<mesh<T,K>> &get_meshes();

};
}

#endif
