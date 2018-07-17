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

#include "camera.h"

template <typename T>
void min::camera<T>::update()
{
    if (_dirty)
    {
        // Get the projection matrix
        mat4<T> const *proj = nullptr;
        if (_proj_ortho)
            proj = &_f.orthographic();
        else
            proj = &_f.perspective();

        // Update the view matrix
        _v = _f.look_at(_p, _forward, _up);

        // Update the projection-view matrix
        // (A*B)^T = (B^T*A^T)
        // A*B = ((A*B)^T)^T = ((B^T*A^T)^T)
        _pv = (*proj).transpose_multiply(_v);

        // No longer dirty
        _dirty = false;
    }
}

template <typename T>
min::camera<T>::camera() : _look(0.0, 0.0, 1.0), _forward(0.0, 0.0, 1.0), _up(0.0, 1.0, 0.0), _dirty(true), _proj_ortho(true) {}

template <typename T>
void min::camera<T>::force_update()
{
    // If the camera needs to be updated
    update();
}

template <typename T>
const min::vec3<T> &min::camera<T>::get_forward() const
{
    return _forward;
}

template <typename T>
const min::vec3<T> &min::camera<T>::get_right() const
{
    return _f.get_right();
}

template <typename T>
const min::vec3<T> &min::camera<T>::get_up() const
{
    return _up;
}

template <typename T>
min::frustum<T> &min::camera<T>::get_frustum()
{
    return _f;
}

template <typename T>
const min::frustum<T> &min::camera<T>::get_frustum() const
{
    return _f;
}

template <typename T>
const min::vec3<T> &min::camera<T>::get_look_at() const
{
    return _look;
}

template <typename T>
const min::mat4<T> &min::camera<T>::get_pv_matrix()
{
    // If the camera needs to be updated
    update();

    // Return the proj-view matrix in opengl format
    return _pv;
}

template <typename T>
const min::mat4<T> &min::camera<T>::get_v_matrix()
{
    // If the camera needs to be updated
    update();

    // Return the view matrix in opengl format
    return _v;
}

template <typename T>
const min::vec3<T> &min::camera<T>::get_position() const
{
    return _p;
}

template <typename T>
void min::camera<T>::make_dirty()
{
    _dirty = true;
}

template <typename T>
min::quat<T> min::camera<T>::move_look_at(const T x, const T y)
{
    // Calculate the direction camera is facing, centered at origin
    vec3<T> direction = _look - _p;

    // Get the right axis
    const vec3<T> &right = _f.get_right();

    // Rotate the camera by the move.y around the right
    quat<T> rotation(right, y);

    // Rotate the camera by the move.x around the global up
    rotation *= quat<T>(vec3<T>::up(), x);

    // Transform the direction
    direction = rotation.transform(direction);

    // Update look in world space
    _look = _p + direction;

    // Update the forward vector
    _forward = (direction).normalize();

    // Transform up vector
    _up = rotation.transform(_up);

    // Camera has moved
    _dirty = true;

    // return quaternion that rotated camera
    return rotation;
}

template <typename T>
min::vec3<T> min::camera<T>::project_point(const T s) const
{
    return _p + _forward * s;
}

template <typename T>
void min::camera<T>::set_look_at(const min::vec3<T> &look)
{
    // Compute new forward vector
    const vec3<T> forward = (look - _p).normalize();

    // Find the quaternion between two vectors
    const quat<T> q(_forward, forward);

    // Update look vector
    _look = look;

    // Update the forward vector
    _forward = forward;

    // Update up vector
    _up = q.transform(_up);

    // Camera has moved
    _dirty = true;
}

template <typename T>
void min::camera<T>::set_orthographic()
{
    _proj_ortho = true;

    // Clear out the frustum proj matrix
    _f.make_dirty();

    // Projection matrix has changed
    _dirty = true;
}

template <typename T>
void min::camera<T>::set_perspective()
{
    _proj_ortho = false;

    // Clear out the frustum proj matrix
    _f.make_dirty();

    // Projection matrix has changed
    _dirty = true;
}

template <typename T>
void min::camera<T>::set_position(const min::vec3<T> &p)
{
    // Change lookat based on updated position
    _look += (p - _p);

    // Update position
    _p = p;

    // Camera has moved
    _dirty = true;
}

template <typename T>
void min::camera<T>::set(const min::vec3<T> &p, const min::vec3<T> &look)
{
    // Compute new forward vector
    const vec3<T> forward = (look - p).normalize();

    // Find the quaternion between two vectors
    const quat<T> q(_forward, forward);

    // Update position
    _p = p;

    // Update look vector
    _look = look;

    // Update the forward vector
    _forward = forward;

    // Update up vector
    _up = q.transform(_up);

    // Camera has moved
    _dirty = true;
}

template <typename T>
void min::camera<T>::set(const min::vec3<T> &p, const min::vec3<T> &look, const min::vec3<T> &up)
{
    // Update position
    _p = p;

    // Update look vector
    _look = look;

    // Update the forward vector
    _forward = (_look - _p).normalize();

    // Update up vector
    _up = up;

    // Camera has moved
    _dirty = true;
}
