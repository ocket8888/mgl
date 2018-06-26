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

#include "oobbox.h"

template <typename T, template <typename> class vec, template <typename> class rot>
inline void min::oobbox_base<T,vec,rot>::add(const std::vector<vec<T>> &verts)
{
    // Calculate the maximum extent of the point list
    const auto size = verts.size();
    vec<T> min, max;
    if (size >= 2)
    {
        const std::pair<vec<T>, vec<T>> p = vec<T>::extents(verts);
        min = p.first - var<T>::TOL_REL2;
        max = p.second + var<T>::TOL_REL2;
    }
    else if (size == 1)
    {
        min = verts[0];
        max = verts[0];
    }

    // Create box internals
    _center = (max + min) * 0.5;
    _half_extent = (max - min) * 0.5;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const vec<T> min::oobbox_base<T,vec,rot>::get_local_min() const
{
    // This returns min in object space (AABB)
    return _half_extent * -1.0;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const vec<T> min::oobbox_base<T,vec,rot>::get_local_max() const
{
    // This returns max in object space (AABB)
    return _half_extent;
}

template <typename T, template <typename> class vec, template <typename> class rot>
min::oobbox_base<T,vec,rot>::oobbox_base(const std::vector<vec<T>> &verts) : _axes(vec<T>::axes())
{
    add(verts);
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline vec<T> min::oobbox_base<T,vec,rot>::align(const vec<T> &v) const
{
    // Transform the point in object space
    return _axes.align(v);
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline vec<T> min::oobbox_base<T,vec,rot>::closest_point(const vec<T> &p) const
{
    // Transform to local coordinates
    // Project the point vector along local axes and clamp to half extent
    const vec<T> out = (p - _center).project_point(_axes, _half_extent);

    // Return closest point
    return out + _center;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const min::coord_sys<T, vec> &min::oobbox_base<T,vec,rot>::get_axes() const
{
    return _axes;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const vec<T> &min::oobbox_base<T,vec,rot>::get_center() const
{
    return _center;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const vec<T> &min::oobbox_base<T,vec,rot>::get_half_extent() const
{
    return _half_extent;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline vec<T> min::oobbox_base<T,vec,rot>::get_extent() const
{
    return _half_extent * 2.0;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const vec<T> min::oobbox_base<T,vec,rot>::get_min() const
{
    // This returns min in world space (AABB)
    return _center - _half_extent;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const vec<T> min::oobbox_base<T,vec,rot>::get_max() const
{
    // This returns max in world space (AABB)
    return _center + _half_extent;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline const rot<T> &min::oobbox_base<T,vec,rot>::get_rotation() const
{
    return _rotation;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline std::vector<std::pair<vec<T>, vec<T>>> min::oobbox_base<T,vec,rot>::grid(size_t scale) const
{
    // Create the grid cells in world space AABB
    return vec<T>::grid(get_min(), get_max(), scale);
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline bool min::oobbox_base<T,vec,rot>::point_inside(const vec<T> &p) const
{
    // Transform the point into object's coordinate system
    const vec<T> t = align(p - _center);

    // Test if point is in aabb in object space
    return t.within(get_local_min(), get_local_max());
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline void min::oobbox_base<T,vec,rot>::set_position(const vec<T> &position)
{
    _center = position;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline void min::oobbox_base<T,vec,rot>::set_rotation(const rot<T> &r)
{
    // Reset rotation
    _rotation = r;

    // Get a copy of the reference axes
    coord_sys<T, vec> ref_axes = vec<T>::axes();

    // Rotate the coordinate axes
    ref_axes.rotate(r);

    // Reassign to local axes
    _axes = ref_axes;
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline T min::oobbox_base<T,vec,rot>::square_distance(const vec<T> &p) const
{
    // Transform to local coordinates
    // Return square distance from point P
    return (p - _center).project_length(_axes, _half_extent);
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline T min::oobbox_base<T,vec,rot>::square_size() const
{
    // This assumes a non rotated box (aabb)!
    // Calculates the squared distance across the box extent
    vec<T> extent = get_extent();
    return extent.dot(extent);
}

template <typename T, template <typename> class vec, template <typename> class rot>
inline std::vector<std::pair<vec<T>, vec<T>>> min::oobbox_base<T,vec,rot>::subdivide() const
{
    // Create the subdivided space in world space AABB
    return vec<T>::subdivide(get_min(), get_max());
}
