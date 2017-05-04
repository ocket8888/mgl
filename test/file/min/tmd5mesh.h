/* Copyright [2013-2016] [Aaron Springstroh, Minimal Graphics Library]

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
#ifndef __TESTMD5MESH__
#define __TESTMD5MESH__

#include <min/test.h>
#include <min/md5_mesh.h>
#include <stdexcept>

bool test_md5_mesh()
{
    bool out = true;

    // Local variables
    const min::md5_mesh<float, uint16_t> md5 = min::md5_mesh<float, uint16_t>("data/models/bob.md5mesh");

    // Test the mesh count
    out = out && compare(1, md5.get_meshes().size());
    out = out && compare(376, md5.get_meshes()[0].vertex.size());
    out = out && compare(376, md5.get_meshes()[0].uv.size());
    out = out && compare(570, md5.get_meshes()[0].index.size());
    out = out && compare(376, md5.get_meshes()[0].bone_index.size());
    out = out && compare(376, md5.get_meshes()[0].bone_weight.size());
    if (!out)
    {
        throw std::runtime_error("Failed md5 load mesh");
    }

    // Check bones and joint sizes
    out = out && compare(15, md5.get_joints().size());
    if (!out)
    {
        throw std::runtime_error("Failed md5 joint/bone sizes");
    }

    return out;
}

#endif