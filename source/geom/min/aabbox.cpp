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

#include "aabbox.h"

template class min::aabbox<float, min::vec3>;

template <typename T, template <typename> class vec>
min::aabbox<T, vec>::aabbox(const std::vector<vec<T>> &verts) : _min(), _max() {
	add(verts);
}


template <typename T, template <typename> class vec>
void min::aabbox<T, vec>::add(const std::vector<vec<T>> &verts)
{
    // Calculate the maximum extent of the point list
    const auto size = verts.size();
    if (size >= 2)
    {
        const std::pair<vec<T>, vec<T>> p = vec<T>::extents(verts);
        _min = p.first - var<T>::TOL_REL2;
        _max = p.second + var<T>::TOL_REL2;
    }
    else if (size == 1)
    {
        _min = verts[0];
        _max = verts[0];
    }
}

template <typename T, template <typename> class vec>
vec<T> min::aabbox<T, vec>::closest_point(const vec<T> &p) const
{
    // Compute the closest point to this aabb
    // Return this point if inside
    vec<T> out = p;
    out.clamp(_min, _max);

    return out;
}

template <typename T, template <typename> class vec>
vec<T> min::aabbox<T, vec>::get_center() const
{
    return (_min + _max) * 0.5;
}

template <typename T, template <typename> class vec>
vec<T> min::aabbox<T, vec>::get_extent() const
{
    return _max - _min;
}

template <typename T, template <typename> class vec>
const vec<T> &min::aabbox<T, vec>::get_min() const
{
    return _min;
}

template <typename T, template <typename> class vec>
const vec<T> &min::aabbox<T, vec>::get_max() const
{
    return _max;
}

template <typename T, template <typename> class vec>
std::vector<std::pair<vec<T>, vec<T>>> min::aabbox<T, vec>::grid(size_t scale) const
{
    // Create the grid cells
    return vec<T>::grid(_min, _max, scale);
}

template <typename T, template <typename> class vec>
bool min::aabbox<T, vec>::point_inside(const vec<T> &p) const
{
    return p.within(_min, _max);
}

template <typename T, template <typename> class vec>
void min::aabbox<T, vec>::set_position(const vec<T> &pos)
{
    // min = _min - center = (_min - _max) * 0.5
    // max = _max - center = (_max - _min) * 0.5
    const vec<T> min = (_min - _max) * 0.5;
    const vec<T> max = (_max - _min) * 0.5;

    // Update the min and max
    _min = min + pos;
    _max = max + pos;
}

template <typename T, template <typename> class vec>
T min::aabbox<T, vec>::square_distance(const vec<T> &p) const
{
    // Return the distance to this aabb
    // Return 0 if inside it
    vec<T> closest = closest_point(p);
    vec<T> d = (p - closest);
    return d.dot(d);
}

template <typename T, template <typename> class vec>
T min::aabbox<T, vec>::square_size() const
{
    // Calculates the squared distance across the aabb extent
    vec<T> extent = get_extent();
    return extent.dot(extent);
}

template <typename T, template <typename> class vec>
std::vector<std::pair<vec<T>, vec<T>>> min::aabbox<T, vec>::subdivide() const
{
    // Create the subdivided space
    return vec<T>::subdivide(_min, _max);
}
