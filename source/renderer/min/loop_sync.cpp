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

#include "loop_sync.h"

void min::loop_sync::calculate_control_parameters(const double idle_time)
{
    // Drop off oldest record from buffer
    _begin = (_begin + 1) % _error_count;

    // Create new record on buffer
    const double prev = _error[_end];
    _end = (_end + 1) % _error_count;

    // Calculate integral of errors
    _ie = _error[_end] = _set_point - _dt;
    for (uint_fast8_t i = _begin; i != _end; i = (i + 1) % _error_count)
    {
        _ie += _error[i];
    }

    // Calculate integral of idle time per frame
    _idle_time = _idle[_end] = idle_time;
    for (uint_fast8_t i = _begin; i != _end; i = (i + 1) % _error_count)
    {
        _idle_time += _idle[i];
    }

    // Calculate the derivative of errors
    _de = (_error[_end] - prev);
}

double min::loop_sync::calculate_delay(const double idle_time) const
{
    // Calculate the delay using PID equation
    const double p = _kp * _error[_end];
    const double i = _ki * _ie;
    const double d = _kd * _de;
    double delay = idle_time + (p + i + d);

    // Check if delay is unstable
    if (delay > _set_point)
    {
        delay = _set_point;
    }

    // return delay
    return delay;
}

double min::loop_sync::diff()
{
    // Calculate current time
    _current_time = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double>(_current_time - _start).count();
}

min::loop_sync::loop_sync(const double fps, const double kp, const double ki, const double kd)
    : _error{}, _idle{}, _begin(0), _end(_error_count - 1),
      _ie(0.0), _idle_time(0.0), _de(0.0),
      _accum_time(0.0), _set_point(1.0 / fps),
      _kp(kp), _ki(ki), _kd(kd), _dt(0.0) {}


double min::loop_sync::get_fps() const
{
    // Return the average fps
    return _error_count / (_set_point * _error_count - _ie);
}

double min::loop_sync::idle() const
{
    // return the average idle time
    return (_idle_time * 100.0) / (_set_point * _error_count);
}

void min::loop_sync::start()
{
    _start = std::chrono::high_resolution_clock::now();
}

double min::loop_sync::sync()
{
    // Calculate time spent rendering
    _dt = diff();
    const double idle_time = _set_point - _dt;

    // Calculate the delay using PID equation and account for accumulated time
    const double adjust_delay = calculate_delay(idle_time) - _accum_time;

    // If we have time to kill, sleep on it
    if (adjust_delay > 0.0)
    {
        // Sleep for the calculated delay
        std::this_thread::sleep_for(std::chrono::duration<double>(adjust_delay));

        // Get time spent sleeping in seconds
        _dt = diff();

        // Set the accumulation if we over-slept
        _accum_time = _dt - adjust_delay;
    }
    else
    {
        // We took too long rendering, accumulate this time
        _accum_time -= idle_time;
    }

    // Calculate PID variables
    calculate_control_parameters(idle_time);

    // Return the frame time step in seconds
    return _dt;
}
