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
#ifndef __SOUND_BUFFER__
#define __SOUND_BUFFER__

#include <AL/al.h>
#include <AL/alc.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "file/min/ogg.h"
#include "file/min/wave.h"

namespace min
{

bool check_al_error();
void throw_al_error();

class sound_buffer
{
  private:
    ALCdevice *_device;
    ALCcontext *_context;
    std::vector<ALuint> _buffers;
    std::vector<ALuint> _sources;
    vec3<float> _listener;

    inline static ALenum al_format(const bool, const unsigned);
    inline size_t add_pcm_data(const ALvoid *const, const ALenum, const ALsizei, const ALsizei);
    inline void clear_error() const;
    inline void create_openal_context();
    inline void shutdown();

  public:
    sound_buffer();
    ~sound_buffer();
    sound_buffer(const sound_buffer&) = delete;

    inline void enumerate_devices() const;
    inline size_t add_source();
    inline size_t add_wave_pcm(const wave&);
    inline size_t add_ogg_pcm(const ogg&);
    inline void bind(const size_t, const size_t) const;
    inline bool check_error() const;
    inline void throw_internal_error() const;
    inline bool is_playing(const size_t) const;
    inline void play_async(const size_t) const;
    inline void stop_async(const size_t) const;
    inline void play_sync(const size_t) const;
    inline void set_distance_model(const ALenum model) const;
    inline void set_listener_position(const vec3<float>&);
    inline void set_listener_orientation(const vec3<float>&, const vec3<float>&) const;
    inline void set_listener_velocity(const vec3<float>&) const;
    inline void set_source_at_listener(const size_t) const;
    inline void set_source_direction(const size_t, const vec3<float>&) const;
    inline void set_source_gain(const size_t, const float) const;
    inline void set_source_loop(const size_t, const bool) const;
    inline void set_source_position(const size_t, const vec3<float>&) const;
    inline void set_source_max_dist(const size_t, const float) const;
    inline void set_source_pitch(const size_t, const float) const;
    inline void set_source_ref_dist(const size_t, const float) const;
    inline void set_source_rolloff(const size_t, const float) const;
    inline void set_source_velocity(const size_t, const vec3<float>&) const;

};
}

#endif
