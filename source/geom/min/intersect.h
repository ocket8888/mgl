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
#ifndef INTERSECT
#define INTERSECT

#include <cmath>

#include "aabbox.h"
#include "frustum.h"
#include "oobbox.h"
#include "ray.h"
#include "sphere.h"

namespace min
{

// Solves the quadratic equation for sphere ray intersection
// ray: o + t*dir, sphere center: s.c; (ray-sphere).(ray-sphere) = r^2
// d = o - s.c
// (o + t*dir - s.c)·(o + t*dir - s.c) = r^2
// (d + t*dir)·(d + t*dir) = r^2
// (dir·dir)t^2 + 2*(d·dir)t + d·d - r^2 = 0

// Thus if (dir.dir) == 1.0 ...
// t^2 + 2(d·dir)t + (d·d) − r^2 = 0
// t = -b +/- sqrt(b^2 -c)
// b = d·dir
// c = d·d - r2
template <typename T, template <typename> class vec>
bool intersect(const sphere<T, vec>&, const ray<T, vec>&, vec<T>&);

// Solves the algebraic equation of a ray intersecting N-planes, where N is the dim of vec
// ray: o + t*dir = x
// plane: n · x - d = 0;
// substitude: n · (o + t*dir) - d = 0;
// <tx, ty, tz> = <(d − o · nx)/(dir · nx), (d − o · ny)/(dir · ny), (d − o · nz)/(dir · nz)>
// Since the box is an aabb, the dot product can be simplified
// <tx, ty, tz> = (d - o) / dir

template <typename T, template <typename> class vec>
bool intersect(const aabbox<T, vec>&, const ray<T, vec>&, vec<T>&);

template <typename T, template <typename> class vec>
bool intersect(const oobbox<T, vec>&, const ray<T, vec>&, vec<T>&);

/////////////////////////////////////////////////////////////////////////////////////
// SPHERE-SPHERE
/////////////////////////////////////////////////////////////////////////////////////

// tests if s1 overlaps with s2 by comparing the squared distance between centers
// with the squared sum of radiuses
template<typename T, template <typename> class vec>
bool intersect(const sphere<T, vec>&, const sphere<T, vec>&);

// Calculates the closest point on s2 to s1's center
// tests if this point is inside s1
template <typename T, template <typename> class vec>
bool intersect(const sphere<T, vec>&, const sphere<T, vec>&, vec<T>&);

/////////////////////////////////////////////////////////////////////////////////////
// SPHERE-AABB
/////////////////////////////////////////////////////////////////////////////////////

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const sphere<T, vec>&, const aabbox<T, vec>&);

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const aabbox<T, vec>&, const sphere<T, vec>&);

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const sphere<T, vec>&, const aabbox<T, vec>&, vec<T>&);

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const aabbox<T, vec>&, const sphere<T, vec>&, vec<T>&);

/////////////////////////////////////////////////////////////////////////////////////
// SPHERE-OOBB
/////////////////////////////////////////////////////////////////////////////////////

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const sphere<T, vec>&, const oobbox<T, vec>&);

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const oobbox<T, vec>&, const sphere<T, vec>&);

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const sphere<T, vec>&, const oobbox<T, vec>&, vec<T>&);

// Calculates the closest point on box to s's center
// tests if this point is inside s
template <typename T, template <typename> class vec>
bool intersect(const oobbox<T, vec>&, const sphere<T, vec>&, vec<T>&);

/////////////////////////////////////////////////////////////////////////////////////
// AABB
/////////////////////////////////////////////////////////////////////////////////////

// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const aabbox<T, vec>&, const aabbox<T, vec>&);

// Calculates the closest point on box2 to box1's center
// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const aabbox<T, vec>&, const aabbox<T, vec>&, vec<T>&);

/////////////////////////////////////////////////////////////////////////////////////
// OOBB
/////////////////////////////////////////////////////////////////////////////////////

// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const oobbox<T, vec>&, const oobbox<T, vec>&);

// Calculates the closest point on box2 to box1's center
// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const oobbox<T, vec>&, const oobbox<T, vec>&, vec<T>&);

/////////////////////////////////////////////////////////////////////////////////////
// OOBB-AABB
/////////////////////////////////////////////////////////////////////////////////////

// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const aabbox<T, vec>&, const oobbox<T, vec>&);

// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const oobbox<T, vec>&, const aabbox<T, vec>&);

// Calculates the closest point on box2 to box1's center
// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const aabbox<T, vec>&, const oobbox<T, vec>&, vec<T>&);

// Calculates the closest point on box2 to box1's center
// tests if box1 intersects with box2
template <typename T, template <typename> class vec>
bool intersect(const oobbox<T, vec>&, const aabbox<T, vec>&, vec<T>&);

/////////////////////////////////////////////////////////////////////////////////////
// FRUSTUM
/////////////////////////////////////////////////////////////////////////////////////

// Tests if the sphere intersects the frustum
template <typename T>
bool intersect(const frustum<T>&, const sphere<T, vec3>&);

// Calculates the closest point on sphere to frustum center
// Tests if the closest point is inside the frustum
template <typename T>
bool intersect(const frustum<T>&, const sphere<T, vec3>&, vec3<T>&);

// Tests if the frustum intersects the aabb
// For each plane in frustum determine if the box axis intersects the frustum plane
template <typename T>
bool intersect(const frustum<T>&, const aabbox<T, vec3>&);

// Calculates the closest point on aabb to frustum center
// Tests if the frustum intersects the aabb
// For each plane in frustum determine if the box axis intersects the frustum plane
template <typename T>
bool intersect(const frustum<T>&, const aabbox<T, vec3>&, vec3<T>&);

// This function is only valid if s1 is intersecting s2
// Calculates the collision normal vector that points toward s1
// Calculates the closest point on s2 to s1's center
// returns the translation offset of s1 needed to eliminate penetration between s1 and s2
template <typename T, template <typename> class vec>
vec<T> resolve(const sphere<T, vec>&, const sphere<T, vec>&, vec<T>&, vec<T>&, const T);

// This function is only valid if box1 is intersecting box2
// Calculates the collision normal vector that points toward box1
// Calculates the closest point on box2 to box1's center
// returns the translation offset of box1 needed to eliminate penetration between box1 and box2
template <typename T, template <typename> class vec>
vec<T> resolve(const aabbox<T, vec>&, const aabbox<T, vec>&, vec<T>&, vec<T>&, const T);

// This function is only valid if box1 is intersecting box2
// Calculates the collision normal vector that points toward box1
// Calculates the closest point on box2 to box1's center
// returns the translation offset of box1 needed to eliminate penetration between box1 and box2
template <typename T, template <typename> class vec>
vec<T> resolve(const oobbox<T, vec>&, const oobbox<T, vec>&, vec<T>&, vec<T>&, const T);
}

#endif
