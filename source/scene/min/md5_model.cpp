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

#include "md5_model.h"


template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
void min::md5_model<T,K,vec,bound>::check_bones()
{
    // Check size of animated components
    for (const auto &m : this->_mesh)
    {
        const size_t size = m.vertex.size();
        if (m.bone_index.size() != size || m.bone_weight.size() != size)
        {
            throw std::runtime_error("md5_model: bone parameters are not of appropriate length");
        }
    }
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
void min::md5_model<T,K,vec,bound>::make_bind_pose(const std::vector<min::md5_joint<T>> &joints)
{
    // Allocate bone matrices
    _inverse_bp.reserve(joints.size());
    _bones.resize(joints.size());

    // Calculate the inverse bind pose matrices
    for (const auto &joint : joints)
    {
        // Create inverse transformation matrix
        mat4<T> bone(joint.get_position(), joint.get_rotation());

        // Check if matrix has an inverse
        if (!bone.invert())
        {
            throw std::runtime_error("md5_model: uninvertable matrix found while calculating inverse bind-pose");
        }

        // Save the inverse matrix for interpolation
        _inverse_bp.push_back(bone);
    }
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
min::md5_model<T,K,vec,bound>::md5_model(md5_mesh<T, K> &&m)
    : model<T, K, vec, bound>(std::move(m.get_meshes())), _current(0)
{
    // Joints are thrown away after this
    make_bind_pose(m.get_joints());

    // Check the mesh bone dimensions
    check_bones();
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
min::md5_model<T,K,vec,bound>::md5_model(const min::md5_mesh<T, K> &m)
    : model<T, K, vec, bound>(m.get_meshes()), _current(0)
{
    // Joints are thrown away after this
    make_bind_pose(m.get_joints());

    // Check the mesh bone dimensions
    check_bones();
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
const std::vector<min::mat4<T>> &min::md5_model<T,K,vec,bound>::get_bones() const
{
    return _bones;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
const min::md5_anim<T> &min::md5_model<T,K,vec,bound>::get_current_animation() const
{
    return _animations[_current];
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
bool min::md5_model<T,K,vec,bound>::is_animating() const
{
    // If we loaded an animation file
    if (_animations.size() > 0)
    {
        return (_animations[_current].get_loop_count() != 0);
    }

    // We haven't loaded any animations
    return false;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
size_t min::md5_model<T,K,vec,bound>::load_animation(const std::string &file)
{
    // Load animation in place
    _animations.emplace_back(file);

    // Update the current animation
    _current = _animations.size() - 1;

    // Get current frame of animation
    const std::vector<mat4<T>> &frame = _animations[_current].get_current_frame();

    // Validate that the animation frame size matches the model bones
    const size_t size = frame.size();
    if (_bones.size() != size || _inverse_bp.size() != size)
    {
        throw std::runtime_error("md5_model: animation is not compatible with model");
    }

    // return current animation index
    return _current;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
size_t min::md5_model<T,K,vec,bound>::load_animation(const mem_file &mem)
{
    // Load animation in place
    _animations.emplace_back(mem);

    // Update the current animation
    _current = _animations.size() - 1;

    // Get current frame of animation
    const std::vector<mat4<T>> &frame = _animations[_current].get_current_frame();

    // Validate that the animation frame size matches the model bones
    const size_t size = frame.size();
    if (_bones.size() != size || _inverse_bp.size() != size)
    {
        throw std::runtime_error("md5_model: animation is not compatible with model");
    }

    // return current animation index
    return _current;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
void min::md5_model<T,K,vec,bound>::reset_bones() const
{
    // Reset bones to mind bose
    for (auto &b : _bones)
    {
        b = mat4<T>();
    }
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
void min::md5_model<T,K,vec,bound>::set_current_animation(const size_t animation)
{
    // Set animation with index
    _current = animation;
}

template <typename T, typename K, template <typename> class vec, template <typename, template <typename> class> class bound>
void min::md5_model<T,K,vec,bound>::step(const T time) const
{
    // Check if we loaded an animation
    if (_animations.size() == 0)
    {
        throw std::runtime_error("md5_model: no animations are loaded");
    }

    // Get the current animation
    const md5_anim<T> &anim = _animations[_current];

    // Step the animation
    anim.step(time);

    // Get current frame of animation
    const std::vector<mat4<T>> &frame = anim.get_current_frame();

    // Check frame size, consider removing since we checked on load
    if (frame.size() != _bones.size())
    {
        throw std::runtime_error("md5_model: animation is not compatible with model");
    }

    // Update the model bones
    const size_t size = frame.size();
    for (size_t i = 0; i < size; i++)
    {
        // Transform the bone based off animation frame
        _bones[i] = _inverse_bp[i] * frame[i];
    }
}
