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
#include "md5_anim.h"

//// md5_node ////

const std::string &min::md5_node::get_name() const
{
	return _name;
}

int min::md5_node::get_parent() const
{
    return _parent;
}

int min::md5_node::get_flag() const
{
    return _flag;
}

unsigned min::md5_node::get_start() const
{
    return _start;
}


//// md5_transform ////

template<class T> min::md5_transform<T>::md5_transform(const min::vec3<T> &t, const min::quat<T> &r) : _position(t), _rotation(r)
{
    _rotation.calculate_w();
}

template<class T> const min::vec3<T> &min::md5_transform<T>::get_position() const
{
    return _position;
}

template<class T> const min::quat<T> &min::md5_transform<T>::get_rotation() const
{
    return _rotation;
}


//// md5_animated_node ////
template<class T> min::md5_animated_node<T>::md5_animated_node(const md5_transform<T> &copy, int parent)
    : _parent(parent), _position(copy.get_position()), _rotation(copy.get_rotation()) {}

template<class T> int min::md5_animated_node<T>::get_parent() const
{
    return _parent;
}
template<class T> min::vec3<T> &min::md5_animated_node<T>::get_position()
{
    return _position;
}
template<class T> const min::vec3<T> &min::md5_animated_node<T>::get_position() const
{
    return _position;
}
template<class T> min::quat<T> &min::md5_animated_node<T>::get_rotation()
{
    return _rotation;
}
template<class T> const min::quat<T> &min::md5_animated_node<T>::get_rotation() const
{
    return _rotation;
}


//// md5_frame_data ////

template <class T>
void min::md5_frame_data<T>::add(const T data)
{
    _data.push_back(data);
}

template <class T>
const std::vector<T> &min::md5_frame_data<T>::get_data() const
{
    return _data;
}

template <class T>
void min::md5_frame_data<T>::reserve(size_t n)
{
    _data.reserve(n);
}


//// md5_frame ////

template <class T>
void min::md5_frame<T>::add_node(const min::md5_animated_node<T> &node, const min::mat4<T> &bone)
{
    _nodes.push_back(node);
    _bones.push_back(bone);
}

template <class T>
const min::md5_animated_node<T> &min::md5_frame<T>::get_node(const int parent_id) const
{
    return _nodes[parent_id];
}

template <class T>
const std::vector<min::mat4<T>> &min::md5_frame<T>::get_bones() const
{
    return _bones;
}

template <class T>
void min::md5_frame<T>::reserve(size_t n)
{
    _bones.reserve(n);
    _nodes.reserve(n);
}


//// md5_anim ////

template <typename T>
void min::md5_anim<T>::load_file(const std::string _file)
{
    std::ifstream file(_file, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        // Get the size of the file
        const auto size = file.tellg();

        // Adjust file pointer to beginning
        file.seekg(0, std::ios::beg);

        // Allocate space for new file
        std::string data(size, 0);

        // Read bytes and close the file
        file.read(&data[0], size);

        // Close the file
        file.close();

        // Process the DDS file
        load(data);
    }
    else
    {
        throw std::runtime_error("md5_anim: Could not load file '" + _file + "'");
    }
}

template <typename T>
void min::md5_anim<T>::load(const std::string &data)
{
    // Get locations of all lines in string buffer
    const auto lines = min::read_lines(data);

    // Read line by line
    unsigned frames, nodes, components;
    frames = nodes = components = 0;
    const size_t size = lines.size();
    for (size_t i = 0; i < size; i++)
    {
        // read line and trim the line whitespace
        const auto &position = lines[i];
        std::string line = data.substr(position.first, position.second);
        min::trim(line);

        // skip empty line size in bytes
        if (line.size() == 0)
        {
            continue;
        }

        // Read the property of this line
        std::string field;
        std::istringstream s(line);
        s >> field;

        // Execute action based on property
        if (min::to_lower(field).compare("md5version") == 0)
        {
            // Parse the version number and check against target
            int version;
            s >> version;

            // Expected version 10
            if (version != 10)
            {
                throw std::runtime_error("md5_anim: Version number unsupported, got '" + std::to_string(version) + "' expected '10'");
            }
        }
        else if (min::to_lower(field).compare("commandline") == 0)
        {
            // Skip these command parameters, as they are unused
            continue;
        }
        else if (min::to_lower(field).compare("numframes") == 0)
        {
            s >> frames;
        }
        else if (min::to_lower(field).compare("numjoints") == 0)
        {
            s >> nodes;
        }
        else if (min::to_lower(field).compare("framerate") == 0)
        {
            s >> _frame_rate;
        }
        else if (min::to_lower(field).compare("numanimatedcomponents") == 0)
        {
            s >> components;
        }
        else if (min::to_lower(field).compare("hierarchy") == 0)
        {
            // Parse the next 'nodes' lines in the file
            const std::vector<std::string> h_lines = min::get_lines(data, lines, nodes, i);

            // process the hierarchy data
            process_hierarchy(h_lines);
        }
        else if (min::to_lower(field).compare("bounds") == 0)
        {
            // Parse the next 'frames' lines in the file
            const std::vector<std::string> b_lines = min::get_lines(data, lines, frames, i);

            // process the bounds data
            process_bounds(b_lines);
        }
        else if (min::to_lower(field).compare("baseframe") == 0)
        {
            // Parse the next 'nodes' lines in the file
            const std::vector<std::string> bf_lines = min::get_lines(data, lines, nodes, i);

            // process the baseframe data
            process_baseframe(bf_lines);
        }
        else if (min::to_lower(field).compare("frame") == 0)
        {
            // Get the frame ID
            unsigned id;
            s >> id;

            // Parse the next 'nodes' lines in the file
            const std::vector<std::string> f_lines = min::get_lines(data, lines, nodes, i);

            // process the frame data
            process_frame_data(f_lines, id, components);
        }
        else if (field.compare("}") != 0)
        {
            // There is another command that we do not know about
            throw std::runtime_error("md5_anim: unknown property '" + field + "'");
        }

        // Check for errors
        if (s.fail())
        {
            throw std::runtime_error("md5_anim: Invalid line in file '" + line + "'");
        }
    }
}

template <typename T>
void min::md5_anim<T>::interpolate_current_frame(const min::md5_frame<T> &frame0, const min::md5_frame<T> &frame1, T ratio) const
{
    const size_t size = _nodes.size();
    for (size_t i = 0; i < size; i++)
    {
        // lerp interpolation for position
        const vec3<T> &from_position = frame0.get_node(i).get_position();
        const vec3<T> &to_position = frame1.get_node(i).get_position();
        const vec3<T> position = vec3<T>::lerp(from_position, to_position, ratio);

        // slerp interpolation for rotation
        const quat<T> &from_rotation = frame0.get_node(i).get_rotation();
        const quat<T> &to_rotation = frame1.get_node(i).get_rotation();
        const quat<T> rotation = quat<T>::slerp(from_rotation, to_rotation, ratio);

        // Set the interpolate position and rotation
        _current_frame[i] = mat4<T>(position, rotation);
    }
}

template <typename T>
void min::md5_anim<T>::process_hierarchy(const std::vector<std::string> &lines)
{
    if (lines.size() > 0)
    {
        _nodes.reserve(lines.size());
        for (const auto &line : lines)
        {
            std::string name;
            int parent, flag;
            unsigned start;

            // Parse the items on the line
            std::istringstream s(line);
            s >> name;
            s >> parent;
            s >> flag;
            s >> start;

            // Remove quotes from string name
            name.erase(std::remove(name.begin(), name.end(), '\"'), name.end());

            // Add the node to the hierarchy
            _nodes.emplace_back(name, parent, flag, start);

            // Check for errors
            if (s.fail())
            {
                throw std::runtime_error("md5_anim: invalid hierarchy line: '" + line + "'");
            }
        }
    }
    else
    {
        throw std::runtime_error("md5_anim: no md5_nodes specified in animation file");
    }
}

template <typename T>
void min::md5_anim<T>::process_bounds(const std::vector<std::string> &lines)
{
    if (lines.size() > 0)
    {
        _bounds.reserve(lines.size());
        for (const auto &line : lines)
        {
            std::string junk;
            T minx, miny, minz;
            T maxx, maxy, maxz;

            // Parse the items on the line
            std::istringstream s(line);

            // Skip the '('
            s >> junk;

            // Read the min bounds
            s >> minx;
            s >> miny;
            s >> minz;

            // Skip the ')' and '('
            s >> junk;
            s >> junk;

            // Read the max bounds
            s >> maxx;
            s >> maxy;
            s >> maxz;

            // Add the bound to the bounds list
            _bounds.emplace_back(vec3<T>(minx, miny, minz), vec3<T>(maxx, maxy, maxz));

            // Check for errors
            if (s.fail())
            {
                throw std::runtime_error("md5_anim: invalid bound line: '" + line + "'");
            }
        }
    }
    else
    {
        throw std::runtime_error("md5_anim: no bounds specified in animation file");
    }
}

template <typename T>
void min::md5_anim<T>::process_baseframe(const std::vector<std::string> &lines)
{
    if (lines.size() > 0)
    {
        _transforms.reserve(lines.size());
        for (const auto &line : lines)
        {
            std::string junk;
            T x, y, z;
            T qx, qy, qz;

            // Parse the items on the line
            std::istringstream s(line);

            // Skip the '('
            s >> junk;

            // Read the translation
            s >> x;
            s >> y;
            s >> z;

            // Skip the ')' and '('
            s >> junk;
            s >> junk;

            // Read the rotation
            s >> qx;
            s >> qy;
            s >> qz;

            // Add the transform to the base frame transform buffer, we are not normalizing here!
            _transforms.emplace_back(vec3<T>(x, y, z), quat<T>(0.0, qx, qy, qz));

            // Check for errors
            if (s.fail())
            {
                throw std::runtime_error("md5_anim: invalid baseframe line: '" + line + "'");
            }
        }
    }
    else
    {
        throw std::runtime_error("md5_anim: no baseframe transforms specified in animation file");
    }
}

template <typename T>
void min::md5_anim<T>::process_frame_data(const std::vector<std::string> &lines, const unsigned frame_id, const unsigned components)
{
    if (lines.size() > 0)
    {
        // Check that the number of components divides evenly, and the ratio is 6
        if (components % lines.size() != 0 && components / lines.size() == 6)
        {
            throw std::runtime_error("md5_anim: unexpected component count");
        }

        // Create the frame for this id and reserve space
        md5_frame_data<T> frame_data(frame_id);
        frame_data.reserve(components);

        unsigned count = 0;
        for (const auto &line : lines)
        {
            std::istringstream s(line);
            T data;

            // Read all of the components on this line
            for (size_t i = 0; i < 6; i++)
            {
                s >> data;
                frame_data.add(data);
                count++;
            }

            // Check for errors
            if (s.fail())
            {
                throw std::runtime_error("md5_anim: invalid frame_data line: '" + line + "'");
            }
        }

        // Check that the component count matches expected
        if (count != components)
        {
            throw std::runtime_error("md5_anim: invalid frame component count, expected '" + std::to_string(components) + "' got '" + std::to_string(count));
        }

        // Add this frame to the frame list
        _frame_data.push_back(frame_data);

        // Create a frame from the frame data
        process_frame(frame_data);
    }
    else
    {
        throw std::runtime_error("md5_anim: no frame data specified in animation file");
    }
}

template <typename T>
void min::md5_anim<T>::process_frame(const min::md5_frame_data<T> &frame_data)
{
    if (_nodes.size() != _transforms.size())
    {
        throw std::runtime_error("md5_anim: node-transform mismatch");
    }

    // Create frame from input data and reserve space
    md5_frame<T> frame;
    frame.reserve(_nodes.size());

    // Transform all the nodes in the hierarchy
    const size_t size = _nodes.size();
    for (size_t i = 0; i < size; i++)
    {
        const md5_node &node = _nodes[i];
        const md5_transform<T> &transform = _transforms[i];
        const std::vector<T> &data = frame_data.get_data();

        // Transform the node, down the parent tree
        md5_animated_node<T> child(transform, node.get_parent());

        size_t j = 0;
        const int flag = node.get_flag();

        // Check for start overflow
        const unsigned start = node.get_start();
        if (start >= data.size())
        {
            throw std::runtime_error("md5_anim: frame data overflow");
        }

        // Set the position for this frame
        vec3<T> &position = child.get_position();
        if (flag & 1)
        {
            position.x(data[start + j++]);
        }
        if (flag & 2)
        {
            position.y(data[start + j++]);
        }
        if (flag & 4)
        {
            position.z(data[start + j++]);
        }

        // Set the animated rotation
        quat<T> &rotation = child.get_rotation();
        if (flag & 8)
        {
            rotation.x(data[start + j++]);
        }
        if (flag & 16)
        {
            rotation.y(data[start + j++]);
        }
        if (flag & 32)
        {
            rotation.z(data[start + j++]);
        }

        // Calculate W since we changed components, this normalizes the quaternion
        // We need to negate rotation angle here since we use left-handed coordinates
        rotation = rotation.conjugate();
        rotation.calculate_w();

        // If this node has a parent and is valid
        const int parent = child.get_parent();
        if (parent >= (int)size)
        {
            throw std::runtime_error("md5_anim: parent overflow '" + std::to_string(parent) + " is greater than '" + std::to_string(size) + "'");
        }
        else if (parent >= 0)
        {
            // Transform the child position based off parent rotation
            const md5_animated_node<T> &parent_node = frame.get_node(parent);
            const vec3<T> rotated = parent_node.get_rotation().transform(position);

            // Translate position by parent position
            position = parent_node.get_position() + rotated;

            // Rotate rotation by parent rotation
            rotation = parent_node.get_rotation() * rotation;
        }

        // Create matrix bone for this node
        const mat4<T> bone(position, rotation);

        // Add this node to the frame
        frame.add_node(child, bone);
    }

    // Add this frame to the frame list
    _frames.push_back(frame);
}

template <typename T>
min::md5_anim<T>::md5_anim(const std::string &file) : _frame_rate(0), _loops(0), _time(0.0)
{
    load_file(file);

    // Set the length of the animation
    _animation_length = static_cast<T>(_frames.size()) / _frame_rate;

    if (_frames.size() == 0)
    {
        throw std::runtime_error("md5_anim: no frames in animation");
    }

    // Set the current frame
    _current_frame = _frames[0].get_bones();
}

template <typename T>
min::md5_anim<T>::md5_anim(const min::mem_file &mem) : _frame_rate(0), _loops(0), _time(0.0)
{
    load(mem.to_string());

    // Set the length of the animation
    _animation_length = static_cast<T>(_frames.size()) / _frame_rate;

    if (_frames.size() == 0)
    {
        throw std::runtime_error("md5_anim: no frames in animation");
    }

    // Set the current frame
    _current_frame = _frames[0].get_bones();
}

template <typename T>
const std::vector<min::aabbox<T, min::vec3>> &min::md5_anim<T>::get_bounds() const
{
    return _bounds;
}

template <typename T>
const std::vector<min::mat4<T>> &min::md5_anim<T>::get_current_frame() const
{
    return _current_frame;
}

template <typename T>
unsigned min::md5_anim<T>::get_frame_rate() const
{
    return _frame_rate;
}

template <typename T>
const std::vector<min::md5_frame_data<T>> &min::md5_anim<T>::get_frame_data() const
{
    return _frame_data;
}

template <typename T>
const std::vector<min::md5_frame<T>> &min::md5_anim<T>::get_frames() const
{
    return _frames;
}

template <typename T>
const std::vector<min::md5_node> &min::md5_anim<T>::get_nodes() const
{
    return _nodes;
}

template <typename T>
const std::vector<min::md5_transform<T>> &min::md5_anim<T>::get_transforms() const
{
    return _transforms;
}

template <typename T>
unsigned min::md5_anim<T>::get_loop_count() const
{
    return _loops;
}

template <typename T>
void min::md5_anim<T>::set_loop_count(const unsigned count) const
{
    // Reset loop count
    _loops = count;
}

template <typename T>
void min::md5_anim<T>::set_time(const T time) const
{
    // Reset the animation time
    _time = time;
}

template <typename T>
void min::md5_anim<T>::step(const T step) const
{
    // Accumulate the time
    _time += step;

    // If time exceeds the animation length, loop
    if (_time >= _animation_length && _loops > 0)
    {
        // Decriment loop count
        _loops--;

        // Calculate time in next loop
        _time = std::fmod(_time, _animation_length);
    }

    // If we ran out of loops, exit
    if (_loops == 0)
    {
        return;
    }

    // Calculate the two frames to interpolate between
    const T frame_time = _time * _frame_rate;
    const unsigned frame_low = std::floor(frame_time);
    const unsigned frame_high = frame_low + 1;

    // Calculate position between the two frames for interpolation
    const T ratio = frame_time - frame_low;

    const size_t frames = _frames.size();
    const unsigned frame0 = frame_low % frames;
    const unsigned frame1 = frame_high % frames;

    // Interpolate between the two frames
    interpolate_current_frame(_frames[frame0], _frames[frame1], ratio);
}
