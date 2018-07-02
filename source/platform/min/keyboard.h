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
#ifndef __KEYBOARD__
#define __KEYBOARD__

#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace min
{

template <typename K>
class key
{
  private:
    bool _is_down;
    void *_down_data;
    void *_down_data_pf;
    void *_up_data;
    void *_up_data_pf;
    void (*_key_down)(void *, K);
    void (*_key_down_pf)(void *, K);
    void (*_key_up)(void *, K);
    void (*_key_up_pf)(void *, K);

  public:
    key();

    void down(K);
    void up(K);
    bool is_down() const;
    void set_down(void(*)(void*, K), void*);
    void set_up(void(*)(void*, K), void*);
    void set_down_per_frame(void(*)(void*, K), void*);
    void set_up_per_frame(void(*)(void*, K), void*);
    void update(const K) const;
};

template <typename T, typename K>
class keyboard
{
  private:
    std::unordered_map<T, key<K>> _keys;
    std::vector<T> _key_order;
    void *_override_data;
    void (*_override)(void *, T);
    bool _disable;

  public:
    keyboard();

    void add(const T);
    void clear();
    void disable();
    void enable();
    const std::vector<T> &get_active_keys() const;
    const std::pair<T, key<K>> &get_key(const T) const;
    bool is_down(const T) const;
    void key_down(const T, const K);
    void key_up(const T, const K);
    void register_keydown(const T, void(*)(void*, K), void*);
    void register_keyup(const T, void(*)(void*, K), void*);
    void register_keydown_per_frame(const T, void(*)(void*, K), void*);
    void register_keyup_per_frame(const T, void(*)(void*, K), void*);
    void register_override_keyup(void(*)(void*, T), void*);
    bool swap(const T, const T);
    void update(const K);

};
}

#endif
