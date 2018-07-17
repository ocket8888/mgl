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
#ifndef TEXTBUFFER
#define TEXTBUFFER


#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "math/min/vec2.h"
#include "math/min/vec4.h"
#include "platform/min/window.h"

#include "program.h"

namespace min
{

class text
{
  private:
    std::string _str;
    vec2<float> _location;
    vec2<float> _line_wrap;
    size_t _offset;
    size_t _size;
    vec2<float> _pixel;

  public:
    text(const std::string&, const vec2<float>&, const size_t);

    const vec2<float> &line_wrap() const;
    void set_line_wrap(const vec2<float>&);
    const vec2<float> &location() const;
    void set_location(const vec2<float>&);
    size_t offset() const;
    const vec2<float> &pixel_size() const;
    void set_offset(const size_t);
    size_t size() const;
    const std::string &str() const;
    void set_pixel_size(const vec2<float>&);
    void set_string(const std::string&);
};

class text_buffer
{
  private:
    class char_data
    {
      public:
        float adv_x;    // advance.x
        float adv_y;    // advance.y
        float height;   // bitmap.rows;
        float width;    // bitmap.width;
        float left;     // bitmap_left;
        float top;      // bitmap_top;
        float offset_x; // x offset of glyph in texture coordinates
        float offset_y; // y offset of glyph in texture coordinates
    } _chars[256];
    unsigned _w;
    unsigned _h;
    GLuint _tid;
    GLuint _vao;
    std::vector<GLuint> _vbo;
    std::vector<text> _text;
    mutable std::vector<vec4<float>> _data;
    size_t _char_count;
    uint_fast16_t _screen_x;
    uint_fast16_t _screen_y;

    void bind_vao() const;
    void check_extensions() const;
    void create_vertex_buffer(const size_t);
    void create_texture_buffer(const unsigned, const unsigned);
    void calculate_texture_dimensions(const FT_Face&);
    void upload_character_glyphs(const FT_Face&);
    void create_texture_atlas(const FT_Face&);
    vec2<float> get_pixel_size(const text&) const;
    void process_text(const text&) const;
    vec2<float> to_screen_coords(const float, const float) const;
    vec2<float> to_pixel_coords(const vec2<float>&) const;
    void upload_data(const size_t) const;


  public:
    text_buffer(const std::string&, const int, const size_t = 1);
    ~text_buffer();
    text_buffer(const text_buffer&) = delete;

    size_t add_text(const std::string&, const float, const float);
    void bind(const size_t) const;
    void bind_buffer(const size_t) const;
    void unbind() const;
    void clear();
    void draw(const size_t) const;
    void draw_all() const;
    void draw_batch(const size_t) const;
    void draw(const size_t, const size_t) const;
    std::pair<uint_fast16_t, uint_fast16_t> get_screen_size() const;
    const vec2<float> get_text_location(const size_t) const;
    void reserve(const size_t);
    void set_texture_uniform(const program&, const std::string&, const size_t) const;
    void set_screen(const uint_fast16_t, const uint_fast16_t);
    void set_text(const size_t, const std::string&);
    void set_text_location(const size_t, const float, const float);
    void set_line_wrap(const size_t, const float, const float);
    void set_text(const size_t, const std::string&, const float, const float);
    void set_text_center(const size_t, const float, const float);
    void set_text_center(const size_t, const std::string&, const float, const float);
    size_t size() const;
    void upload(const size_t = 0) const;
    size_t upload_batch(const size_t, const std::vector<size_t>&) const;

};
}

#endif
