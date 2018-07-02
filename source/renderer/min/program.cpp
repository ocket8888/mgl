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

#include "program.h"

inline void min::program::check_extensions() const
{
    const bool vp = GLEW_ARB_vertex_program;
    const bool fp = GLEW_ARB_fragment_program;

    // Check that we have the extensions we need
    if (!vp || !fp)
    {
        throw std::runtime_error("program: minimum extensions not met");
    }
}

inline void min::program::create_program()
{
    // Create program ID
    _id = glCreateProgram();
    if (_id == 0)
    {
        throw std::runtime_error("program: Failed to create program ID.");
    }
}

inline void min::program::link_program()
{
    // Link the program
    glLinkProgram(_id);

    // Check that the link was successful
    GLint status;
    glGetProgramiv(_id, GL_LINK_STATUS, &status);
    if (!status)
    {
        print_errors();
    }
}

void min::program::print_errors() const
{
    GLint log_length = 0;
    if (glIsProgram(_id))
    {
        // Get the shader log length
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_length);

        // Check that error exists
        if (log_length > 0)
        {
            // Create space for the log
            std::string log(log_length + 1, '\0');

            // Get the log data, NULL means we do not need the length since we queried for it
            glGetProgramInfoLog(_id, log_length, nullptr, &log[0]);

            // Handle the error
            throw std::runtime_error("program: Failed to link shader program. " + log);
        }
    }
    else
    {
        throw std::runtime_error("program: program id is invalid!");
    }

    // Default operation
    throw std::runtime_error("program: Failed to link shader program.");
}

min::program::program(const min::shader &vertex, const min::shader &fragment)
{
    // Check that all needed extensions are present
    check_extensions();

    // Create program id
    create_program();

    // Attach vertex and fragment shader
    glAttachShader(_id, vertex.id());
    glAttachShader(_id, fragment.id());

    // link program
    link_program();

    // Detach the shaders as we do not need them anymore
    glDetachShader(_id, vertex.id());
    glDetachShader(_id, fragment.id());
}

min::program::program(const std::vector<GLuint> &shaders)
{
    // Check that all needed extensions are present
    check_extensions();

    // Create program id
    create_program();

    // Attach all shaders
    for (const auto &shader_id : shaders)
    {
        // Attach vertex and fragment shader
        glAttachShader(_id, shader_id);
    }

    // link program
    link_program();

    // Detach the shaders as we do not need them anymore
    for (const auto &shader_id : shaders)
    {
        glDetachShader(_id, shader_id);
    }
}

min::program::~program()
{
    if (_id > 0)
    {
        // Disable the current program
        glUseProgram(0);

        // Delete the program
        glDeleteProgram(_id);
    }

    // check for errors
    throw_gl_error();
}

inline void min::program::use() const
{
    // Enable the program for use
    glUseProgram(_id);
}

inline GLuint min::program::id() const
{
    return _id;
}