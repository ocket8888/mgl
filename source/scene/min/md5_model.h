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
#ifndef __MD5_MODEL__
#define __MD5_MODEL__

#include <vector>

#include "file/min/md5_anim.h"
#include "file/min/md5_mesh.h"
#include "model.h"

namespace min
{

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
class md5_model : public model<T, K, vec, bound>
{
  protected:
    std::vector<mat4<T>> _inverse_bp;
    mutable std::vector<mat4<T>> _bones;
    std::vector<md5_anim<T>> _animations;
    size_t _current;

    inline void check_bones();
    inline void make_bind_pose(const std::vector<md5_joint<T>>&);


  public:
    // This will steal data from provider
    md5_model(md5_mesh<T, K>&&);
    md5_model(const md5_mesh<T, K>&);

    inline const std::vector<mat4<T>> &get_bones() const;
    inline const md5_anim<T> &get_current_animation() const;
    inline bool is_animating() const;
    inline size_t load_animation(const std::string&);
    inline size_t load_animation(const mem_file&);
    inline void reset_bones() const;
    inline void set_current_animation(const size_t);
    inline void step(const T) const;

};
}

#endif
