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
#ifndef MODEL
#define MODEL

#include <algorithm>
#include <vector>

#include "geom/min/mesh.h"
#include "math/min/vec2.h"
#include "math/min/vec3.h"
#include "math/min/vec4.h"

namespace min
{

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
class model
{
  protected:
    std::vector<mesh<T, K>> _mesh;
    std::vector<bound<T, vec>> _bounds;
    vec<T> _center;

    void calculate_bounds();

  public:
    // This will steal data from provider
    model(std::vector<mesh<T, K>>&&);
    model(const std::vector<mesh<T, K>>&);

    const vec<T> &center_model();
    void calculate_normals();
    void calculate_tangents();
    const vec<T> &get_center() const;
    const std::vector<mesh<T, K>> &get_meshes() const;
    std::vector<mesh<T, K>> &get_meshes();
};
}

#endif
