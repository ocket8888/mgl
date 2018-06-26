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

#include "sphere.h"

// Calculate an aabbox using the most separating axis
// Center the sphere at this location
template <typename T, template <typename> class vec>
void min::sphere<T,vec>::fit(const std::vector<vec<T>> &verts)
{
    const auto size = verts.size();
    if (size >= 2)
    {
        // Find the greatest separation
        const std::pair<vec<T>, vec<T>> p = vec<T>::most_separating(verts);
        vec<T> min = p.first;
        vec<T> max = p.second;

        // Set the sphere properties from calculated points
        _center = (min + max) * 0.5;
        const vec<T> dxyz = max - _center;
        _radius2 = dxyz.dot(dxyz);
        _radius = std::sqrt(_radius2);
    }
    else if (size == 1)
    {
        _center = verts[0];
        _radius = _radius2 = 0.0;
    }
}

template <typename T, template <typename> class vec>
min::sphere<T,vec>::sphere(const vec<T> &min, const vec<T> &max)
{
    // calculate center position of space
    _center = (min + max) * 0.5;

    // Calculate maximum distance from space center
    vec<T> half_extent = (max - _center);

    // Calculate the square distance between center and extent
    _radius2 = half_extent.dot(half_extent);

    // Calculate the radius
    _radius = std::sqrt(_radius2);
}

template <typename T, template <typename> class vec>
min::sphere<T,vec>::sphere(const std::vector<vec<T>> &verts) : _center(), _radius()
{
    // Find maximum separation between points
    fit(verts);

    // Extend sphere if points outside
    add(verts);
}

template <typename T, template <typename> class vec>
void min::sphere<T,vec>::add(const std::vector<vec<T>> &verts)
{
    for (const auto &v : verts)
    {
        // If point is outside the sphere
        const vec<T> dxyz = v - _center;
        const T d2 = dxyz.dot(dxyz);
        if (d2 > _radius2)
        {
            const T d = std::sqrt(d2);
            const T update = (_radius + d) * 0.5;
            const T k = (update - _radius) / d;

            // Update the sphere radius and center
            _radius = update;
            _radius2 = _radius * _radius;
            _center += dxyz * k;
        }
    }
}

template <typename T, template <typename> class vec>
inline vec<T> min::sphere<T,vec>::closest_point(const vec<T> &p) const
{
    vec<T> direction = (p - _center).normalize();
    return (_center + direction * _radius);
}

template <typename T, template <typename> class vec>
inline const vec<T> &min::sphere<T,vec>::get_center() const
{
    return _center;
}

template <typename T, template <typename> class vec>
inline vec<T> min::sphere<T,vec>::get_extent() const
{
    // Calculate the circumference of the sphere
    T factor = vec<T>::inverse_unit_length();
    return vec<T>().set_all(2.0 * _radius * factor);
}

template <typename T, template <typename> class vec>
inline vec<T> min::sphere<T,vec>::get_min() const
{
    T factor = vec<T>::inverse_unit_length();
    return _center - vec<T>().set_all(_radius * factor);
}

template <typename T, template <typename> class vec>
inline vec<T> min::sphere<T,vec>::get_max() const
{
    T factor = vec<T>::inverse_unit_length();
    return _center + vec<T>().set_all(_radius * factor);
}

template <typename T, template <typename> class vec>
inline T min::sphere<T,vec>::get_radius() const
{
    return _radius;
}

template <typename T, template <typename> class vec>
inline T min::sphere<T,vec>::get_square_radius() const
{
    return _radius2;
}

template <typename T, template <typename> class vec>
inline std::vector<std::pair<vec<T>, T>> min::sphere<T,vec>::grid(size_t scale) const
{
    // Calculate radius for this scale
    T radius = _radius / scale;

    // Create the grid cells
    return vec<T>::grid_center(get_min(), get_max(), scale, radius);
}

template <typename T, template <typename> class vec>
inline vec<T> min::sphere<T,vec>::normal(const vec<T> &p, T &length, const T tolerance) const
{
    // Calculate normal direction vector
    vec<T> normal = p - _center;

    // Length is initially zero
    length = 0.0;

    // Test for zero vector if p = center
    T mag2 = normal.dot(normal);
    if (mag2 < tolerance)
    {
        // If the two spheres have the same center use up vector
        return vec<T>::up();
    }

    // Calculate the length
    length = std::sqrt(mag2);

    // Normalize the collision normal vector
    normal /= length;

    return normal;
}

template <typename T, template <typename> class vec>
inline bool min::sphere<T,vec>::point_inside(const vec<T> &p) const
{
    vec<T> d = p - _center;
    return (d.dot(d) <= _radius2);
}

template <typename T, template <typename> class vec>
inline void min::sphere<T,vec>::set_position(const vec<T> &pos)
{
    _center = pos;
}

template <typename T, template <typename> class vec>
inline T min::sphere<T,vec>::square_distance(const vec<T> &p) const
{
    // Calculate the distance between p and closest point
    vec<T> closest = closest_point(p);
    vec<T> d = (p - closest);
    return d.dot(d);
}

template <typename T, template <typename> class vec>
inline T min::sphere<T,vec>::square_size() const
{
    // Calculates the squared distance across the sphere extent
    return 4.0 * _radius2;
}

template <typename T, template <typename> class vec>
inline std::vector<std::pair<vec<T>, T>> min::sphere<T,vec>::subdivide() const
{
    // Calculate radius for next level
    T radius = _radius * 0.5;

    // Create the subdivided space
    return vec<T>::subdivide_center(get_min(), get_max(), radius);
}
