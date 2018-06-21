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
