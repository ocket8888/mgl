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
#ifndef __MD5_ANIM__
#define __MD5_ANIM__

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "geom/min/aabbox.h"
#include "math/min/mat4.h"
#include "mem_chunk.h"
#include "math/min/quat.h"
#include "strtoken.h"
#include "math/min/vec3.h"

// A node is the same thing as a joint
// A transform is a base frame
// An animated node is an animated joint
// A bound is stored into an aabb

namespace min
{

class md5_node
{
  private:
    std::string _name;
    int _parent;
    int _flag;
    unsigned _start;

  public:
    md5_node(const std::string name, int parent, int flag, unsigned start) : _name(name), _parent(parent), _flag(flag), _start(start) {}
    const std::string &get_name() const;
    int get_parent() const;
    int get_flag() const;
    unsigned get_start() const;
};

template <typename T>
class md5_transform
{
  private:
    vec3<T> _position;
    quat<T> _rotation;

  public:
    md5_transform(const vec3<T>&, const quat<T>&);
    const vec3<T> &get_position() const;
    const quat<T> &get_rotation() const;
};

template <class T>
class md5_animated_node
{
  private:
    int _parent;
    vec3<T> _position;
    quat<T> _rotation;

  public:
    md5_animated_node(const md5_transform<T>&, int);
    int get_parent() const;
    vec3<T> &get_position();
    const vec3<T> &get_position() const;
    quat<T> &get_rotation();
    const quat<T> &get_rotation() const;
};

template <typename T>
class md5_frame_data
{
  private:
    unsigned _id;
    std::vector<T> _data;

  public:
    md5_frame_data(unsigned id) : _id(id) {}
    void add(const T);
    const std::vector<T> &get_data() const;
    void reserve(size_t);
};

template <class T>
class md5_frame
{
  private:
    std::vector<mat4<T>> _bones;
    std::vector<md5_animated_node<T>> _nodes;

  public:
    void add_node(const md5_animated_node<T>&, const mat4<T>&);
    const md5_animated_node<T> &get_node(const int) const;
    const std::vector<mat4<T>> &get_bones() const;
    void reserve(size_t);

};

template <typename T>
class md5_anim
{
  private:
    std::vector<md5_node> _nodes;
    std::vector<aabbox<T, vec3>> _bounds;
    std::vector<md5_transform<T>> _transforms;
    std::vector<md5_frame_data<T>> _frame_data;
    std::vector<md5_frame<T>> _frames;
    unsigned _frame_rate;
    T _animation_length;
    mutable unsigned _loops;
    mutable std::vector<mat4<T>> _current_frame;
    mutable T _time;

    void load_file(const std::string);
    void load(const std::string&);
    void interpolate_current_frame(const md5_frame<T>&, const md5_frame<T>&, T) const;
    void process_hierarchy(const std::vector<std::string>&);
    void process_bounds(const std::vector<std::string>&);
    void process_baseframe(const std::vector<std::string>&);
    void process_frame_data(const std::vector<std::string>&, const unsigned, const unsigned);
    void process_frame(const md5_frame_data<T>&);

  public:
    md5_anim(const std::string&);
    md5_anim(const mem_file&);

    const std::vector<aabbox<T, vec3>> &get_bounds() const;
    const std::vector<mat4<T>> &get_current_frame() const;
    unsigned get_frame_rate() const;
    const std::vector<md5_frame_data<T>> &get_frame_data() const;
    const std::vector<md5_frame<T>> &get_frames() const;
    const std::vector<md5_node> &get_nodes() const;
    const std::vector<md5_transform<T>> &get_transforms()const;
    unsigned get_loop_count() const;
    void set_loop_count(const unsigned) const;
    void set_time(const T) const;
    void step(const T) const;
};
}

#endif
