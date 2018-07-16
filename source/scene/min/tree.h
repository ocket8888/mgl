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
#ifndef __TREE__
#define __TREE__

#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "geom/min/intersect.h"
#include "math/min/utility.h"

// The shape class must fulfill the following interface to be inserted into the spatial structure
// shape.get_center()
// shape.get_min()
// shape.get_max()
// shape.square_size()
// intersect(shape, shape)

// Forward declaration for tree_node
namespace min
{
template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape>
class tree;
}

namespace min
{

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape>
class tree_node
{
    friend class tree<T, K, L, vec, cell, shape>;

  private:
    std::vector<tree_node<T, K, L, vec, cell, shape>> _child;
    std::vector<K> _keys;
    cell<T, vec> _cell;
    mutable std::vector<size_t> _sub_ray_cache;

    inline void add_key(K);
    inline void clear();
    inline std::vector<tree_node<T, K, L, vec, cell, shape>> &get_children();
    inline std::vector<K> &get_keys();

  public:
    tree_node(const cell<T, vec>&);

    inline const std::vector<tree_node<T, K, L, vec, cell, shape>> &get_children() const;
    inline const std::vector<K> &get_keys() const;
    inline const cell<T, vec> &get_cell() const;
    inline std::vector<size_t> &get_sub_ray_cache() const;
    inline bool point_inside(const vec<T>&) const;
    inline K size() const;
};

template <typename T, typename K, typename L, template <typename> class vec, template <typename, template <typename> class> class cell, template <typename, template <typename> class> class shape>
class tree
{
  private:
    std::vector<shape<T, vec>> _shapes;
    std::vector<K> _index_map;
    std::vector<size_t> _key_cache;
    std::vector<K> _sort_copy;
    mutable std::vector<uint_fast8_t> _sub_overlap;
    mutable bit_flag<K, L> _flags;
    mutable std::vector<std::pair<K, K>> _hits;
    mutable std::vector<std::pair<K, vec<T>>> _ray_hits;
    tree_node<T, K, L, vec, cell, shape> _root;
    vec<T> _lower_bound;
    vec<T> _upper_bound;
    K _depth;
    K _scale;
    vec<T> _cell_extent;
    bool _depth_override;
    size_t _flag_size;

    inline void build(tree_node<T, K, L, vec, cell, shape>&, const K);
    inline void create_keys();
    inline size_t get_sorting_key(const vec<T>&) const;
    inline void get_overlap(const tree_node<T, K, L, vec, cell, shape>&) const;
    inline void get_overlap(const tree_node<T, K, L, vec, cell, shape>&, const vec<T>&, const vec<T>&, const K) const;
    inline void get_pairs(const tree_node<T, K, L, vec, cell, shape>&) const;
    inline void get_pairs(const tree_node<T, K, L, vec, cell, shape>&, const K) const;
    inline void get_ray_intersect(const tree_node<T, K, L, vec, cell, shape>&, const ray<T, vec>&, const K) const;
    inline K optimize_depth(const std::vector<shape<T, vec>>&);
    inline void sort(const std::vector<shape<T, vec>>&);

  public:
    tree(const cell<T, vec>&);

    inline void resize(const cell<T, vec>&);
    inline void check_size(const std::vector<shape<T, vec>>&) const;
    inline vec<T> clamp_bounds(const vec<T>&) const;
    inline const vec<T> &get_lower_bound() const;
    inline const vec<T> &get_upper_bound() const;
    inline const tree_node<T, K, L, vec, cell, shape> &get_node(const vec<T>&) const;
    inline K get_scale() const;
    inline const std::vector<shape<T, vec>> &get_shapes();
    inline const std::vector<std::pair<K, K>> &get_collisions() const;
    inline const std::vector<std::pair<K, K>> &get_collisions(const vec<T>&) const;
    inline const std::vector<std::pair<K, vec<T>>> &get_collisions(const ray<T, vec>&) const;
    inline K get_depth() const;
    inline const std::vector<K> &get_index_map() const;
    inline const std::vector<std::pair<K, K>> &get_overlap(const shape<T, vec>&) const;
    bool inside(const vec<T>&) const;
    inline void insert(const std::vector<shape<T, vec>>&);
    inline void insert(const std::vector<shape<T, vec>>&, const K depth);
    inline void insert_no_sort(const std::vector<shape<T, vec>>&);
    inline const std::vector<K> &point_inside(const vec<T>&) const;
    inline void set_depth(const K depth);

};
}

#endif
