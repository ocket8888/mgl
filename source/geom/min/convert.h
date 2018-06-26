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
#ifndef __CONVERT__
#define __CONVERT__

#include <array>
#include <initializer_list>

#include "aabbox.h"
#include "mesh.h"
#include "sphere.h"

namespace min
{

// Calculates normals and tangents
template <typename T, typename K>
inline void finalize_mesh(mesh<T, K>&);

// Appends a sphere into a mesh for drawing
template <typename T, typename K>
inline void append_mesh(const sphere<T, vec3>&, mesh<T,K>&);

// Converts an aabbox into a mesh for drawing
template <typename T, typename K>
inline void append_mesh(const aabbox<T, vec3>&, mesh<T,K>&);

// Converts a sphere into a mesh for drawing
template <typename T, typename K>
inline mesh<T,K> to_mesh(const sphere<T, vec3>&);

// Converts an aabbox into a mesh for drawing
template <typename T, typename K>
inline mesh<T, K> to_mesh(const aabbox<T, vec3>&);
}

#endif
