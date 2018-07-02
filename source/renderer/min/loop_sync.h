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
#ifndef __LOOPSYNC__
#define __LOOPSYNC__

#include <chrono>
#include <cmath>
#include <thread>

namespace min
{

class loop_sync
{
  private:
    // History buffer
    static constexpr uint_fast8_t _error_count = 16;
    double _error[_error_count];
    double _idle[_error_count];
    uint_fast8_t _begin;
    uint_fast8_t _end;
    double _ie;
    double _idle_time;
    double _de;

    // Time properties
    std::chrono::high_resolution_clock::time_point _start;
    std::chrono::high_resolution_clock::time_point _current_time;

    // Control properties
    double _accum_time;
    double _set_point;
    double _kp;
    double _ki;
    double _kd;
    double _dt;

    inline void calculate_control_parameters(const double);
    inline double calculate_delay(const double) const;
    inline double diff();

  public:
    loop_sync(const double, const double = 0.5, const double = 0.75, const double = 0.75);

    inline double get_fps() const;
    inline double idle() const;
    inline void start();
    double sync();

};
}

#endif
