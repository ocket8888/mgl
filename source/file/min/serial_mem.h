#ifndef __SERIALIZE_MEM__
#define __SERIALIZE_MEM__

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "math/min/vec2.h"
#include "math/min/vec3.h"
#include "math/min/vec4.h"

// A CPU register is neither big or little endian
// However accessing bytes through a pointer is big or little endian specific
// Therefore we use bit shifts to hide the machine endianess from us, this also simplifies endianness issues
// A stream is declared either big or little endian and we convert the bytes to the host machine byte order using CPU register shifts
// Although this produces more instructions for the worst case,
// we do not have to constantly check the machine byte order through the program lifetime and this eliminates branching

namespace min
{

class mem_file
{
  private:
    std::vector<uint8_t> *const _data;
    size_t _offset;
    size_t _size;

  public:
    mem_file(std::vector<uint8_t> *const data, const size_t offset, const size_t size)
        : _data(data), _offset(offset), _size(size) {}

    const uint8_t &operator[](const size_t) const;
    uint8_t &operator[](const size_t);
    size_t offset() const;
    size_t size() const;
    std::string to_string() const;
};

template <typename T> T read_le(const mem_file&, size_t&);
template <typename T> T read_be(const mem_file&, size_t&);
template <typename T> std::vector<T> read_le_vector(const mem_file&, size_t&);
template <typename T> std::vector<T> read_be_vector(const mem_file&, size_t&);
template <typename T> min::vec2<T> read_le_vec2(const mem_file&, size_t&);
template <typename T> min::vec2<T> read_be_vec2(const mem_file&, size_t&);
template <typename T> min::vec3<T> read_le_vec3(const mem_file&, size_t&);
template <typename T> min::vec3<T> read_be_vec3(const mem_file&, size_t&);
template <typename T> min::vec4<T> read_le_vec4(const mem_file&, size_t&);
template <typename T> min::vec4<T> read_be_vec4(const mem_file&, size_t&);
template <typename T> std::vector<min::vec2<T>> read_le_vector_vec2(const mem_file&, size_t&);
template <typename T> std::vector<min::vec2<T>> read_be_vector_vec2(const mem_file&, size_t&);
template <typename T> std::vector<min::vec3<T>> read_le_vector_vec3(const mem_file&, size_t&);
template <typename T> std::vector<min::vec3<T>> read_be_vector_vec3(const mem_file&, size_t&);
template <typename T> std::vector<min::vec4<T>> read_le_vector_vec4(const mem_file&, size_t&);
template <typename T> std::vector<min::vec4<T>> read_be_vector_vec4(const mem_file&, size_t&);

}
#endif
