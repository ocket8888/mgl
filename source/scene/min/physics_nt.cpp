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

#include "physics_nt.h"

//// body_base ////
template <typename T, template <typename> class vec, class angular, template<typename> class rot>
min::body_base<T, vec, angular, rot>::body_base(const vec<T> &center, const vec<T> &gravity, const T mass, const size_t id, const body_data data)
    : _force(gravity * mass), _position(center), _angular_velocity{},
      _mass(mass), _inv_mass(1.0 / mass),
      _id(id), _data(data), _dead(false) {}


template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::add_force(const vec<T> &force)
{
    // Add force to force vector
    _force += force;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::clear_force(const vec<T> &gravity)
{
    // Gravity = mg
    _force = gravity * _mass;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::clear_no_force()
{
    // Set no force on this object
    _force = vec<T>();

    // Clear all linear velocity
    _linear_velocity = vec<T>();

    // Clear all rotational  velocity
    _angular_velocity = angular{};
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const angular &min::body_base<T, vec, angular, rot>::get_angular_velocity() const
{
    return _angular_velocity;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline min::body_data min::body_base<T, vec, angular, rot>::get_data() const
{
    return _data;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline size_t min::body_base<T, vec, angular, rot>::get_id() const
{
    return _id;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const vec<T> min::body_base<T, vec, angular, rot>::get_linear_acceleration(const vec<T> &linear_velocity, const T damping) const
{
    // Calculate the acceleration
    return (_force - linear_velocity * damping) * _inv_mass;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const vec<T> &min::body_base<T, vec, angular, rot>::get_linear_velocity() const
{
    return _linear_velocity;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const T min::body_base<T, vec, angular, rot>::get_mass() const
{
    return _mass;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const T min::body_base<T, vec, angular, rot>::get_inv_mass() const
{
    return _inv_mass;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const rot<T> &min::body_base<T, vec, angular, rot>::get_rotation() const
{
    return _rotation;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const vec<T> &min::body_base<T, vec, angular, rot>::get_position() const
{
    return _position;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline bool min::body_base<T, vec, angular, rot>::is_dead() const
{
    return _dead;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::kill()
{
    _dead = true;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::set_angular_velocity(const angular w)
{
    _angular_velocity = w;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::set_data(const body_data data)
{
    _data = data;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::set_linear_velocity(const vec<T> &v)
{
    _linear_velocity = v;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::set_no_move()
{
    // Make the object's mass infinite
    _inv_mass = 0.0;
    _mass = 0.0;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::set_position(const vec<T> &p)
{
    _position = p;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::set_rotation(const rot<T> &r)
{
    _rotation = r;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline const void min::body_base<T, vec, angular, rot>::move_offset(const vec<T> &offset)
{
    _position += offset;
}

template <typename T, template <typename> class vec, class angular, template<typename> class rot>
inline void min::body_base<T, vec, angular, rot>::update_position(const vec<T> &linear_velocity, const T time_step, const vec<T> &min, const vec<T> &max)
{
    // Update position from velocity
    _position += linear_velocity * time_step;

    // Clamp position to wall of physics world
    const vec<T> direction = _position.clamp_direction(min, max);

    // Reverses linear velocity if hit edge of world
    _linear_velocity = linear_velocity * direction;
}

//// body<T, vec2> ////
template <typename T>
min::body<T, min::vec2>::body(const min::vec2<T> &center, const min::vec2<T> &gravity, const T mass, const size_t id, const min::body_data data)
    : body_base<T, vec2, T, mat2>(center, gravity, mass, id, data), _f(nullptr) {}


template <typename T>
inline void min::body<T, min::vec2>::callback(body<T, min::vec2> &b2)
{
    // If we registered a callback
    if (this->_f)
    {
        this->_f(*this, b2);
    }
}

template <typename T>
inline void min::body<T, min::vec2>::register_callback(const std::function<void(body<T, min::vec2> &, min::body<T, min::vec2> &)> &f)
{
    this->_f = f;
}

template <typename T>
inline min::mat2<T> min::body<T, min::vec2>::update_rotation(const T time_step)
{
    // Rotation is around the Z axis in euler angles
    const mat2<T> out(this->_angular_velocity * time_step);

    // Transform the absolute rotation
    this->_rotation *= out;

    // return the relative rotation
    return this->_rotation;
}


//// body<T, vec3> ////
template <typename T>
min::body<T, min::vec3>::body(const min::vec3<T> &center, const min::vec3<T> &gravity, const T mass, const size_t id, const min::body_data data)
    : body_base<T, vec3, vec3<T>, quat>(center, gravity, mass, id, data), _f(nullptr) {}


template <typename T>
inline void min::body<T, min::vec3>::callback(body<T, min::vec3> &b2)
{
    // If we registered a callback
    if (this->_f)
    {
        this->_f(*this, b2);
    }
}

template <typename T>
inline void min::body<T, min::vec3>::register_callback(const std::function<void(body<T, min::vec3> &, min::body<T, min::vec3> &)> &f)
{
    this->_f = f;
}

template <typename T>
inline min::quat<T> min::body<T, min::vec3>::update_rotation(const T time_step)
{
    // Calculate rotation for this timestep
    vec3<T> rotation = this->_angular_velocity * time_step;

    // Calculate rotation angle for angular velocity
    const T angle = rotation.magnitude();
    if (angle > var<T>::TOL_REL)
    {
        // Normalize rotation axis
        const T inv_angle = 1.0 / angle;
        rotation *= inv_angle;

        // Create quaternion rotation with angle
        const quat<T> q(rotation, angle);

        // Transform the absolute rotation
        this->_rotation *= q;

        // Normalize the rotation vector to avoid accumulation of rotational energy
        this->_rotation.normalize();
    }

    // return the absolute rotation
    return this->_rotation;
}


//// body<T, vec4> ////
template <typename T>
min::body<T, min::vec4>::body(const min::vec4<T> &center, const min::vec4<T> &gravity, const T mass, const size_t id, const min::body_data data)
    : body_base<T, vec4, vec4<T>, quat>(center, gravity, mass, id, data), _f(nullptr) {}


template <typename T>
inline void min::body<T, min::vec4>::callback(body<T, min::vec4> &b2)
{
    // If we registered a callback
    if (this->_f)
    {
        this->_f(*this, b2);
    }
}

template <typename T>
inline void min::body<T, min::vec4>::register_callback(const std::function<void(body<T, min::vec4> &, min::body<T, min::vec4> &)> &f)
{
    this->_f = f;
}

template <typename T>
inline min::quat<T> min::body<T, min::vec4>::update_rotation(const T time_step)
{
    // Calculate rotation for this timestep
    vec3<T> rotation = this->_angular_velocity * time_step;

    // Calculate rotation angle for angular velocity
    const T angle = rotation.magnitude();
    if (angle > var<T>::TOL_REL)
    {
        // Normalize rotation axis
        const T inv_angle = 1.0 / angle;
        rotation *= inv_angle;

        // Create quaternion rotation with angle
        const quat<T> q(rotation, angle);

        // Transform the absolute rotation
        this->_rotation *= q;

        // Normalize the rotation vector to avoid accumulation of rotational energy
        this->_rotation.normalize();
    }

    // return the absolute rotation
    return this->_rotation;
}


//// physics ////
template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::collide(const size_t index1, const size_t index2)
{
    // Get rigid bodies to solve energy equations
    body<T, vec> &b1 = _bodies[index1];
    body<T, vec> &b2 = _bodies[index2];

    // Check if either body has died
    if (b1.is_dead() || b2.is_dead())
    {
        return;
    }

    // Get shapes from spatial index
    const shape<T, vec> &s1 = _shapes[index1];
    const shape<T, vec> &s2 = _shapes[index2];

    // Calculate...
    // 1) the collision normal vector that points toward b1
    // 2) the intersection point between bodies
    // 3) the offset to resolve the collision

    vec<T> collision_normal;
    vec<T> intersection;
    const vec<T> offset = resolve<T, vec>(s1, s2, collision_normal, intersection, _collision_tolerance);

    // Do the collision callback function
    b1.callback(b2);
    b2.callback(b1);

    // Solve linear and angular momentum conservation equations
    solve_energy_conservation(b1, b2, collision_normal, intersection);

    // If an object has infinite mass, inv_mass = 0
    // Move each object based off inv_mass
    // Treat this as a parallel circuit, 1/R = 1/R_1 + 1/R_2
    // For this case V = 1.0
    // The current through this circuit is V/R = I, or 1.0*(1/R_1 + 1/R_2) = 1/R_1 + 1/R_2 = I
    // The percentage split can be calculated from (I - 1/R_x) / I

    // Split offset in half and move b1 and b2 in opposite directions
    const T total = b1.get_inv_mass() + b2.get_inv_mass();
    if (total > var<T>::TOL_ZERO)
    {
        const T inv_total = 1.0 / total;
        const vec<T> half_offset1 = offset * (total - b2.get_inv_mass()) * inv_total;
        const vec<T> half_offset2 = offset * (b1.get_inv_mass() - total) * inv_total;

        // Resolve collision and resolve penetration depth
        b1.move_offset(half_offset1);
        b2.move_offset(half_offset2);
    }
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline bool min::physics<T,K,L,vec,cell,shape,spatial>::collide_static(const size_t index, const shape<T, vec> &s2)
{
    // Get rigid bodies to solve energy equations
    body<T, vec> &b = _bodies[index];

    // Check if body has died
    if (b.is_dead())
    {
        return false;
    }

    // Get shapes from spatial index
    const shape<T, vec> &s1 = _shapes[index];

    // Test if the body is intersecting the static shape
    const bool collide = intersect(s1, s2);
    if (collide)
    {
        // Calculate...
        // 1) the collision normal vector that points toward body
        // 2) the intersection point between body and static shape
        // 3) the offset to resolve the collision

        vec<T> collision_normal;
        vec<T> intersection;
        const vec<T> offset = resolve<T, vec>(s1, s2, collision_normal, intersection, _collision_tolerance);

        // Solve linear and angular momentum conservation equations
        solve_energy_conservation_static(b, collision_normal, intersection);

        // Move based off calculated offfset
        b.move_offset(offset);
    }

    // return if we collided
    return collide;
}
// Intersection point intersect

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::solve_energy_conservation(body<T, vec> &b1, min::body<T, vec> &b2, const vec<T> &n, const vec<T> &intersect)
{
    // Get velocities of bodies in world space
    const T v1n = b1.get_linear_velocity().dot(n);
    const T v2n = b2.get_linear_velocity().dot(n);

    // If objects are moving very slowly, skip calculation
    // If objects are moving away from each other, skip calculation
    if (v1n >= -_collision_tolerance && v2n <= _collision_tolerance)
    {
        return;
    }

    // If objects are moving in the same direction, skip calculation
    if (std::abs(v1n - v2n) <= _collision_tolerance)
    {
        return;
    }

    // Get inverse masses of bodies
    const T inv_m1 = b1.get_inv_mass();
    const T inv_m2 = b2.get_inv_mass();

    // Get velocities of bodies in world space
    const vec<T> &v1 = b1.get_linear_velocity();
    const vec<T> &v2 = b2.get_linear_velocity();

    // Calculate the relative velocity between b1 and b2 in world space
    const vec<T> v12 = v1 - v2;

    // Calculate the kinetic resistance of the object
    const T resistance = inv_m1 + inv_m2;

    // Calculate the impulse
    const T j = -(1.0 + _elasticity) * (v12.dot(n) / resistance);

    // Calculate the impulse vector
    const vec<T> impulse = n * j;

    // Calculate linear velocity vectors
    const vec<T> v1_out = v1 + impulse * inv_m1;
    const vec<T> v2_out = v2 - impulse * inv_m2;

    // Update body linear velocity
    b1.set_linear_velocity(v1_out);
    b2.set_linear_velocity(v2_out);
}

// Collision with object of infinite mass

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::solve_energy_conservation_static(body<T, vec> &b, const vec<T> &n, const vec<T> &intersect)
{
    // Get velocities of bodies in world space
    const T v1n = b.get_linear_velocity().dot(n);

    // If objects are moving very slowly, skip calculation
    // If objects are moving away from each other, skip calculation
    if (v1n >= -_collision_tolerance)
    {
        return;
    }

    // If object is not moving, skip calculation
    if (std::abs(v1n) <= _collision_tolerance)
    {
        return;
    }

    // Get inverse masses of bodies
    const T inv_m = b.get_inv_mass();

    // Get velocities of bodies in world space
    const vec<T> &v = b.get_linear_velocity();

    // Calculate the relative velocity of body
    const vec<T> v_rel = v;

    // Calculate the kinetic resistance of the object
    const T resistance = inv_m;

    // Calculate the impulse
    const T j = -(1.0 + _elasticity) * (v_rel.dot(n) / resistance);

    // Calculate the impulse vector
    const vec<T> impulse = n * j;

    // Calculate linear velocity vectors
    const vec<T> v_out = v + impulse * inv_m;

    // Update body linear velocity
    b.set_linear_velocity(v_out);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::solve_integrals(const size_t index, const T dt, const T damping)
{
    // Check if body has died
    body<T, vec> &b = _bodies[index];
    if (b.is_dead())
    {
        return;
    }

    // Precalculate time constants
    const T dt2 = dt * 0.5;
    const T dt6 = dt * 0.16667;

    // Solve for linear velocity
    const auto v_n = b.get_linear_velocity();

    // Evaluate the derivative at different linear velocities
    const auto vk1 = b.get_linear_acceleration(v_n, damping);
    const auto vk2 = b.get_linear_acceleration(v_n + vk1 * dt2, damping);
    const auto vk3 = b.get_linear_acceleration(v_n + vk2 * dt2, damping);
    const auto vk4 = b.get_linear_acceleration(v_n + vk3 * dt, damping);

    // Calculate the linear velocity at this time step
    const auto v_n1 = v_n + (vk1 + (vk2 * 2.0) + (vk3 * 2.0) + vk4) * dt6;

    // Update the body position at this timestep
    b.update_position(v_n1, dt, _spatial.get_lower_bound(), _spatial.get_upper_bound());

    // Update the body rotation at this timestep
    const auto abs_rotation = b.update_rotation(dt);

    // Clear any acting forces on this object
    b.clear_force(_gravity);

    // Update the shapes position
    shape<T, vec> &s = _shapes[index];
    s.set_position(b.get_position());

    // Rotate the shapes by the relative rotation
    rotate<T>(s, abs_rotation);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::solve_integrals(const T dt, const T damping)
{
    // Solve the first order initial value problem differential equations with Runge-Kutta4
    const size_t size = _bodies.size();
    for (size_t i = 0; i < size; i++)
    {
        solve_integrals(i, dt, damping);
    }
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
min::physics<T,K,L,vec,cell,shape,spatial>::physics(const cell<T, vec> &world, const vec<T> &gravity)
    : _spatial(world),
      _gravity(gravity), _elasticity(1.0), _clean(true) {}


template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline size_t min::physics<T,K,L,vec,cell,shape,spatial>::add_body(const shape<T, vec> &s, const T mass, const size_t id, const min::body_data data)
{
    // Clamp the body to be inside the grid
    const vec<T> center = clamp_bounds(s.get_center());

    // Create new shape for simulation using clamped position
    shape<T, vec> in_s(s);
    in_s.set_position(center);

    // If bodies can be recycled
    if (_dead.size() > 0)
    {
        // Get recycle index
        const size_t index = _dead.back();
        _dead.pop_back();

        // Flag clean if no more dead bodies
        if (_dead.size() == 0)
        {
            _clean = true;
        }

        // Recycle shape
        _shapes[index] = in_s;

        // Recycle body
        _bodies[index] = body<T, vec>(center, _gravity, mass, id, data);

        // Return recycled index
        return index;
    }

    // Add shape to shape vector
    _shapes.push_back(in_s);

    // Create rigid body for this shape
    _bodies.emplace_back(center, _gravity, mass, id, data);

    // return the body id
    return _bodies.size() - 1;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline vec<T> min::physics<T,K,L,vec,cell,shape,spatial>::clamp_bounds(const vec<T> &point) const
{
    return _spatial.clamp_bounds(point);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::clear_body(const size_t index)
{
    // Flag this body for destruction
    _bodies[index].kill();

    // Add body index to the dead list
    _dead.push_back(index);

    // Flag that we dirtied up
    _clean = false;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::clear()
{
    // Clear out the shapes
    _shapes.clear();

    // Clear out the bodies
    _bodies.clear();

    // Clear out the dead bodies
    _dead.clear();

    // Clean the simulation
    _clean = true;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline bool min::physics<T,K,L,vec,cell,shape,spatial>::collide(const size_t index, const shape<T, vec> &s)
{
    // return whether we collided or not
    return collide_static(index, s);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const min::body<T, vec> &min::physics<T,K,L,vec,cell,shape,spatial>::get_body(const size_t index) const
{
    return _bodies[index];
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline min::body<T, vec> &min::physics<T,K,L,vec,cell,shape,spatial>::get_body(const size_t index)
{
    return _bodies[index];
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const std::vector<min::body<T, vec>> &min::physics<T,K,L,vec,cell,shape,spatial>::get_bodies() const
{
    return _bodies;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline std::vector<min::body<T, vec>> &min::physics<T,K,L,vec,cell,shape,spatial>::get_bodies()
{
    return _bodies;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const std::vector<std::pair<K, vec<T>>> &min::physics<T,K,L,vec,cell,shape,spatial>::get_collisions(const min::ray<T, vec> &r) const
{
    return _spatial.get_collisions(r);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const vec<T> &min::physics<T,K,L,vec,cell,shape,spatial>::get_gravity() const
{
    return _gravity;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const std::vector<K> &min::physics<T,K,L,vec,cell,shape,spatial>::get_index_map() const
{
    return _spatial.get_index_map();
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const std::vector<std::pair<K, K>> &min::physics<T,K,L,vec,cell,shape,spatial>::get_overlap(const shape<T, vec> &overlap) const
{
    return _spatial.get_overlap(overlap);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const size_t min::physics<T,K,L,vec,cell,shape,spatial>::get_scale() const
{
    return _spatial.get_scale();
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline const shape<T, vec> &min::physics<T,K,L,vec,cell,shape,spatial>::get_shape(const size_t index) const
{
    return _shapes[index];
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::prune_after(const size_t index)
{
    // Check if we need to prune
    if (!_clean)
    {
        prune_after_force(index);
    }
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::prune_after_force(const size_t index)
{
    // Clear all dead bodies
    _dead.clear();

    // Shorten shape and body buffers
    const size_t size = _shapes.size();
    for (size_t i = index; i < size; i++)
    {
        _shapes.pop_back();
        _bodies.pop_back();
    }

    // Scan for dead bodies in remnants
    for (size_t i = 0; i < index; i++)
    {
        if (_bodies[i].is_dead())
        {
            _dead.push_back(i);
        }
    }

    // Flag that we cleaned up
    _clean = true;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::register_callback(const size_t index, const std::function<void(body<T, vec> &, min::body<T, vec> &)> &f)
{
    _bodies[index].register_callback(f);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::reserve(const size_t size)
{
    // Reserve memory for shapes and bodies
    _shapes.reserve(size);
    _bodies.reserve(size);
    _dead.reserve(size);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::solve(const T dt, const T damping)
{
    if (_shapes.size() > 0)
    {
        // Create the spatial partitioning structure based off rigid bodies
        // This reorders the shapes vector so we need to reorganize the shape and body data to reflect this!
        _spatial.insert(_shapes);

        // Get the index map for reordering
        const std::vector<K> &map = _spatial.get_index_map();

        // Determine intersecting shapes for contact resolution
        const std::vector<std::pair<K, K>> &collisions = _spatial.get_collisions();

        // Handle all collisions between objects
        for (const auto &c : collisions)
        {
            collide(map[c.first], map[c.second]);
        }

        // Solve the simulation
        solve_integrals(dt, damping);
    }
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::solve_no_collide(const T dt, const T damping)
{
    // Solve the simulation
    solve_integrals(dt, damping);
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::solve_no_sort(const T dt, const T damping)
{
    if (_shapes.size() > 0)
    {
        // Create the spatial partitioning structure based off rigid bodies
        // This doesn't reorder the shapes vector
        _spatial.insert_no_sort(_shapes);

        // Determine intersecting shapes for contact resolution
        const std::vector<std::pair<K, K>> &collisions = _spatial.get_collisions();

        // Handle all collisions between objects
        for (const auto &c : collisions)
        {
            collide(c.first, c.second);
        }

        // Solve the simulation
        solve_integrals(dt, damping);
    }
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline T min::physics<T,K,L,vec,cell,shape,spatial>::get_total_energy() const
{
    T KE2 = 0.0;
    T PE = 0.0;

    for (auto &b : _bodies)
    {
        // Calculate double kinetic energy = mv^2
        const vec<T> &v = b.get_linear_velocity();
        const T m = b.get_mass();
        KE2 += m * v.dot(v);

        // Calculate the potential energy = -mgh
        PE += m * _gravity.dot(_spatial.get_lower_bound() - b.get_position());
    }

    return 0.5 * KE2 + PE;
}

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
    template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
inline void min::physics<T,K,L,vec,cell,shape,spatial>::set_elasticity(const T e)
{
    _elasticity = e;
}