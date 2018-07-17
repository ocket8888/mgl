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
#include "coord_sys.h"

template class min::coord_sys_base<double, min::vec2>;
template class min::coord_sys_base<float, min::vec2>;
template class min::coord_sys<double, min::vec2>;
template class min::coord_sys<float, min::vec2>;
template class min::coord_sys_base<double, min::vec3>;
template class min::coord_sys_base<float, min::vec3>;
template class min::coord_sys<double, min::vec3>;
template class min::coord_sys<float, min::vec3>;
template class min::coord_sys_base<double, min::vec4>;
template class min::coord_sys_base<float, min::vec4>;
template class min::coord_sys<double, min::vec4>;
template class min::coord_sys<float, min::vec4>;

//// coord_sys_base ////
template <typename T, template <typename> class vec>
const vec<T> &min::coord_sys_base<T,vec>::x() const
{
	return _x;
}

template <typename T, template <typename> class vec>
const vec<T> &min::coord_sys_base<T,vec>::y() const
{
	return _y;
}


//// coord_sys for vec2 ////
template <typename T>
void min::coord_sys<T, min::vec2>::rotate(const min::mat2<T> &r)
{
	this->_x = r.transform(this->_x);
	this->_y = r.transform(this->_y);
}

template <typename T>
min::vec2<T> min::coord_sys<T, min::vec2>::align( const min::vec2<T> &v) const
{
	return min::vec2<T>(v.dot(this->_x), v.dot(this->_y));
}


//// coord_sys for vec3 ////
template <typename T>
const min::vec3<T> &min::coord_sys<T, min::vec3>::z() const
{
    return _z;
}

template <typename T>
void min::coord_sys<T, min::vec3>::rotate(const min::quat<T> &r)
{
    this->_x = r.transform(this->_x);
    this->_y = r.transform(this->_y);
    this->_z = r.transform(this->_z);
}

template <typename T>
min::vec3<T> min::coord_sys<T, min::vec3>::align(const min::vec3<T> &v) const
{
    return vec3<T>(v.dot(this->_x), v.dot(this->_y), v.dot(this->_z));
}

//// coord_sys for vec4 ////

template <typename T>
const min::vec4<T> &min::coord_sys<T, min::vec4>::z() const
{
    return _z;
}

template <typename T>
void min::coord_sys<T, min::vec4>::rotate(const min::quat<T> &r)
{
    this->_x = r.transform(this->_x);
    this->_y = r.transform(this->_y);
    this->_z = r.transform(this->_z);
}

template <typename T>
min::vec4<T> min::coord_sys<T, min::vec4>::align(const min::vec4<T> &v) const
{
    return vec4<T>(v.dot(this->_x), v.dot(this->_y), v.dot(this->_z), 1.0);
}
