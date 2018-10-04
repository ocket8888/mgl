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
#include <iostream>

#include "file/min/tbmp.h"
#include "file/min/tdds.h"
#include "file/min/tmd5anim.h"
#include "file/min/tmd5mesh.h"
#include "file/min/tmem_chunk.h"
#include "file/min/tserial.h"
#include "file/min/twavefront.h"
#include "geom/min/taabbox.h"
#include "geom/min/taabboxinter.h"
#include "geom/min/taabbresolve.h"
#include "geom/min/tfrustinter.h"
#include "geom/min/tfrustum.h"
#include "geom/min/toobbox.h"
#include "geom/min/toobboxinter.h"
#include "geom/min/toobbresolve.h"
#include "geom/min/tplane.h"
#include "geom/min/tray.h"
#include "geom/min/tsphere.h"
#include "geom/min/tsphinter.h"
#include "geom/min/tsphresolve.h"
#include "math/min/tbitflag.h"
#include "math/min/tcubic.h"
#include "math/min/tmat2.h"
#include "math/min/tmat3.h"
#include "math/min/tmat4.h"
#include "math/min/tquat.h"
#include "math/min/tsample.h"
#include "math/min/ttran2.h"
#include "math/min/ttran3.h"
#include "math/min/tuint_sort.h"
#include "math/min/tvec2.h"
#include "math/min/tvec3.h"
#include "math/min/tvec4.h"
#include "scene/min/taabbgrid.h"
#include "scene/min/taabbtree.h"
#include "scene/min/tcamera.h"
#include "scene/min/tmd5model.h"
#include "scene/min/tmodel.h"
#include "scene/min/tphysics.h"
#include "scene/min/traygrid.h"
#include "scene/min/traytree.h"
#include "scene/min/tsphgrid.h"
#include "scene/min/tsphtree.h"

int main()
{
    try
    {
        bool out = true;
        out = out && test_vec2();
        out = out && test_vec3();
        out = out && test_vec4();
        out = out && test_quat();
        out = out && test_mat2();
        out = out && test_mat3();
        out = out && test_mat4();
        out = out && test_tran2();
        out = out && test_tran3();
        out = out && test_bezier_cubic();
        out = out && test_bspline_cubic();
        out = out && test_hermite_cubic();
        out = out && test_plane();
        out = out && test_ray();
        out = out && test_ray_grid();
        out = out && test_ray_tree();
        out = out && test_sphere();
        out = out && test_aabbox();
        out = out && test_oobbox();
        out = out && test_sphere_intersect();
        out = out && test_sphere_resolve();
        out = out && test_aabbox_intersect();
        out = out && test_aabb_resolve();
        out = out && test_oobbox_intersect();
        out = out && test_oobb_resolve();
        out = out && test_frustum();
        out = out && test_frustum_intersect();
        out = out && test_camera();
        out = out && test_sample();
        out = out && test_wavefront();
        out = out && test_model();
        out = out && test_bmp();
        out = out && test_dds();
        out = out && test_aabb_tree();
        out = out && test_sphere_tree();
        out = out && test_bit_flag();
        out = out && test_uint_sort();
        out = out && test_aabb_grid();
        out = out && test_sphere_grid();
        out = out && test_md5_anim();
        out = out && test_md5_mesh();
        out = out && test_md5_model();
        out = out && test_physics_aabb_grid();
        out = out && test_serial();
        out = out && test_mem_chunk();
        if (out)
        {
            std::cout << "Graphics tests passed!" << std::endl;
            return 0;
        }
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    std::cout << "Graphics tests failed!" << std::endl;
    return -1;
}
