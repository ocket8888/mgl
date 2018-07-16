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

#include "model.h"


template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
inline void min::model<T,K,vec,bound>::calculate_bounds()
{
    // Creating bounding volumes for all meshes
    for (auto &m : _mesh)
    {
        _bounds.emplace_back(m.vertex);
    }

    // Calculate the model center by averaging all mesh centers
    _center = vec<T>();
    for (size_t i = 0; i < _mesh.size(); i++)
    {
        _center += _bounds[i].get_center();
    }
    _center /= _mesh.size();
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
min::model<T,K,vec,bound>::model(std::vector<min::mesh<T, K>> &&mesh) : _mesh(std::move(mesh)), _center()
{
    calculate_bounds();

    // calculate normals
    calculate_normals();

    // calculate tangents
    calculate_tangents();
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
min::model<T,K,vec,bound>::model(const std::vector<min::mesh<T, K>> &mesh) : _mesh(mesh), _center()
{
    calculate_bounds();

    // calculate normals
    calculate_normals();

    // calculate tangents
    calculate_tangents();
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
const vec<T> &min::model<T,K,vec,bound>::center_model()
{
    // Center all vertices in the model
    for (auto &m : _mesh)
    {
        // Center all meshes by substracting the center
        for (auto &vert : m.vertex)
        {
            vert -= _center;
        }
    }

    // Clear out the model bounding volumes
    _bounds.clear();

    // Recalculate the model bounding volumes
    calculate_bounds();

    return _center;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
void min::model<T,K,vec,bound>::calculate_normals()
{
    for (auto &m : _mesh)
    {
        m.calculate_normals();
    }
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
void min::model<T,K,vec,bound>::calculate_tangents()
{
    for (auto &m : _mesh)
    {
        m.calculate_tangents();
    }
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
inline const vec<T> &min::model<T,K,vec,bound>::get_center() const
{
    return _center;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
inline const std::vector<min::mesh<T, K>> &min::model<T,K,vec,bound>::get_meshes() const
{
    // Allows for const usage
    return _mesh;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
inline std::vector<min::mesh<T, K>> &min::model<T,K,vec,bound>::get_meshes()
{
    // Allows for const usage
    return _mesh;
}