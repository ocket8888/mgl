/* Copyright [2013-2016] [Aaron Springstroh, Minimal Graphics Library]

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
#ifndef __VECTOR3__
#define __VECTOR3__

// Forward declarations
namespace min
{
template <typename T, template <typename> class vec>
class coord_sys;
}

#include <algorithm>
#include <cmath>
#include <min/coord_sys.h>
#include <min/utility.h>
#include <utility>
#include <vector>

namespace min
{

template <typename T>
class vec3
{
  private:
    T _x;
    T _y;
    T _z;

  public:
    vec3() : _x(0), _y(0), _z(0) {}
    vec3(const T x, const T y, const T z) : _x(x), _y(y), _z(z) {}
    vec3(const vec2<T> &v) : _x(v.x()), _y(v.y()), _z(1.0) {}
    vec3(const vec4<T> &v) : _x(v.x()), _y(v.y()), _z(v.z()) {}
    T x() const
    {
        return _x;
    }
    inline T y() const
    {
        return _y;
    }
    inline T z() const
    {
        return _z;
    }
    inline void x(const T x)
    {
        _x = x;
    }
    inline void y(const T y)
    {
        _y = y;
    }
    inline void z(const T z)
    {
        _z = z;
    }
    inline vec3<T> &set_all(const T v)
    {
        _x = v;
        _y = v;
        _z = v;

        return *this;
    }
    inline vec3<T> &abs()
    {
        _x = std::abs(_x);
        _y = std::abs(_y);
        _z = std::abs(_z);

        return *this;
    }
    inline bool any_zero() const
    {
        return (std::abs(_x) <= 1E-6) || (std::abs(_y) <= 1E-6) || (std::abs(_z) <= 1E-6);
    }
    inline constexpr static coord_sys<T, vec3> axes()
    {
        return coord_sys<T, vec3>(vec3<T>(1.0, 0.0, 0.0), vec3<T>(0.0, 1.0, 0.0), vec3<T>(0.0, 0.0, 1.0));
    }
    inline vec3<T> &clamp(const vec3<T> &min, const vec3<T> &max)
    {
        min::clamp(_x, min.x(), max.x());
        min::clamp(_y, min.y(), max.y());
        min::clamp(_z, min.z(), max.z());

        return *this;
    }
    inline vec3<T> clamp_direction(const vec3<T> &min, const vec3<T> &max)
    {
        T x = min::clamp_direction(_x, min.x(), max.x());
        T y = min::clamp_direction(_y, min.y(), max.y());
        T z = min::clamp_direction(_z, min.z(), max.z());

        return vec3<T>(x, y, z);
    }
    inline vec3<T> cross(const vec3<T> &A) const
    {
        T x = _y * A.z() - _z * A.y();
        T y = _z * A.x() - _x * A.z();
        T z = _x * A.y() - _y * A.x();
        return vec3<T>(x, y, z);
    }
    inline vec3<T> cross_x() const
    {
        return vec3<T>(0, _z, -_y);
    }
    inline vec3<T> cross_y() const
    {
        return vec3<T>(-_z, 0, _x);
    }
    inline vec3<T> cross_z() const
    {
        return vec3<T>(_y, -_x, 0);
    }
    inline T dot(const vec3<T> &A) const
    {
        return _x * A.x() + _y * A.y() + _z * A.z();
    }
    inline T dot_x() const
    {
        return _x;
    }
    inline T dot_y() const
    {
        return _y;
    }
    inline T dot_z() const
    {
        return _z;
    }
    // Creating N^2 grid from min corner to max corner
    // Grid iterates in the Y than X dimension to reflect tree data layout
    // ^     /-----/-----/
    // |    /  1  /  3  /
    // c   /-----/-----/
    // o  /  0  /  2  /
    // l /-----/-----/
    //   row --->
    inline static std::vector<std::pair<vec3<T>, vec3<T>>> grid(const vec3<T> &min, const vec3<T> &max, const size_t scale)
    {
        // Create out vector
        std::vector<std::pair<vec3<T>, vec3<T>>> out;
        out.reserve(scale * scale * scale);

        // Calculate the grid dimensions
        const vec3<T> size = max - min;
        const vec3<T> extent = size / scale;
        const T dx = extent.x();
        const T dy = extent.y();
        const T dz = extent.z();

        // Start at the bottom left corner
        vec3<T> cell;

        // Across the X dim
        for (T x = min.x(); x < max.x(); x += dx)
        {
            // Set the cell x value
            cell.x(x);

            // Across the Y dim
            for (T y = min.y(); y < max.y(); y += dy)
            {
                // Set the y value
                cell.y(y);

                // Across the Z dim
                for (T z = min.z(); z < max.z(); z += dz)
                {
                    // Set the cell z value
                    cell.z(z);
                    out.emplace_back(std::make_pair(cell, cell + extent));
                }
            }
        }

        // return the compute grid
        return out;
    }
    inline static std::vector<std::pair<vec3<T>, T>> grid_center(const vec3<T> &min, const vec3<T> &max, const size_t scale, const T size)
    {
        // Create out vector
        std::vector<std::pair<vec3<T>, T>> out;
        out.reserve(scale * scale * scale);

        // Calculate the grid dimensions
        const vec3<T> extent = (max - min) / scale;
        const vec3<T> half_extent = extent * 0.5;
        const T dx = extent.x();
        const T dy = extent.y();
        const T dz = extent.z();

        // Start at the bottom left corner
        vec3<T> cell;

        // Across the X dim
        for (T x = min.x(); x < max.x(); x += dx)
        {
            // Set the cell x value
            cell.x(x);

            // Across the Y dim
            for (T y = min.y(); y < max.y(); y += dy)
            {
                // Set the y value
                cell.y(y);

                // Across the Z dim
                for (T z = min.z(); z < max.z(); z += dz)
                {
                    // Set the cell z value
                    cell.z(z);
                    out.emplace_back(std::make_pair(cell + half_extent, size));
                }
            }
        }

        // return the compute grid
        return out;
    }
    inline static size_t grid_key(const vec3<T> &min, const vec3<T> &extent, const size_t scale, const vec3<T> &point)
    {
        // Calculate the grid dimensions
        const T dx = extent.x();
        const T dy = extent.y();
        const T dz = extent.z();

        const size_t row = (point.x() - min.x()) / dx;
        const size_t col = (point.y() - min.y()) / dy;
        const size_t zin = (point.z() - min.z()) / dz;

        return row * scale * scale + col * scale + zin;
    }
    inline static std::vector<size_t> grid_overlap(const vec3<T> &min, const vec3<T> &extent, const size_t scale, const vec3<T> &b_min, const vec3<T> &b_max)
    {
        // Create the output vector
        std::vector<size_t> out;
        out.reserve(27);

        // Calculate the grid dimensions
        const T dx = extent.x();
        const T dy = extent.y();
        const T dz = extent.z();

        // Calculate the center cell
        const vec3<T> center = (b_min + b_max) * 0.5;

        // Test for early out
        if (center.x() < min.x() || center.y() < min.y() || center.z() < min.z())
        {
            return out;
        }

        // Center cell indices
        const size_t x = (center.x() - min.x()) / dx;
        const size_t y = (center.y() - min.y()) / dy;
        const size_t z = (center.z() - min.z()) / dz;
        const size_t scale2 = scale * scale;

        // Bounds of the center cell
        const T minx = min.x() + dx * x;
        const T miny = min.y() + dy * y;
        const T minz = min.z() + dz * z;
        const T maxx = minx + dx;
        const T maxy = miny + dy;
        const T maxz = minz + dz;

        // Calculate the neighboring indices
        const int nx = x - 1;
        const int px = x + 1;
        const int ny = y - 1;
        const int py = y + 1;
        const int nz = z - 1;
        const int pz = z + 1;

        // Calculate whether neighbors are outside the main grid
        bool nxg = nx >= 0;
        bool nyg = ny >= 0;
        bool nzg = nz >= 0;
        bool pxg = px < (long)scale;
        bool pyg = py < (long)scale;
        bool pzg = pz < (long)scale;
        bool lx = b_min.x() < minx;
        bool ly = b_min.y() < miny;
        bool lz = b_min.z() < minz;
        bool gx = b_max.x() >= maxx;
        bool gy = b_max.y() >= maxy;
        bool gz = b_max.z() >= maxz;
        bool tny = ly && nyg;
        bool tgy = gy && pyg;
        bool tnz = lz && nzg;
        bool tgz = gz && pzg;

        // -X
        if (lx && nxg)
        {
            // -Y
            if (tny)
            {
                if (tnz)
                    out.push_back(nx * scale2 + ny * scale + nz); // -Z
                out.push_back(nx * scale2 + ny * scale + z);      // Z
                if (tgz)
                    out.push_back(nx * scale2 + ny * scale + pz); // +Z
            }

            // Y
            if (tnz)
                out.push_back(nx * scale2 + y * scale + nz); // -Z
            out.push_back(nx * scale2 + y * scale + z);      // Z
            if (tgz)
                out.push_back(nx * scale2 + y * scale + pz); // +Z

            // +Y
            if (tgy)
            {
                if (tnz)
                    out.push_back(nx * scale2 + py * scale + nz); // -Z
                out.push_back(nx * scale2 + py * scale + z);      // Z
                if (tgz)
                    out.push_back(nx * scale2 + py * scale + pz); // +Z
            }
        }

        // X -Y
        if (tny)
        {
            if (tnz)
                out.push_back(x * scale2 + ny * scale + nz); // -Z
            out.push_back(x * scale2 + ny * scale + z);      // Z
            if (tgz)
                out.push_back(x * scale2 + ny * scale + pz); // +Z
        }

        // X Y
        if (tnz)
            out.push_back(x * scale2 + y * scale + nz); // -Z
        out.push_back(x * scale2 + y * scale + z);      // Z
        if (tgz)
            out.push_back(x * scale2 + y * scale + pz); // +Z

        // X +Y
        if (tgy)
        {
            if (tnz)
                out.push_back(x * scale2 + py * scale + nz); // -Z
            out.push_back(x * scale2 + py * scale + z);      // Z
            if (tgz)
                out.push_back(x * scale2 + py * scale + pz); // +Z
        }

        // +X
        if (gx && pxg)
        {
            // -Y
            if (tny)
            {
                if (tnz)
                    out.push_back(px * scale2 + ny * scale + nz); // -Z
                out.push_back(px * scale2 + ny * scale + z);      // Z
                if (tgz)
                    out.push_back(px * scale2 + ny * scale + pz); // +Z
            }

            // Y
            if (tnz)
                out.push_back(px * scale2 + y * scale + nz); // -Z
            out.push_back(px * scale2 + y * scale + z);      // Z
            if (tgz)
                out.push_back(px * scale2 + y * scale + pz); // +Z

            // +Y
            if (tgy)
            {
                if (tnz)
                    out.push_back(px * scale2 + py * scale + nz); // -Z
                out.push_back(px * scale2 + py * scale + z);      // Z
                if (tgz)
                    out.push_back(px * scale2 + py * scale + pz); // +Z
            }
        }

        return out;
    }
    inline static std::pair<vec3<T>, vec3<T>> extents(const std::vector<vec3<T>> &verts)
    {
        // Find the greatest extents in the collection of vec3<T>
        const auto size = verts.size();
        if (size > 1)
        {
            const vec3<T> &first = verts[0];
            T minx = first.x();
            T miny = first.y();
            T minz = first.z();
            T maxx = first.x();
            T maxy = first.y();
            T maxz = first.z();
            for (size_t i = 1; i < size; i++)
            {
                const vec3<T> &v = verts[i];
                extend<T>(v.x(), minx, maxx);
                extend<T>(v.y(), miny, maxy);
                extend<T>(v.z(), minz, maxz);
            }

            // Return the greatest extents
            return std::make_pair(vec3<T>(minx, miny, minz), vec3<T>(maxx, maxy, maxz));
        }

        return std::make_pair(vec3<T>(), vec3<T>());
    }
    inline bool inside(const vec3<T> &min, const vec3<T> &max) const
    {
        // Return true if this vector is inside the min and max vector range
        return (_x > min.x() && _x < max.x() && _y > min.y() && _y < max.y() && _z > min.z() && _z < max.z());
    }
    inline vec3<T> inverse() const
    {
        return vec3<T>(1.0 / _x, 1.0 / _y, 1.0 / _z);
    }
    inline static vec3<T> lerp(const vec3<T> &v0, const vec3<T> &v1, T t)
    {
        return (v0 + (v1 - v0) * (t));
    }
    inline static vec3<T> interpolate(const vec3<T> &v0, const vec3<T> &v1, T t)
    {
        return lerp(v0, v1, t);
    }
    inline T magnitude() const
    {
        return sqrt(_x * _x + _y * _y + _z * _z);
    }
    inline T max() const
    {
        return std::max(std::max(_x, _y), _z);
    }
    inline T min() const
    {
        return std::min(std::min(_x, _y), _z);
    }
    inline static std::pair<vec3<T>, vec3<T>> most_separating(const std::vector<vec3<T>> &verts)
    {
        // Find the two most separating points in the collection of vec3<T>
        const auto size = verts.size();
        if (size > 1)
        {
            size_t minx, maxx, miny, maxy, minz, maxz, min, max;
            minx = maxx = miny = maxy = minz = maxz = min = max = 0;
            for (size_t i = 0; i < size; i++)
            {
                const vec3<T> &v = verts[i];
                if (v.x() > verts[maxx].x())
                    maxx = i;
                if (v.x() < verts[minx].x())
                    minx = i;
                if (v.y() > verts[maxy].y())
                    maxy = i;
                if (v.y() < verts[miny].y())
                    miny = i;
                if (v.z() > verts[maxz].z())
                    maxz = i;
                if (v.z() < verts[minz].z())
                    minz = i;
            }

            // Calculate span of the extents
            const vec3<T> dx = verts[maxx] - verts[minx];
            const vec3<T> dy = verts[maxy] - verts[miny];
            const vec3<T> dz = verts[maxz] - verts[minz];

            // Calculated the squared euclidean distance
            const T dx2 = dx.dot(dx);
            const T dy2 = dy.dot(dy);
            const T dz2 = dz.dot(dz);

            // Calculate the max extents
            min = minx;
            max = maxx;
            if (dy2 > dx2 && dy2 > dz2)
            {
                min = miny;
                max = maxy;
            }
            if (dz2 > dx2 && dz2 > dy2)
            {
                min = minz;
                max = maxz;
            }

            // Return the two most separating verts
            return std::make_pair(verts[min], verts[max]);
        }

        return std::make_pair(vec3<T>(), vec3<T>());
    }
    inline static vec3<T> normal(const vec3<T> &a, const vec3<T> &b, const vec3<T> &c)
    {
        // Used in plane to compute normal vector to three points
        // Normal is not normalized
        return (b - a).cross(c - a);
    }
    inline vec3<T> &normalize()
    {
        T mag = 1.0 / magnitude();
        _x *= mag;
        _y *= mag;
        _z *= mag;

        return *this;
    }
    inline static void order(vec3<T> &min, vec3<T> &max)
    {
        // Order the components into min and max
        if (min._x > max._x)
        {
            swap(min._x, max._x);
        }
        if (min._y > max._y)
        {
            swap(min._y, max._y);
        }
        if (min._z > max._z)
        {
            swap(min._z, max._z);
        }
    }
    inline vec3<T> orthogonal() const
    {
        const T x2 = _x * _x;
        const T y2 = _y * _y;
        const T z2 = _z * _z;
        const T ratio = x2 / (x2 + y2 + z2);

        // If parallel to x axis, switch to y axis
        if (ratio > 0.95)
        {
            return cross_y();
        }

        // Compute cross product around x axis
        return cross_x();
    }
    inline vec3<T> project_point(const coord_sys<T, vec3> &axis, const vec3<T> &extent)
    {
        // Project v onto local x axis
        T x = this->dot(axis.x());

        // Clamp d onto the box half extent
        min::clamp<T>(x, -extent.x(), extent.x());

        // Project v onto local y axis
        T y = this->dot(axis.y());

        // Clamp d onto the box half extent
        min::clamp<T>(y, -extent.y(), extent.y());

        // Project v onto local z axis
        T z = this->dot(axis.z());

        // Clamp d onto the box half extent
        min::clamp<T>(z, -extent.z(), extent.z());

        // Compute the point along this axis
        return (axis.x() * x) + (axis.y() * y) + (axis.z() * z);
    }
    inline T project_length(const coord_sys<T, vec3> &axis, const vec3<T> &extent)
    {
        // Project this onto local x axis
        const T x = this->dot(axis.x());

        // Clamp x onto the box half extent, else zero
        const T dx = clamp_value<T>(x, -extent.x(), x + extent.x(), extent.x(), x - extent.x());

        // Project this onto local y axis
        const T y = this->dot(axis.y());

        // Clamp y onto the box half extent, else 0
        const T dy = clamp_value<T>(y, -extent.y(), y + extent.y(), extent.y(), y - extent.y());

        // Project this onto local y axis
        const T z = this->dot(axis.z());

        // Clamp y onto the box half extent, else 0
        const T dz = clamp_value<T>(z, -extent.z(), z + extent.z(), extent.z(), z - extent.z());

        // Compute the square distance from this point
        return (dx * dx) + (dy * dy) + (dz * dz);
    }
    static inline bool project_sat(const coord_sys<T, vec3> &axis1, const vec3<T> &center1, const vec3<T> &extent1, const coord_sys<T, vec3> &axis2, const vec3<T> &center2, const vec3<T> &extent2)
    {
        // This performs the separating axis theorem for checking oobb-oobb intersections
        // For every axis test (C2-C1).dot(axis_n) > (a.get_extent() + b.get_extent()).dot(axis_n)
        // This means testing the difference between box centers, C1 & C2, along the separating axis L
        // With the addition of box extents along this same axis L
        // For 3D, there are 15 axes that need to be tested against...
        // 2x3=6 local box axes plus and 3x3=9 axes perpendicular to the 6 local box axes

        // Rotation matrix expressing A2 in A1's coordinate frame
        // Even though dot product is always > 0, if the dot product is zero, 0 > -0 may skew results
        const T xx = std::abs(axis1.x().dot(axis2.x()));
        const T xy = std::abs(axis1.x().dot(axis2.y()));
        const T xz = std::abs(axis1.x().dot(axis2.z()));
        const T yx = std::abs(axis1.y().dot(axis2.x()));
        const T yy = std::abs(axis1.y().dot(axis2.y()));
        const T yz = std::abs(axis1.y().dot(axis2.z()));
        const T zx = std::abs(axis1.z().dot(axis2.x()));
        const T zy = std::abs(axis1.z().dot(axis2.y()));
        const T zz = std::abs(axis1.z().dot(axis2.z()));

        // Bring translation into A1's coordinate frame
        const vec3<T> d = center2 - center1;
        const vec3<T> t = vec3<T>(d.dot(axis1.x()), d.dot(axis1.y()), d.dot(axis1.z())).abs();

        // Test L = A1.x(); d1 and d2 is the length of extents along L
        T dL1 = extent1.x();
        T dL2 = extent2.x() * xx + extent2.y() * xy + extent2.z() * xz;
        if (t.x() > dL1 + dL2)
            return false;

        // Test L = A1.y(); d1 and d2 is the length of extents along L
        dL1 = extent1.y();
        dL2 = extent2.x() * yx + extent2.y() * yy + extent2.z() * yz;
        if (t.y() > dL1 + dL2)
            return false;

        // Test L = A1.z(); d1 and d2 is the length of extents along L
        dL1 = extent1.z();
        dL2 = extent2.x() * zx + extent2.y() * zy + extent2.z() * zz;
        if (t.z() > dL1 + dL2)
            return false;

        // Test L = A2.x(); d1 and d2 is the length of extents along L
        dL1 = extent1.x() * xx + extent1.y() * yx + extent1.z() * zx;
        dL2 = extent2.x();
        if (t.x() * xx + t.y() * yx + t.z() * zx > dL1 + dL2)
            return false;

        // Test L = A2.y(); d1 and d2 is the length of extents along L
        dL1 = extent1.x() * xy + extent1.y() * yy + extent1.z() * zy;
        dL2 = extent2.y();
        if (t.x() * xy + t.y() * yy + t.z() * zy > dL1 + dL2)
            return false;

        // Test L = A2.z(); d1 and d2 is the length of extents along L
        dL1 = extent1.x() * xz + extent1.y() * yz + extent1.z() * zz;
        dL2 = extent2.z();
        if (t.x() * xz + t.y() * yz + t.z() * zz > dL1 + dL2)
            return false;

        // Test axis L = A1.x() X A2.x()
        dL1 = extent1.y() * zx + extent1.z() * yx;
        dL2 = extent2.y() * xz + extent2.z() * xy;
        if (t.z() * yx - t.y() * zx > dL1 + dL2)
            return false;

        // Test axis L = A1.x() X A2.y()
        dL1 = extent1.y() * zy + extent1.z() * yy;
        dL2 = extent2.x() * xz + extent2.z() * xx;
        if (t.z() * yy - t.y() * zy > dL1 + dL2)
            return false;

        // Test axis L = A1.x() X A2.z()
        dL1 = extent1.y() * zz + extent1.z() * yz;
        dL2 = extent2.x() * xy + extent2.y() * xx;
        if (t.z() * yz - t.y() * zz > dL1 + dL2)
            return false;

        // Test axis L = A1.y() X A2.x()
        dL1 = extent1.x() * zx + extent1.z() * xx;
        dL2 = extent2.y() * yz + extent2.z() * yy;
        if (t.x() * zx - t.z() * xx > dL1 + dL2)
            return false;

        // Test axis L = A1.y() X A2.y()
        dL1 = extent1.x() * zy + extent1.z() * xy;
        dL2 = extent2.x() * yz + extent2.z() * yx;
        if (t.x() * zy - t.z() * xy > dL1 + dL2)
            return false;

        // Test axis L = A1.y() X A2.z()
        dL1 = extent1.x() * zz + extent1.z() * xz;
        dL2 = extent2.x() * yy + extent2.y() * yx;
        if (t.x() * zz - t.z() * xz > dL1 + dL2)
            return false;

        // Test axis L = A1.z() X A2.x()
        dL1 = extent1.x() * yx + extent1.y() * xx;
        dL2 = extent2.y() * zz + extent2.z() * zy;
        if (t.y() * xx - t.x() * yx > dL1 + dL2)
            return false;

        // Test axis L = A1.z() X A2.y()
        dL1 = extent1.x() * yy + extent1.y() * xy;
        dL2 = extent2.x() * zz + extent2.z() * zx;
        if (t.y() * xy - t.x() * yy > dL1 + dL2)
            return false;

        // Test axis L = A1.z() X A2.z()
        dL1 = extent1.x() * yz + extent1.y() * xz;
        dL2 = extent2.x() * zy + extent2.y() * zx;
        if (t.y() * xz - t.x() * yz > dL1 + dL2)
            return false;

        return true;
    }
    // Subdividing vector space into 2^3 spaces using binary key location codes for index (xyz)
    // Most significant bit of (x - xmin)/(xmax - xmin), (y - ymin)/(ymax - ymin), (z - zmin)/(zmax - zmin)
    // Yields the key location code if MSB 0 = -, MSB 1 = +
    // Example position 3 = 011 = left, up, forward
    //
    //     /-----/-----/
    //    /  3  /  7  /
    //   /-----/-----/
    //  /  2  /  6  /
    // /-----/-----/
    //     /-----/-----/
    //    /  1  /  5  /
    //   /-----/-----/
    //  /  0  /  4  /
    // /-----/-----/
    inline static vec3<T> ratio(const vec3<T> &min, const vec3<T> &max, const vec3<T> &point)
    {
        T xr = (point.x() - min.x()) / (max.x() - min.x());
        T yr = (point.y() - min.y()) / (max.y() - min.y());
        T zr = (point.z() - min.z()) / (max.z() - min.z());

        return vec3<T>(xr, yr, zr);
    }
    inline uint8_t subdivide_key(const T middle)
    {
        T x = _x;
        T y = _y;
        T z = _z;

        // Get the x portion of key
        uint8_t key = 0;
        if (x > middle)
        {
            // Set the least significant bit
            key |= 0x1;
            x -= middle;
            _x = x;
        }

        // Get the y portion of key
        key <<= 1;
        if (y > middle)
        {
            // Set the least significant bit
            key |= 0x1;
            y -= middle;
            _y = y;
        }

        // Get the z portion of key
        key <<= 1;
        if (z > middle)
        {
            // Set the least significant bit
            key |= 0x1;
            z -= middle;
            _z = z;
        }

        return key;
    }
    inline static std::vector<std::pair<vec3<T>, vec3<T>>> subdivide(const vec3<T> &min, const vec3<T> &max)
    {
        std::vector<std::pair<vec3<T>, vec3<T>>> out;
        out.reserve(8);

        // Half extent of vector space
        vec3<T> h = (max - min) * 0.5;

        // Center of the vector space
        vec3<T> c = (max + min) * 0.5;

        // Positions
        T cx_hx = c.x() - h.x();
        T cy_hy = c.y() - h.y();
        T cz_hz = c.z() - h.z();

        T cxhx = c.x() + h.x();
        T cyhy = c.y() + h.y();
        T czhz = c.z() + h.z();

        // Octant 0
        vec3<T> min0 = vec3<T>(cx_hx, cy_hy, cz_hz);
        vec3<T> max0 = vec3<T>(c.x(), c.y(), c.z());

        // Octant 1
        vec3<T> min1 = vec3<T>(cx_hx, cy_hy, c.z());
        vec3<T> max1 = vec3<T>(c.x(), c.y(), czhz);

        // Octant 2
        vec3<T> min2 = vec3<T>(cx_hx, c.y(), cz_hz);
        vec3<T> max2 = vec3<T>(c.x(), cyhy, c.z());

        // Octant 3
        vec3<T> min3 = vec3<T>(cx_hx, c.y(), c.z());
        vec3<T> max3 = vec3<T>(c.x(), cyhy, czhz);

        // Octant 4
        vec3<T> min4 = vec3<T>(c.x(), cy_hy, cz_hz);
        vec3<T> max4 = vec3<T>(cxhx, c.y(), c.z());

        // Octant 5
        vec3<T> min5 = vec3<T>(c.x(), cy_hy, c.z());
        vec3<T> max5 = vec3<T>(cxhx, c.y(), czhz);

        // Octant 6
        vec3<T> min6 = vec3<T>(c.x(), c.y(), cz_hz);
        vec3<T> max6 = vec3<T>(cxhx, cyhy, c.z());

        // Octant 7
        vec3<T> min7 = vec3<T>(c.x(), c.y(), c.z());
        vec3<T> max7 = vec3<T>(cxhx, cyhy, czhz);

        // Add sub spaces to out vector
        out.push_back(std::make_pair(min0, max0));
        out.push_back(std::make_pair(min1, max1));
        out.push_back(std::make_pair(min2, max2));
        out.push_back(std::make_pair(min3, max3));
        out.push_back(std::make_pair(min4, max4));
        out.push_back(std::make_pair(min5, max5));
        out.push_back(std::make_pair(min6, max6));
        out.push_back(std::make_pair(min7, max7));

        return out;
    }
    inline static std::vector<std::pair<vec3<T>, T>> subdivide_center(const vec3<T> &min, const vec3<T> &max, const T size)
    {
        std::vector<std::pair<vec3<T>, T>> out;
        out.reserve(8);

        // Quarter extent of vector space
        vec3<T> h = (max - min) * 0.25;

        // Center of the vector space
        vec3<T> c = (max + min) * 0.5;

        // Positions
        T cx_hx = c.x() - h.x();
        T cy_hy = c.y() - h.y();
        T cz_hz = c.z() - h.z();

        T cxhx = c.x() + h.x();
        T cyhy = c.y() + h.y();
        T czhz = c.z() + h.z();

        // Octant 0
        vec3<T> c0 = vec3<T>(cx_hx, cy_hy, cz_hz);

        // Octant 1
        vec3<T> c1 = vec3<T>(cx_hx, cy_hy, czhz);

        // Octant 2
        vec3<T> c2 = vec3<T>(cx_hx, cyhy, cz_hz);

        // Octant 3
        vec3<T> c3 = vec3<T>(cx_hx, cyhy, czhz);

        // Octant 4
        vec3<T> c4 = vec3<T>(cxhx, cy_hy, cz_hz);

        // Octant 5
        vec3<T> c5 = vec3<T>(cxhx, cy_hy, czhz);

        // Octant 6
        vec3<T> c6 = vec3<T>(cxhx, cyhy, cz_hz);

        // Octant 7
        vec3<T> c7 = vec3<T>(cxhx, cyhy, czhz);

        // Add sub spaces to out vector
        out.push_back(std::make_pair(c0, size));
        out.push_back(std::make_pair(c1, size));
        out.push_back(std::make_pair(c2, size));
        out.push_back(std::make_pair(c3, size));
        out.push_back(std::make_pair(c4, size));
        out.push_back(std::make_pair(c5, size));
        out.push_back(std::make_pair(c6, size));
        out.push_back(std::make_pair(c7, size));

        return out;
    }
    inline static std::vector<uint8_t> sub_overlap(const vec3<T> &min, const vec3<T> &max, const vec3<T> &center)
    {
        std::vector<uint8_t> out;
        out.reserve(8);

        bool minx = min.x() < center.x();
        bool miny = min.y() < center.y();
        bool minz = min.z() < center.z();
        bool maxx = max.x() > center.x();
        bool maxy = max.y() > center.y();
        bool maxz = max.z() > center.z();

        // If overlapping 0,1,2,3 cells
        if (minx)
        {
            // If overlapping 0,1
            if (miny)
            {
                // If overlapping 0
                if (minz)
                {
                    out.push_back(0);
                    // If overlapping 1
                    if (maxz)
                    {
                        out.push_back(1);
                    }
                }
                else
                {
                    // Must be overlapping 1
                    out.push_back(1);
                }

                // If overlapping 2,3
                if (maxy)
                {
                    if (minz)
                    {
                        out.push_back(2);
                        // If overlapping 3
                        if (maxz)
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
            else
            {
                if (minz)
                {
                    out.push_back(2);
                    // If overlapping 3
                    if (maxz)
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

        // If overlapping 4,5,6,7 cells
        if (maxx)
        {
            // If overlapping 4,5
            if (miny)
            {
                // If overlapping 4
                if (minz)
                {
                    out.push_back(4);
                    // If overlapping 5
                    if (maxz)
                    {
                        out.push_back(5);
                    }
                }
                else
                {
                    // Must be overlapping 1
                    out.push_back(5);
                }

                // If overlapping 6,7
                if (maxy)
                {
                    if (minz)
                    {
                        out.push_back(6);
                        // If overlapping 7
                        if (maxz)
                        {
                            out.push_back(7);
                        }
                    }
                    else
                    {
                        // Must be overlapping 7
                        out.push_back(7);
                    }
                }
            }
            else
            {
                if (minz)
                {
                    out.push_back(6);
                    // If overlapping 7
                    if (maxz)
                    {
                        out.push_back(7);
                    }
                }
                else
                {
                    // Must be overlapping 7
                    out.push_back(7);
                }
            }
        }

        return out;
    }
    inline constexpr static T unit_length()
    {
        return var<T>::SQRT3;
    }
    inline constexpr static T inverse_unit_length()
    {
        return var<T>::INV_SQRT3;
    }
    inline constexpr static vec3<T> up()
    {
        return vec3<T>(0.0, 1.0, 0.0);
    }
    inline bool within(const vec3<T> &min, const vec3<T> &max) const
    {
        // Return true if this vector is within the min and max vector range
        return (_x >= min.x() && _x <= max.x() && _y >= min.y() && _y <= max.y() && _z >= min.z() && _z <= max.z());
    }
    inline vec3<T> &operator+=(const T a)
    {
        _x += a;
        _y += a;
        _z += a;
        return *this;
    }
    inline vec3<T> &operator+=(const vec3<T> &A)
    {
        _x += A.x();
        _y += A.y();
        _z += A.z();
        return *this;
    }
    inline vec3<T> &operator-=(const T a)
    {
        _x -= a;
        _y -= a;
        _z -= a;
        return *this;
    }
    inline vec3<T> &operator-=(const vec3<T> &A)
    {
        _x -= A.x();
        _y -= A.y();
        _z -= A.z();
        return *this;
    }
    inline vec3<T> &operator*=(const T a)
    {
        _x *= a;
        _y *= a;
        _z *= a;
        return *this;
    }
    inline vec3<T> &operator*=(const vec3<T> &A)
    {
        _x *= A.x();
        _y *= A.y();
        _z *= A.z();
        return *this;
    }
    inline vec3<T> &operator/=(const T a)
    {
        _x /= a;
        _y /= a;
        _z /= a;
        return *this;
    }
    inline vec3<T> &operator/=(const vec3<T> &A)
    {
        _x /= A.x();
        _y /= A.y();
        _z /= A.z();
        return *this;
    }
    inline vec3<T> operator+(const T a) const
    {
        vec3<T> temp = *this;
        temp += a;
        return temp;
    }
    inline vec3<T> operator+(const vec3<T> &A) const
    {
        vec3<T> temp = *this;
        temp += A;
        return temp;
    }
    inline vec3<T> operator-(const T a) const
    {
        vec3<T> temp = *this;
        temp -= a;
        return temp;
    }
    inline vec3<T> operator-(const vec3<T> &A) const
    {
        vec3<T> temp = *this;
        temp -= A;
        return temp;
    }
    inline vec3<T> operator*(const T a) const
    {
        vec3<T> temp = *this;
        temp *= a;
        return temp;
    }
    inline vec3<T> operator*(const vec3<T> &A) const
    {
        vec3<T> temp = *this;
        temp *= A;
        return temp;
    }
    inline vec3<T> operator/(const T a) const
    {
        vec3<T> temp = *this;
        temp /= a;
        return temp;
    }
    inline vec3<T> operator/(const vec3<T> &A) const
    {
        vec3<T> temp = *this;
        temp /= A;
        return temp;
    }
    inline bool operator>(const vec3<T> &A) const
    {
        return _x > A._x && _y > A._y && _z > A._z;
    }
    inline bool operator>=(const vec3<T> &A) const
    {
        return _x >= A._x && _y >= A._y && _z >= A._z;
    }
    inline bool operator<(const vec3<T> &A) const
    {
        return _x < A._x && _y < A._y && _z < A._z;
    }
    inline bool operator<=(const vec3<T> &A) const
    {
        return _x <= A._x && _y <= A._y && _z <= A._z;
    }

  private:
};
}

#endif
