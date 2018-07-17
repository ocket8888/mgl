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

#include "vec2.h"

template class min::vec2<float>;
template class min::vec2<double>;

template <typename T>
min::vec2<T>::vec2() : x(0.0), y(0.0) {}

template <typename T>
min::vec2<T>::vec2(const T x, const T y) : x(x), y(y) {}

template <typename T>
min::vec2<T> &min::vec2<T>::set_all(const T v)
{
    x = v;
    y = v;

    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::abs()
{
    x = std::abs(x);
    y = std::abs(y);

    return *this;
}

template <typename T>
bool min::vec2<T>::any_zero_outside(const min::vec2<T> &p, const min::vec2<T> &min, const min::vec2<T> &max) const
{
    // If p is zero and this is outside min and max return true else false
    if (std::abs(p.x) <= var<T>::TOL_REL)
    {
        if (x < min.x || x > max.x)
            return true;
    }
    else if (std::abs(p.y) <= var<T>::TOL_REL)
    {
        if (y < min.y || y > max.y)
            return true;
    }

    return false;
}

template <typename T>
min::vec2<T> &min::vec2<T>::clamp(const min::vec2<T> &min, const min::vec2<T> &max)
{
    min::clamp(x, min.x, max.x);
    min::clamp(y, min.y, max.y);

    return *this;
}

template <typename T>
min::vec2<T> min::vec2<T>::clamp_direction(const min::vec2<T> &min, const min::vec2<T> &max)
{
    T x = min::clamp_direction(x, min.x, max.x);
    T y = min::clamp_direction(y, min.y, max.y);

    return min::vec2<T>(x, y);
}

template <typename T>
T min::vec2<T>::cross(const min::vec2<T> &A) const
{
    return x * A.y - y * A.x;
}

template <typename T>
T min::vec2<T>::dot(const min::vec2<T> &A) const
{
    return x * A.x + y * A.y;
}

template <typename T>
std::pair<min::vec2<T>, min::vec2<T>> min::vec2<T>::extents(const std::vector<min::vec2<T>> &verts)
{
    // Find the greatest extents in the collection of min::vec2<T>
    const auto size = verts.size();
    if (size > 1)
    {
        const min::vec2<T> &first = verts[0];
        T minx = first.x;
        T miny = first.y;
        T maxx = first.x;
        T maxy = first.y;
        for (size_t i = 1; i < size; i++)
        {
            const min::vec2<T> &v = verts[i];
            extend<T>(v.x, minx, maxx);
            extend<T>(v.y, miny, maxy);
        }

        // Return the greatest extents
        return std::make_pair(vec2<T>(minx, miny), min::vec2<T>(maxx, maxy));
    }

    return std::make_pair(vec2<T>(), min::vec2<T>());
}
// Creating N^2 grid from min corner to max corner
// Grid iterates in the Y than X dimension to reflect tree data layout
// ^     /-----/-----/
// |    /  1  /  3  /
// c   /-----/-----/
// o  /  0  /  2  /
// l /-----/-----/
//   row --->

template <typename T>
std::vector<std::pair<min::vec2<T>, min::vec2<T>>> min::vec2<T>::grid(const min::vec2<T> &min, const min::vec2<T> &max, const size_t scale)
{
    // Create out min::vector
    std::vector<std::pair<min::vec2<T>, min::vec2<T>>> out;
    out.reserve(scale * scale);

    // Calculate the grid dimensions
    const min::vec2<T> size = max - min;
    const min::vec2<T> extent = size / scale;
    const T dx = extent.x;
    const T dy = extent.y;

    // Start at the bottom left corner
    min::vec2<T> cell;

    // Across the X dim
    for (T x = min.x; x < max.x; x += dx)
    {
        // Set the cell x value
        cell.x = x;

        // Across the Y dim
        for (T y = min.y; y < max.y; y += dy)
        {
            // Set the cell y value
            cell.y = y;
            out.emplace_back(std::make_pair(cell, cell + extent));
        }
    }

    // return the compute grid
    return out;
}

template <typename T>
std::vector<std::pair<min::vec2<T>, T>> min::vec2<T>::grid_center(const min::vec2<T> &min, const min::vec2<T> &max, const size_t scale, const T size)
{
    // Create out min::vector
    std::vector<std::pair<min::vec2<T>, T>> out;
    out.reserve(scale * scale);

    // Calculate the grid dimensions
    const min::vec2<T> extent = (max - min) / scale;
    const min::vec2<T> half_extent = extent * 0.5;
    const T dx = extent.x;
    const T dy = extent.y;

    // Start at the bottom left corner
    min::vec2<T> cell;

    // Across the X dim
    for (T x = min.x; x < max.x; x += dx)
    {
        // Set the cell x value
        cell.x = x;

        // Across the Y dim
        for (T y = min.y; y < max.y; y += dy)
        {
            // Set the cell y value
            cell.y = y;
            out.emplace_back(std::make_pair(cell + half_extent, size));
        }
    }

    // return the compute grid
    return out;
}

template <typename T>
std::pair<size_t, size_t> min::vec2<T>::grid_index(const min::vec2<T> &min, const min::vec2<T> &extent, const min::vec2<T> &point)
{
    // Calculate the grid dimensions
    const T ex = extent.x;
    const T ey = extent.y;

    // Get the row / col of cell
    const size_t col = (point.x - min.x) / ex;
    const size_t row = (point.y - min.y) / ey;

    // Return the row / col of cell
    return std::make_pair(col, row);
}

template <typename T>
std::pair<size_t, size_t> min::vec2<T>::grid_index(const size_t index, const size_t scale)
{
    const size_t col = index / scale;
    const size_t row = index - (col * scale);

    // return tuple
    return std::make_pair(col, row);
}

template <typename T>
size_t min::vec2<T>::grid_key(const min::vec2<T> &min, const min::vec2<T> &extent, const size_t scale, const min::vec2<T> &point)
{
    // Calculate the cell location
    const std::pair<size_t, size_t> index = grid_index(min, extent, point);

    // Get the row / col of cell
    const size_t col = index.first;
    const size_t row = index.second;

    // Return the grid index key for accessing cell
    return col * scale + row;
}

template <typename T>
size_t min::vec2<T>::grid_key(const std::pair<size_t, size_t> &index, const size_t scale)
{
    // Get the row / col of cell
    const size_t col = index.first;
    const size_t row = index.second;

    // Return the grid index key for accessing cell
    return col * scale + row;
}

template <typename T>
void min::vec2<T>::grid_overlap(std::vector<size_t> &out, const min::vec2<T> &min, const min::vec2<T> &extent, const size_t scale, const min::vec2<T> &b_min, const min::vec2<T> &b_max)
{
    // Reserve space for output
    out.clear();
    out.reserve(9);

    // Calculate the grid dimensions
    const T dx = extent.x;
    const T dy = extent.y;

    // Calculate the center cell
    const min::vec2<T> center = (b_min + b_max) * 0.5;

    // Center cell indices
    const std::pair<size_t, size_t> p = min::vec2<T>::grid_index(min, extent, center);
    const size_t x = p.first;
    const size_t y = p.second;

    // Bounds of the center cell
    const T minx = min.x + dx * x;
    const T miny = min.y + dy * y;
    const T maxx = minx + dx;
    const T maxy = miny + dy;

    // Calculate the neighboring indices
    const int nx = x - 1;
    const int px = x + 1;
    const int ny = y - 1;
    const int py = y + 1;

    // Calculate whether neighbors are outside the main grid
    const bool nxg = nx >= 0;
    const bool pxg = px < (long)scale;
    const bool nyg = ny >= 0;
    const bool pyg = py < (long)scale;
    const bool lx = b_min.x < minx;
    const bool ly = b_min.y < miny;
    const bool gx = b_max.x >= maxx;
    const bool gy = b_max.y >= maxy;
    const bool tny = ly && nyg;
    const bool tgy = gy && pyg;

    // -X
    if (lx && nxg)
    {
        if (tny)
            out.push_back(nx * scale + ny); // -Y
        out.push_back(nx * scale + y);      // Y
        if (tgy)
            out.push_back(nx * scale + py); // +Y
    }

    // X
    if (tny)
        out.push_back(x * scale + ny); // -Y
    out.push_back(x * scale + y);      // Y
    if (tgy)
        out.push_back(x * scale + py); // +Y

    // +X
    if (gx && pxg)
    {
        if (tny)
            out.push_back(px * scale + ny); // -Y
        out.push_back(px * scale + y);      // Y
        if (tgy)
            out.push_back(px * scale + py); // + Y
    }
}

template <typename T>
std::tuple<int, T, T, int, T, T> min::vec2<T>::grid_ray(const min::vec2<T> &extent, const min::vec2<T> &origin, const min::vec2<T> &dir, const min::vec2<T> &inv_dir)
{
    // Get the grid dimensions
    const T ex = extent.x;
    const T ey = extent.y;

    // Get the origin starting points
    const T x = origin.x;
    const T y = origin.y;

    // Calculate distance to left of ray origin
    const T minx = ex * std::floor(x / ex);

    // Calculate distance to right of ray origin
    const T maxx = minx + ex;

    // Test for ray parallel to X axis
    T tx = std::numeric_limits<T>::max();
    T dtx = std::numeric_limits<T>::max();
    int drx = 1;
    if (std::abs(dir.x) >= var<T>::TOL_ZERO)
    {
        // Choose distance based on ray direction
        if (dir.x < 0.0)
        {
            drx = -1;
            tx = (x - minx) * std::abs(inv_dir.x);
        }
        else
        {
            tx = (maxx - x) * std::abs(inv_dir.x);
        }

        // Compute the length of the cell along the ray
        dtx = ex * std::abs(inv_dir.x);
    }

    // Calculate distance to below ray origin
    const T miny = ey * std::floor(y / ey);

    // Calculate distance to above ray origin
    const T maxy = miny + ey;

    // Test for ray parallel to Y axis
    T ty = std::numeric_limits<T>::max();
    T dty = std::numeric_limits<T>::max();
    int dry = 1;
    if (std::abs(dir.y) >= var<T>::TOL_ZERO)
    {
        // Choose distance based on ray direction
        if (dir.y < 0.0)
        {
            dry = -1;
            ty = (y - miny) * std::abs(inv_dir.y);
        }
        else
        {
            ty = (maxy - y) * std::abs(inv_dir.y);
        }

        // Compute the length of the cell along the ray
        dty = ey * std::abs(inv_dir.y);
    }

    // return the ray tuple
    return std::make_tuple(drx, tx, dtx, dry, ty, dty);
}

template <typename T>
size_t min::vec2<T>::grid_ray_next(std::pair<size_t, size_t> &index, std::tuple<int, T, T, int, T, T> &grid_ray, bool &flag, const size_t scale)
{
    // Get the cell row / col
    size_t &col = index.first;
    size_t &row = index.second;

    // X
    const int &drx = std::get<0>(grid_ray);
    T &tx = std::get<1>(grid_ray);
    const T &dtx = std::get<2>(grid_ray);

    // Y
    const int &dry = std::get<3>(grid_ray);
    T &ty = std::get<4>(grid_ray);
    const T &dty = std::get<5>(grid_ray);

    // Should we move along the x, y, or z axis? Guarantee a valid return value.
    if (tx <= ty)
    {
        if (!(col == 0 && drx == -1) && !(col == scale - 1 && drx == 1))
        {
            // Increment column == choose x
            col += drx;
            tx += dtx;
        }
        else
        {
            flag = true;
        }
    }
    else
    {
        if (!(row == 0 && dry == -1) && !(row == scale - 1 && dry == 1))
        {
            // Increment row == choose y
            row += dry;
            ty += dty;
        }
        else
        {
            flag = true;
        }
    }

    // Return the grid index key for accessing cell
    return col * scale + row;
}

template <typename T>
template <typename F>
void min::vec2<T>::grid_range(const min::vec2<T> &min, const min::vec2<T> &extent, const size_t scale,
                              const min::vec2<T> &over_min, const min::vec2<T> &over_max,
                              const F &f)
{
    // Assumes over_min and over_max are clamped to world edges!!
    // Get the key of min and max points for overlap
    const std::pair<size_t, size_t> p_min = min::vec2<T>::grid_index(min, extent, over_min);
    const std::pair<size_t, size_t> p_max = min::vec2<T>::grid_index(min, extent, over_max);

    // Get all cells in between points and get overlapping shapes
    for (size_t i = p_min.first; i <= p_max.first; i++)
    {
        for (size_t j = p_min.second; j <= p_max.second; j++)
        {
            // Get the key for this index
            const size_t key = min::vec2<T>::grid_key(std::make_pair(i, j), scale);

            // Callback function on f
            f(key);
        }
    }
}

template <typename T>
bool min::vec2<T>::inside(const min::vec2<T> &min, const min::vec2<T> &max) const
{
    // Return true if this min::vector is inside the min and max min::vector range
    return (x > min.x + var<T>::TOL_REL && x < max.x - var<T>::TOL_REL && y > min.y + var<T>::TOL_REL && y < max.y - var<T>::TOL_REL);
}

template <typename T>
min::vec2<T> min::vec2<T>::inverse() const
{
    return min::vec2<T>(1.0 / x, 1.0 / y);
}

template <typename T>
min::vec2<T> min::vec2<T>::inverse_safe() const
{
    const T x = safe_inverse<T>(x);
    const T y = safe_inverse<T>(y);

    // return inverse
    return min::vec2<T>(x, y);
}

template <typename T>
min::vec2<T> min::vec2<T>::lerp(const min::vec2<T> &v0, const min::vec2<T> &v1, T t)
{
    return (v0 + (v1 - v0) * (t));
}

template <typename T>
min::vec2<T> min::vec2<T>::interpolate(const min::vec2<T> &v0, const min::vec2<T> &v1, T t)
{
    return lerp(v0, v1, t);
}

template <typename T>
T min::vec2<T>::magnitude() const
{
    return std::sqrt(x * x + y * y);
}

template <typename T>
T min::vec2<T>::max() const
{
    return std::max(x, y);
}

template <typename T>
T min::vec2<T>::min() const
{
    return std::min(x, y);
}

template <typename T>
std::pair<min::vec2<T>, min::vec2<T>> min::vec2<T>::most_separating(const std::vector<min::vec2<T>> &verts)
{
    // Find the two most separating points in the collection of min::vec2<T>
    const auto size = verts.size();
    if (size > 1)
    {
        size_t minx, maxx, miny, maxy, min, max;
        minx = maxx = miny = maxy = min = max = 0;
        for (size_t i = 0; i < size; i++)
        {
            const min::vec2<T> &v = verts[i];
            if (v.x > verts[maxx].x)
                maxx = i;
            if (v.x < verts[minx].x)
                minx = i;
            if (v.y > verts[maxy].y)
                maxy = i;
            if (v.y < verts[miny].y)
                miny = i;
        }

        // Calculate span of the extents
        const min::vec2<T> dx = verts[maxx] - verts[minx];
        const min::vec2<T> dy = verts[maxy] - verts[miny];

        // Calculated the squared euclidean distance
        const T dx2 = dx.dot(dx);
        const T dy2 = dy.dot(dy);

        // Calculate the max extents
        min = minx;
        max = maxx;
        if (dy2 > dx2)
        {
            min = miny;
            max = maxy;
        }

        // Return the two most separating verts
        return std::make_pair(verts[min], verts[max]);
    }

    return std::make_pair(vec2<T>(), min::vec2<T>());
}

template <typename T>
min::vec2<T> min::vec2<T>::normal(const min::vec2<T> &a, const min::vec2<T> &b, const min::vec2<T> &c)
{
    // Computes normal min::vector to two points, third argument is ignored
    // Normal is not normalized
    return (b - a).orthogonal();
}

template <typename T>
min::vec2<T> &min::vec2<T>::normalize()
{
    const T inv_mag = 1.0 / magnitude();
    x *= inv_mag;
    y *= inv_mag;

    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::normalize_safe(const min::vec2<T> &safe)
{
    const T mag = magnitude();
    if (std::abs(mag) > var<T>::TOL_ZERO)
    {
        T inv_mag = 1.0 / mag;
        x *= inv_mag;
        y *= inv_mag;
    }
    else
    {
        x = safe.x;
        y = safe.y;
    }

    return *this;
}

template <typename T>
void min::vec2<T>::order(vec2<T> &min, min::vec2<T> &max)
{
    // Order the components into min and max
    if (min.x > max.x)
    {
        swap(min.x, max.x);
    }
    if (min.y > max.y)
    {
        swap(min.y, max.y);
    }
}

template <typename T>
min::vec2<T> min::vec2<T>::orthogonal() const
{
    // Compute the orthogonal min::vector to A
    return min::vec2<T>(y, -x);
}

template <typename T>
min::vec2<T> min::vec2<T>::project_point(const min::coord_sys<T, min::vec2> &axis, const min::vec2<T> &extent)
{
    // Project this onto local x axis
    T x = this->dot(axis.x());

    // Clamp x onto the box half extent, else x
    min::clamp<T>(x, -extent.x, extent.x);

    // Project this onto local y axis
    T y = this->dot(axis.y());

    // Clamp y onto the box half extent, else y
    min::clamp<T>(y, -extent.y, extent.y);

    // Compute the point along this axis
    return (axis.x() * x) + (axis.y() * y);
}

template <typename T>
T min::vec2<T>::project_length(const min::coord_sys<T, min::vec2> &axis, const min::vec2<T> &extent)
{
    // Project this onto local x axis
    const T x = this->dot(axis.x());

    // Clamp x onto the box half extent, else zero
    const T dx = clamp_value<T>(x, -extent.x, x + extent.x, extent.x, x - extent.x);

    // Project this onto local y axis
    const T y = this->dot(axis.y());

    // Clamp y onto the box half extent, else 0
    const T dy = clamp_value<T>(y, -extent.y, y + extent.y, extent.y, y - extent.y);

    // Compute the square distance from this point
    return (dx * dx) + (dy * dy);
}

template <typename T>
bool min::vec2<T>::project_sat(const min::coord_sys<T, min::vec2> &axis1, const min::vec2<T> &center1, const min::vec2<T> &extent1, const min::coord_sys<T, min::vec2> &axis2, const min::vec2<T> &center2, const min::vec2<T> &extent2)
{
    // This performs the separating axis theorem for checking oobb-oobb intersections
    // For every axis test (C2-C1).dot(L) > (a.get_extent() + b.get_extent()).dot(L)
    // This means testing the difference between box centers, C1 & C2, along the separating axis L
    // With the addition of box extents along this same axis L
    // For 2D, there are 4 axes that need to be tested against...
    // 2*2 = 4 local box axes

    // Rotation min::matrix expressing A2 in A1's coordinate frame
    const T x1x2 = axis1.x().dot(axis2.x());
    const T x1y2 = axis1.x().dot(axis2.y());
    const T y1x2 = axis1.y().dot(axis2.x());
    const T y1y2 = axis1.y().dot(axis2.y());

    const T absx1x2 = std::abs(x1x2) + var<T>::TOL_REL;
    const T absx1y2 = std::abs(x1y2) + var<T>::TOL_REL;
    const T absy1x2 = std::abs(y1x2) + var<T>::TOL_REL;
    const T absy1y2 = std::abs(y1y2) + var<T>::TOL_REL;

    // Bring translation into A1's coordinate frame
    const min::vec2<T> d = center2 - center1;
    const min::vec2<T> t = min::vec2<T>(d.dot(axis1.x()), d.dot(axis1.y()));

    // Test L = A1.x; d1 and d2 is the length of extents along L
    T dL1 = extent1.x;
    T dL2 = extent2.x * absx1x2 + extent2.y * absx1y2;
    if (std::abs(t.x) > dL1 + dL2)
        return false;

    // Test L = A1.y; d1 and d2 is the length of extents along L
    dL1 = extent1.y;
    dL2 = extent2.x * absy1x2 + extent2.y * absy1y2;
    if (std::abs(t.y) > dL1 + dL2)
        return false;

    // Test L = A2.x; d1 and d2 is the length of extents along L
    dL1 = extent1.x * absx1x2 + extent1.y * absy1x2;
    dL2 = extent2.x;
    if (std::abs(t.x * x1x2 + t.y * y1x2) > dL1 + dL2)
        return false;

    // Test L = A2.y; d1 and d2 is the length of extents along L
    dL1 = extent1.x * absx1y2 + extent1.y * absy1y2;
    dL2 = extent2.y;
    if (std::abs(t.x * x1y2 + t.y * y1y2) > dL1 + dL2)
        return false;

    return true;
}


template <typename T>
std::pair<min::vec2<T>, T> min::vec2<T>::project_sat_penetration(
    const min::coord_sys<T, min::vec2> &axis1, const min::vec2<T> &center1, const min::vec2<T> &extent1,
    const min::coord_sys<T, min::vec2> &axis2, const min::vec2<T> &center2, const min::vec2<T> &extent2, const T tolerance)
{
    // This performs the separating axis theorem for checking oobb-oobb intersection penetration
    // For every axis, penetration = (a.get_extent() + b.get_extent()).dot(L) - (C2-C1).dot(L)
    // This means testing the difference between box centers, C1 & C2, along the separating axis L
    // With the addition of box extents along this same axis L
    // For 2D, there are 4 axes that need to be computed...
    // 2*2 = 4 local box axes

    // Rotation matrix expressing A2 in A1's coordinate frame
    const T x1x2 = axis1.x().dot(axis2.x());
    const T x1y2 = axis1.x().dot(axis2.y());
    const T y1x2 = axis1.y().dot(axis2.x());
    const T y1y2 = axis1.y().dot(axis2.y());

    const T absx1x2 = std::abs(x1x2) + tolerance;
    const T absx1y2 = std::abs(x1y2) + tolerance;
    const T absy1x2 = std::abs(y1x2) + tolerance;
    const T absy1y2 = std::abs(y1y2) + tolerance;

    // Bring translation into A1's coordinate frame
    const vec2<T> d = center2 - center1;
    const vec2<T> t = vec2<T>(d.dot(axis1.x()), d.dot(axis1.y()));

    // Store axis and penetration depths
    vec2<T> axes[4];
    T penetration[4];

    // Test L = A1.x; d1 and d2 is the length of extents along L
    T dL1 = extent1.x;
    T dL2 = extent2.x * absx1x2 + extent2.y * absx1y2;
    axes[0] = axis1.x();
    penetration[0] = (dL1 + dL2) - std::abs(t.x);

    // Test L = A1.y; d1 and d2 is the length of extents along L
    dL1 = extent1.y;
    dL2 = extent2.x * absy1x2 + extent2.y * absy1y2;
    axes[1] = axis1.y();
    penetration[1] = (dL1 + dL2) - std::abs(t.y);

    // Test L = A2.x; d1 and d2 is the length of extents along L
    dL1 = extent1.x * absx1x2 + extent1.y * absy1x2;
    dL2 = extent2.x;
    axes[2] = axis2.x();
    penetration[2] = (dL1 + dL2) - std::abs(t.x * x1x2 + t.y * y1x2);

    // Test L = A2.y; d1 and d2 is the length of extents along L
    dL1 = extent1.x * absx1y2 + extent1.y * absy1y2;
    dL2 = extent2.y;
    axes[3] = axis2.y();
    penetration[3] = (dL1 + dL2) - std::abs(t.x * x1y2 + t.y * y1y2);

    // normal default up vector return and zero penetration
    vec2<T> normal = vec2<T>::up();
    T overlap = 0.0;

    // Find the minimum, non-zero penetration index
    T min = 1E15;
    int index = -1;
    for (int i = 0; i < 4; i++)
    {
        // Prune all parallel normal vectors and non-penetrating depths
        const T mag2 = axes[i].dot(axes[i]);
        if (mag2 > tolerance && penetration[i] > tolerance && penetration[i] < min)
        {
            min = penetration[i];
            index = i;
        }
    }

    // check if we found an intersection penetration
    if (index != -1)
    {
        // Calculate the sign of normal towards body1 and scale normal
        const vec2<T> sign = (center1 - center2).sign();
        normal = axes[index].abs() * sign;
        overlap = min;
    }

    // return normal vector and minimum penentration
    return std::make_pair(normal, overlap);
}

template <typename T>
std::pair<min::vec2<T>, T> min::vec2<T>::project_sat_aligned_penetration(
    const min::vec2<T> &center1, const min::vec2<T> &extent1,
    const min::vec2<T> &center2, const min::vec2<T> &extent2, const T tolerance)
{
    // This performs the separating axis theorem for checking aabb-aabb intersection penetration
    // For every axis, penetration = (a.get_extent() + b.get_extent()).dot(L) - (C2-C1).dot(L) on aligned axises
    // This means testing the difference between box centers, C1 & C2, along the separating axis L
    // With the addition of box extents along this same axis L
    // For 2D, there are 2 axes that need to be computed...
    // 2 local box axes

    const min::vec2<T> d = center1 - center2;
    const min::vec2<T> t = min::vec2<T>(d).abs();

    // Test L = A1.x = A2.x; d1 and d2 is the length of extents along L
    // Test L = A1.y = A2.y; d1 and d2 is the length of extents along L
    const min::vec2<T> dL = (extent1 + extent2 + tolerance) - t;

    // Store axis and penetration depths
    const min::vec2<T> axes[2] = {vec2<T>(1.0, 0.0), min::vec2<T>(0.0, 1.0)};
    const T penetration[2] = {dL.x, dL.y};

    // normal default up min::vector return and zero penetration
    min::vec2<T> normal = min::vec2<T>::up();
    T overlap = 0.0;

    T min = 1E15;
    int index = -1;
    if (penetration[0] > tolerance)
    {
        // Find the minimum, non-zero penetration index
        if (penetration[1] > tolerance && penetration[1] < penetration[0])
        {
            min = penetration[1];
            index = 1;
        }
        else
        {
            min = penetration[0];
            index = 0;
        }
    }
    else if (penetration[1] > tolerance)
    {
        min = penetration[1];
        index = 1;
    }

    // check if we found an intersection penetration
    if (index != -1)
    {
        // Calculate the sign of normal towards body1 and scale normal
        const min::vec2<T> sign = d.sign();
        normal = axes[index] * sign;
        overlap = min;
    }

    // return normal min::vector and minimum penentration
    return std::make_pair(normal, overlap);
}
// Subdividing min::vector space into 2^2 spaces using binary key location codes for index (xy)
// Most significant bit of (x - xmin)/(xmax - xmin), (y - ymin)/(ymax - ymin)
// Yields the key location code if MSB 0 = -, MSB 1 = +
// Example position 2 = 01 = left, up
//
//     /-----/-----/
//    /  1  /  3  /
//   /-----/-----/
//  /  0  /  2  /
// /-----/-----/

template <typename T>
min::vec2<T> min::vec2<T>::ratio(const min::vec2<T> &min, const min::vec2<T> &max, const min::vec2<T> &point)
{
    const T xr = (point.x - min.x) / (max.x - min.x);
    const T yr = (point.y - min.y) / (max.y - min.y);

    return min::vec2<T>(xr, yr);
}

template <typename T>
min::vec2<T> min::vec2<T>::sign() const
{
    // Get the sign of the min::vector
    const T x = sgn<T>(x);
    const T y = sgn<T>(y);

    return min::vec2<T>(x, y);
}

template <typename T>
uint_fast8_t min::vec2<T>::subdivide_key(const T middle)
{
    T x = x;
    T y = y;

    // Get the x portion of key
    uint_fast8_t key = 0;
    if (x > middle)
    {
        // Set the least significant bit
        key |= 0x1;
        x -= middle;
        x = x;
    }

    // Get the y portion of key
    key <<= 1;
    if (y > middle)
    {
        // Set the least significant bit
        key |= 0x1;
        y -= middle;
        y = y;
    }

    return key;
}

template <typename T>
std::vector<std::pair<min::vec2<T>, min::vec2<T>>> min::vec2<T>::subdivide(const min::vec2<T> &min, const min::vec2<T> &max)
{
    std::vector<std::pair<min::vec2<T>, min::vec2<T>>> out;
    out.reserve(4);

    // Half extent of min::vector space
    const min::vec2<T> h = (max - min) * 0.5;

    // Center of the min::vector space
    const min::vec2<T> c = (max + min) * 0.5;

    // Positions
    const T cx_hx = c.x - h.x;
    const T cy_hy = c.y - h.y;

    const T cxhx = c.x + h.x;
    const T cyhy = c.y + h.y;

    // Octant 0
    const min::vec2<T> min0 = min::vec2<T>(cx_hx, cy_hy);
    const min::vec2<T> max0 = min::vec2<T>(c.x, c.y);

    // Octant 1
    const min::vec2<T> min1 = min::vec2<T>(cx_hx, c.y);
    const min::vec2<T> max1 = min::vec2<T>(c.x, cyhy);

    // Octant 2
    const min::vec2<T> min2 = min::vec2<T>(c.x, cy_hy);
    const min::vec2<T> max2 = min::vec2<T>(cxhx, c.y);

    // Octant 3
    const min::vec2<T> min3 = min::vec2<T>(c.x, c.y);
    const min::vec2<T> max3 = min::vec2<T>(cxhx, cyhy);

    // Add sub spaces to out min::vector
    out.push_back(std::make_pair(min0, max0));
    out.push_back(std::make_pair(min1, max1));
    out.push_back(std::make_pair(min2, max2));
    out.push_back(std::make_pair(min3, max3));

    return out;
}

template <typename T>
std::vector<std::pair<min::vec2<T>, T>> min::vec2<T>::subdivide_center(const min::vec2<T> &min, const min::vec2<T> &max, const T size)
{
    std::vector<std::pair<min::vec2<T>, T>> out;
    out.reserve(4);

    // quarter extent of min::vector space
    const min::vec2<T> h = (max - min) * 0.25;

    // Center of the min::vector space
    const min::vec2<T> c = (max + min) * 0.5;

    // Positions
    const T cx_hx = c.x - h.x;
    const T cy_hy = c.y - h.y;

    const T cxhx = c.x + h.x;
    const T cyhy = c.y + h.y;

    // Octant 0
    const min::vec2<T> c0 = min::vec2<T>(cx_hx, cy_hy);

    // Octant 1
    const min::vec2<T> c1 = min::vec2<T>(cx_hx, cyhy);

    // Octant 2
    const min::vec2<T> c2 = min::vec2<T>(cxhx, cy_hy);

    // Octant 3
    const min::vec2<T> c3 = min::vec2<T>(cxhx, cyhy);

    // Add sub spaces to out min::vector
    out.push_back(std::make_pair(c0, size));
    out.push_back(std::make_pair(c1, size));
    out.push_back(std::make_pair(c2, size));
    out.push_back(std::make_pair(c3, size));

    return out;
}
// Plane n·x - c = 0
// Ray x = P + td
// If intersecting n · (P + td) - c = 0; x > 0.0
// n · P + n · td - c = 0
// t = (c - n · P) / (n · d)
// Each axis is axis aligned so we can simplify to, where nx = ny = 1
// tx = (cx - nx · Px) / (nx · dx)
// ty = (cy - ny · Py) / (ny · dy)

template <typename T>
void min::vec2<T>::subdivide_ray(std::vector<size_t> &out, const min::vec2<T> &min, const min::vec2<T> &max, const min::vec2<T> &origin, const min::vec2<T> &dir, const min::vec2<T> &inv_dir)
{
    // Reserve space for output
    out.clear();
    out.reserve(4);

    // Temporaries for holding the quadrants across intersecting plane, flag signals if we need to push_back
    size_t f, s;
    bool flag = false;

    // half extent of min::vector space
    const min::vec2<T> h = (max - min) * 0.5;

    // Center of the min::vector space
    const min::vec2<T> c = (max + min) * 0.5;

    // Calculate ray intersections among all axes
    const min::vec2<T> t = (c - origin) * inv_dir;
    const min::vec2<T> t_abs = min::vec2<T>(t).abs();

    const auto x_lamda = [&out, &origin, &dir, &t, &c, &h, &f, &s, &flag]() {
        // Clear the flag
        flag = false;

        // Check that we are not parallel to y-axis
        if (t.x >= 0.0 && std::abs(dir.x) >= var<T>::TOL_ZERO)
        {
            // Calculate octant ranges
            const T cy_hy = c.y - h.y;
            const T cyhy = c.y + h.y;

            // Find y value at c.x of intersection
            const T py = origin.y + t.x * dir.y;

            // Check if we are crossing between 0-2 along y-axis
            if (py > cy_hy && py < c.y)
            {
                if (dir.x < 0.0)
                {
                    f = 2;
                    s = 0;
                    flag = true;
                }
                else
                {
                    f = 0;
                    s = 2;
                    flag = true;
                }
            }
            // Check if we are crossing between 1-3 along y-axis
            else if (py >= c.y && py < cyhy)
            {
                if (dir.x < 0.0)
                {
                    f = 3;
                    s = 1;
                    flag = true;
                }
                else
                {
                    f = 1;
                    s = 3;
                    flag = true;
                }
            }
        }
    };

    const auto y_lamda = [&out, &origin, &dir, &t, &c, &h, &f, &s, &flag]() {
        // Clear the flag
        flag = false;

        // Check that we are not parallel to x-axis
        if (t.y >= 0.0 && std::abs(dir.y) >= var<T>::TOL_ZERO)
        {
            // Calculate octant ranges
            const T cx_hx = c.x - h.x;
            const T cxhx = c.x + h.x;

            // Find x value at c.y of intersection
            const T px = origin.x + t.y * dir.x;

            // Check if we are crossing between 0-1 along x-axis
            if (px > cx_hx && px < c.x)
            {
                if (dir.y < 0.0)
                {
                    f = 1;
                    s = 0;
                    flag = true;
                }
                else
                {
                    f = 0;
                    s = 1;
                    flag = true;
                }
            }
            // Check if we are crossing between 2-3 along x-axis
            else if (px >= c.x && px < cxhx)
            {
                if (dir.y < 0.0)
                {
                    f = 3;
                    s = 2;
                    flag = true;
                }
                else
                {
                    f = 2;
                    s = 3;
                    flag = true;
                }
            }
        }
    };

    // x is before y on ray
    if (t_abs.x < t_abs.y)
    {
        x_lamda();
        if (flag)
        {
            out.push_back(f);
            out.push_back(s);
        }
    }
    // y is before x on ray
    else if (t_abs.y < t_abs.x)
    {
        y_lamda();
        if (flag)
        {
            out.push_back(f);
            out.push_back(s);
        }
    }

    // x is after y on ray
    if (t_abs.x > t_abs.y)
    {
        x_lamda();
        if (flag)
        {
            out.push_back(s);
        }
    }
    // y is after x on ray
    else if (t_abs.y > t_abs.x)
    {
        y_lamda();
        if (flag)
        {
            out.push_back(s);
        }
    }
    // t.x == t.y == 0.0
    else if (t_abs.x < var<T>::TOL_ZERO && t_abs.y < var<T>::TOL_ZERO)
    {
        out = {0, 1, 2, 3};
    }

    // If we didn't hit any planes, test if ray origin is within the cell
    if (out.size() == 0 && origin.within(min, max))
    {
        // Find the quadrant the origin is in
        min::vec2<T> enter = min::vec2<T>(origin).clamp(min, max);

        // Calculate ratio between 0.0 and 1.0
        min::vec2<T> ratio = min::vec2<T>::ratio(min, max, enter);

        // Get the key from quadrant
        const uint_fast8_t key = ratio.subdivide_key(0.5);
        out.push_back(key);
    }
}

template <typename T>
void min::vec2<T>::sub_overlap(std::vector<uint_fast8_t> &out, const min::vec2<T> &min, const min::vec2<T> &max, const min::vec2<T> &center)
{
    // Reserve space for output
    out.clear();
    out.reserve(4);

    const bool minx = min.x <= center.x;
    const bool miny = min.y <= center.y;
    const bool maxx = max.x >= center.x;
    const bool maxy = max.y >= center.y;

    // If overlapping 0-1 cells
    if (minx)
    {
        // If overlapping 0
        if (miny)
        {
            out.push_back(0);
            // If overlapping 1
            if (maxy)
            {
                out.push_back(1);
            }
        }
        else
        {
            // Must be overlapping 1
            out.push_back(1);
        }
    }

    // If overlapping 2-3 cells
    if (maxx)
    {
        // If overlapping 2
        if (miny)
        {
            out.push_back(2);
            // If overlapping 3
            if (maxy)
            {
                out.push_back(3);
            }
        }
        else
        {
            // Must be overlapping 3
            out.push_back(3);
        }
    }
}

template <typename T>
constexpr T min::vec2<T>::unit_length()
{
    return var<T>::SQRT2;
}

template <typename T>
constexpr T min::vec2<T>::inverse_unit_length()
{
    return var<T>::INV_SQRT2;
}

template <typename T>
constexpr min::vec2<T> min::vec2<T>::up()
{
    return min::vec2<T>(0.0, 1.0);
}

template <typename T>
bool min::vec2<T>::within(const min::vec2<T> &min, const min::vec2<T> &max) const
{
    // Return true if this min::vector is within the min and max min::vector range
    return (x >= min.x + var<T>::TOL_REL && x <= max.x - var<T>::TOL_REL && y >= min.y + var<T>::TOL_REL && y <= max.y - var<T>::TOL_REL);
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator+=(const T a)
{
    x += a;
    y += a;
    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator+=(const min::vec2<T> &A)
{
    x += A.x;
    y += A.y;
    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator-=(const T a)
{
    x -= a;
    y -= a;
    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator-=(const min::vec2<T> &A)
{
    x -= A.x;
    y -= A.y;
    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator*=(const T a)
{
    x *= a;
    y *= a;
    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator*=(const min::vec2<T> &A)
{
    x *= A.x;
    y *= A.y;
    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator/=(const T a)
{
    x /= a;
    y /= a;
    return *this;
}

template <typename T>
min::vec2<T> &min::vec2<T>::operator/=(const min::vec2<T> &A)
{
    x /= A.x;
    y /= A.y;
    return *this;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator+(const T a) const
{
    min::vec2<T> temp = *this;
    temp += a;
    return temp;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator+(const min::vec2<T> &A) const
{
    min::vec2<T> temp = *this;
    temp += A;
    return temp;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator-(const T a) const
{
    min::vec2<T> temp = *this;
    temp -= a;
    return temp;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator-(const min::vec2<T> &A) const
{
    min::vec2<T> temp = *this;
    temp -= A;
    return temp;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator*(const T a) const
{
    min::vec2<T> temp = *this;
    temp *= a;
    return temp;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator*(const min::vec2<T> &A) const
{
    min::vec2<T> temp = *this;
    temp *= A;
    return temp;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator/(const T a) const
{
    min::vec2<T> temp = *this;
    temp /= a;
    return temp;
}

template <typename T>
min::vec2<T> min::vec2<T>::operator/(const min::vec2<T> &A) const
{
    min::vec2<T> temp = *this;
    temp /= A;
    return temp;
}

template <typename T>
bool min::vec2<T>::operator>(const min::vec2<T> &A) const
{
    return x > A.x && y > A.y;
}

template <typename T>
bool min::vec2<T>::operator>=(const min::vec2<T> &A) const
{
    return x >= A.x && y >= A.y;
}

template <typename T>
bool min::vec2<T>::operator<(const min::vec2<T> &A) const
{
    return x < A.x && y < A.y;
}

template <typename T>
bool min::vec2<T>::operator<=(const min::vec2<T> &A) const
{
    return x <= A.x && y <= A.y;
}
