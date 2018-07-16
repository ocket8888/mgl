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
#ifndef __PHYSICS__
#define __PHYSICS__

// The physics equations governing rigid bodies are listed below
// V = linear_velocity
// omega = angular_velocity
// F = Force
// P = position
// C = contact_position
// G = Gravity
// m = mass
// I = inertia
// k = damping parameter
// 1.) dV/dt = a = (F - k*V + G) / m
// 2.) domega/dt = alpha = ((C - P) x F - k*omega) / I

// These equations are solved using RK4
// dy/dt = f(t, y)
// y_n+1 = y_n + (dt / 6) * (k_1 + 2*k_2 + 2*k_3 + k_4)
// t_n+1 = t + dt
// k1 = f(t_n, y_n)
// k2 = f(t_n + 0.5*dt, y_n + 0.5*k1*dt)
// k3 = f(t_n + 0.5*dt, y_n + 0.5*k2*dt)
// k4 = f(t_n + dt, y_n + k3*dt)

#include <cmath>
#include <functional>
#include <stdexcept>
#include <vector>

#include "geom/min/intersect.h"
#include "template_math.h"

namespace min
{

union body_data {
    size_t index;
    void *ptr;
    int32_t sign;
    body_data() : ptr(nullptr) {}
    body_data(const size_t i) : index(i) {}
    body_data(void *const p) : ptr(p) {}
    body_data(const int32_t i) : sign(i) {}
};

template <typename T, template <typename> class vec, class angular, template <typename> class rot>
class body_base
{
  protected:
    vec<T> _force;
    angular _torque;
    vec<T> _position; // This is at the center of mass
    rot<T> _rotation;
    vec<T> _linear_velocity;
    angular _angular_velocity;
    T _mass;
    T _inv_mass;
    angular _inertia;
    angular _inv_inertia;
    size_t _id;
    body_data _data;
    bool _dead;

  public:
    body_base(const vec<T>&, const vec<T>&, const T, const angular&, const size_t, const body_data);

    inline void add_force(const vec<T>&);
    inline void add_torque(const vec<T>&);
    inline void add_torque(const vec<T>&, const vec<T>&);
    inline vec<T> align(const vec<T>&) const;
    inline void clear_force(const vec<T>&);
    inline void clear_torque();
    inline void clear_no_force();
    inline const angular get_angular_acceleration(const angular, const T) const;
    inline const angular &get_angular_velocity() const;
    inline body_data get_data() const;
    inline size_t get_id() const;
    inline const vec<T> get_linear_acceleration(const vec<T>&, const T) const;
    inline const vec<T> &get_linear_velocity() const;
    inline const T get_mass() const;
    inline const T get_inv_mass() const;
    inline const angular &get_inertia() const;
    inline const angular &get_inv_inertia() const;
    inline const rot<T> &get_rotation() const;
    inline const vec<T> &get_position() const;
    inline bool is_dead() const;
    inline void kill();
    inline void set_angular_velocity(const angular);
    inline void set_data(const body_data);
    inline void set_linear_velocity(const vec<T>&);
    inline void set_no_move();
    inline void set_no_rotate();
    inline void set_position(const vec<T>&);
    inline void set_rotation(const rot<T>&);
    inline const void move_offset(const vec<T>&);
    inline void update_position(const vec<T>&, const T, const vec<T>&, const vec<T>&);

};

template <typename T, template <typename> class vec>
class body : public body_base<T, vec, vec<T>, quat>
{
};

// Partial specialization for resolving the type of angular_velocity for vec2 = T
template <typename T>
class body<T, vec2> : public body_base<T, vec2, T, mat2>
{
  private:
    std::function<void(body<T, vec2> &, body<T, vec2> &)> _f;

  public:
    body(const vec2<T>&, const vec2<T>&, const T, const T, const size_t, const body_data);
    inline void callback(body<T, vec2>&);
    inline void register_callback(const std::function<void(body<T, vec2>&, body<T, vec2>&)>&);
    inline mat2<T> update_rotation(const T, const T);
};

// Partial specialization for resolving the type of angular_velocity for vec3 = vec3<T>
template <typename T>
class body<T, vec3> : public body_base<T, vec3, vec3<T>, quat>
{
  private:
    std::function<void(body<T, vec3> &, body<T, vec3> &)> _f;

  public:
    body(const vec3<T>&, const vec3<T>&, const T, const vec3<T>&, const size_t, const body_data);

    inline void callback(body<T, vec3>&);
    inline void register_callback(const std::function<void(body<T, vec3>&, body<T, vec3>&)>&);
    inline quat<T> update_rotation(const vec3<T>&, const T);
};

// Partial specialization for resolving the type of angular_velocity for vec4 = vec4<T>
template <typename T>
class body<T, vec4> : public body_base<T, vec4, vec4<T>, quat>
{
  private:
    std::function<void(body<T, vec4> &, body<T, vec4> &)> _f;

  public:
    body(const vec4<T>&, const vec4<T>&, const T, const vec4<T>&, const size_t, const body_data);

    inline void callback(body<T, vec4> &b2);
    inline void register_callback(const std::function<void(body<T, vec4>&, body<T, vec4>&)>&);
    inline quat<T> update_rotation(const vec4<T>&, const T);
};

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape,
          template <typename, typename, typename, template <typename> class, template <typename, template <typename> class> class, template <typename, template <typename> class> class> class spatial>
class physics
{
  private:
    spatial<T, K, L, vec, cell, shape> _spatial;
    std::vector<shape<T, vec>> _shapes;
    std::vector<body<T, vec>> _bodies;
    std::vector<size_t> _dead;
    vec<T> _gravity;
    T _elasticity;
    bool _clean;

    static constexpr T _collision_tolerance = 1E-4;

    inline void collide(const size_t, const size_t);
    inline bool collide_static(const size_t, const shape<T, vec>&);

    // The normal axis is defined to be the vector between b1 and b2, pointing towards b1
    // n = C1 - C2
    // These equations yield an impulse J, which is a change in linear momentum
    // This impulse at point P creates a change in angular momentum
    // J1 = n * m1(v1' - v1)
    // J2 = n * m2(v2' - v2)
    // F = J/dt
    // dL1 = (P - C1) x J1
    // dL2 = (P - C2) x J2

    // Intersection point intersect
    inline void solve_energy_conservation(body<T, vec>&, body<T, vec>&, const vec<T>&, const vec<T>&);
    // Collision with object of infinite mass
    inline void solve_energy_conservation_static(body<T, vec>&, const vec<T>&, const vec<T>&);
    inline void solve_integrals(const size_t, const T, const T);
    inline void solve_integrals(const T, const T);

  public:
    physics(const cell<T, vec>&, const vec<T>&);

    inline size_t add_body(const shape<T, vec>&, const T, const size_t = 0, const body_data = nullptr);
    inline vec<T> clamp_bounds(const vec<T>&) const;
    inline void clear_body(const size_t);
    inline void clear();
    inline bool collide(const size_t, const shape<T, vec>&);
    inline const body<T, vec> &get_body(const size_t) const;
    inline body<T, vec> &get_body(const size_t);
    inline const std::vector<body<T, vec>> &get_bodies() const;
    inline std::vector<body<T, vec>> &get_bodies();
    inline const std::vector<std::pair<K, vec<T>>> &get_collisions(const ray<T, vec>&) const;
    inline const vec<T> &get_gravity() const;
    inline const std::vector<K> &get_index_map() const;
    inline const std::vector<std::pair<K, K>> &get_overlap(const shape<T, vec>&) const;
    inline const size_t get_scale() const;
    inline const shape<T, vec> &get_shape(const size_t index) const;
    inline void prune_after(const size_t);
    inline void prune_after_force(const size_t);
    inline void register_callback(const size_t, const std::function<void(body<T, vec>&, body<T, vec>&)>&);
    inline void reserve(const size_t);
    inline void solve(const T, const T);
    inline void solve_no_collide(const T, const T);
    inline void solve_no_sort(const T dt, const T);
    inline T get_total_energy() const;
    inline void set_elasticity(const T);
};
}

#endif
