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
#ifndef __SAMPLE__
#define __SAMPLE__

namespace min
{

template <typename T, template <typename> class vec>
class sample
{
  private:
    vec<T> _src;
    vec<T> _dst;
    T _t;
    T _weight;

  public:
    sample() : _t(0.0), _weight(1.0) {}
    sample(const vec<T> &src, const vec<T> &dst) : _src(src), _dst(dst), _t(0.0), _weight(1.0) {}
    sample(const vec<T> &src, const vec<T> &dst, const T weight) : _src(src), _dst(dst), _t(0.0), _weight(weight) {}

    bool done() const;
    const vec<T> &get_start() const;
    const vec<T> &get_dest() const;
    vec<T> interpolate(const T);
    vec<T> weight_interpolate(const T);
    vec<T> lerp(const T);
    vec<T> weight_lerp(const T);
    void reset();
};
}

#endif
