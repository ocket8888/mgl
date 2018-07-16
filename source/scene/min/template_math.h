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
#ifndef __TEMPLATE_MATH__
#define __TEMPLATE_MATH__

#include "geom/min/aabbox.h"
#include "geom/min/oobbox.h"
#include "geom/min/sphere.h"
#include "math/min/mat2.h"
#include "math/min/mat3.h"
#include "math/min/mat4.h"
#include "math/min/quat.h"
#include "math/min/vec2.h"
#include "math/min/vec3.h"
#include "math/min/vec4.h"

namespace min
{
template <typename T> inline T align(const T, const mat2<T>&);
template <typename T> inline vec3<T> align(const vec3<T>&, const quat<T>&);
template <typename T> inline vec4<T> align(const vec4<T>&, const quat<T>&);
template <typename T> inline T transform(const T, const mat2<T>&);
template <typename T> inline vec3<T> transform(const vec3<T>&, const quat<T>&);
template <typename T> inline vec4<T> transform(const vec4<T>&, const quat<T>&);
template <typename T> inline vec2<T> cross(const T, const vec2<T>&);
template <typename T> inline vec3<T> cross(const vec3<T>&, const vec3<T>&);
template <typename T> inline vec4<T> cross(const vec4<T>&, const vec4<T>&);
template <typename T> inline T dot(const T, const T);
template <typename T> inline T dot(const vec3<T>&, const vec3<T>&);
template <typename T> inline T dot(const vec4<T>&, const vec4<T>&);
template <typename T> inline T inverse(const T);
template <typename T> inline vec3<T> inverse(const vec3<T>&);
template <typename T> inline vec4<T> inverse(const vec4<T>&);
template <typename T> inline T get_inertia(const aabbox<T, vec2>&, const T);
template <typename T> inline vec3<T> get_inertia(const aabbox<T, vec3>&, const T);
template <typename T> inline vec4<T> get_inertia(const aabbox<T, vec4>&, const T);
template <typename T> inline T get_inertia(const oobbox<T, vec2>&, const T);
template <typename T> inline vec3<T> get_inertia(const oobbox<T, vec3>&, const T);
template <typename T> inline vec4<T> get_inertia(const oobbox<T, vec4>&, const T);
template <typename T> inline T get_inertia(const sphere<T, vec2>&, const T);
template <typename T> inline vec3<T> get_inertia(const sphere<T, vec3>&, const T);
template <typename T> inline vec4<T> get_inertia(const sphere<T, vec4>&, const T);
template <typename T> inline void rotate(aabbox<T, vec2>&, const mat2<T> &);
template <typename T> inline void rotate(aabbox<T, vec3>&, const quat<T> &);
template <typename T> inline void rotate(aabbox<T, vec4>&, const quat<T> &);
template <typename T> inline void rotate(oobbox<T, vec2>&, const mat2<T> &);
template <typename T> inline void rotate(oobbox<T, vec3>&, const quat<T> &);
template <typename T> inline void rotate(oobbox<T, vec4>&, const quat<T> &);
template <typename T> inline void rotate(sphere<T, vec2>&, const mat2<T> &);
template <typename T> inline void rotate(sphere<T, vec3>&, const quat<T> &);
template <typename T> inline void rotate(sphere<T, vec4>&, const quat<T> &);

}

#endif
