#ifndef SERIALIZE
#define SERIALIZE

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "math/min/vec2.h"
#include "math/min/vec3.h"
#include "math/min/vec4.h"

#include "serial_mem.h"

// A CPU register is neither big or little endian
// However accessing bytes through a pointer is big or little endian specific
// Therefore we use bit shifts to hide the machine endianess from us, this also simplifies endianness issues
// A stream is declared either big or little endian and we convert the bytes to the host machine byte order using CPU register shifts
// Although this produces more instructions for the worst case,
// we do not have to constantly check the machine byte order through the program lifetime and this eliminates branching

namespace min
{
template <typename T> T read_le(const std::vector<uint8_t>&, size_t&);
template <typename T> T read_be(const std::vector<uint8_t>&, size_t&);
template <typename T> void write_le(std::vector<uint8_t>&, const T);
template <typename T> void write_be(std::vector<uint8_t>&, const T);
template <typename T> void write_le(std::vector<uint8_t>&, const T, const size_t);
template <typename T> void write_be(std::vector<uint8_t>&, const T, const size_t);
template <typename T> std::vector<T> read_le_vector(const std::vector<uint8_t>&, size_t&);
template <typename T> std::vector<T> read_be_vector(const std::vector<uint8_t>&, size_t&);
template <typename T> void write_le_vector(std::vector<uint8_t>&, const std::vector<T>&);
template <typename T> void write_be_vector(std::vector<uint8_t>&, const std::vector<T>&);
template <typename T> min::vec2<T> read_le_vec2(const std::vector<uint8_t>&, size_t&);
template <typename T> min::vec2<T> read_be_vec2(const std::vector<uint8_t>&, size_t&);
template <typename T> min::vec3<T> read_le_vec3(const std::vector<uint8_t>&, size_t&);
template <typename T> min::vec3<T> read_be_vec3(const std::vector<uint8_t>&, size_t&);
template <typename T> min::vec4<T> read_le_vec4(const std::vector<uint8_t>&, size_t&);
template <typename T> min::vec4<T> read_be_vec4(const std::vector<uint8_t>&, size_t&);
template <typename T> void write_le_vec2(std::vector<uint8_t>&, const min::vec2<T> &v);
template <typename T> void write_be_vec2(std::vector<uint8_t>&, const min::vec2<T> &v);
template <typename T> void write_le_vec3(std::vector<uint8_t>&, const min::vec3<T> &v);
template <typename T> void write_be_vec3(std::vector<uint8_t>&, const min::vec3<T> &v);
template <typename T> void write_le_vec4(std::vector<uint8_t>&, const min::vec4<T> &v);
template <typename T> void write_be_vec4(std::vector<uint8_t>&, const min::vec4<T> &v);
template <typename T> std::vector<min::vec2<T>> read_le_vector_vec2(const std::vector<uint8_t>&, size_t&);
template <typename T> std::vector<min::vec2<T>> read_be_vector_vec2(const std::vector<uint8_t>&, size_t&);
template <typename T> std::vector<min::vec3<T>> read_le_vector_vec3(const std::vector<uint8_t>&, size_t&);
template <typename T> std::vector<min::vec3<T>> read_be_vector_vec3(const std::vector<uint8_t>&, size_t&);
template <typename T> std::vector<min::vec4<T>> read_le_vector_vec4(const std::vector<uint8_t>&, size_t&);
template <typename T> std::vector<min::vec4<T>> read_be_vector_vec4(const std::vector<uint8_t>&, size_t&);
template <typename T> void write_le_vector_vec2(std::vector<uint8_t>&, const std::vector<vec2<T>>&);
template <typename T> void write_be_vector_vec2(std::vector<uint8_t>&, const std::vector<vec2<T>>&);
template <typename T> void write_le_vector_vec3(std::vector<uint8_t>&, const std::vector<vec3<T>>&);
template <typename T> void write_be_vector_vec3(std::vector<uint8_t>&, const std::vector<vec3<T>>&);
template <typename T> void write_le_vector_vec4(std::vector<uint8_t>&, const std::vector<vec4<T>>&);
template <typename T> void write_be_vector_vec4(std::vector<uint8_t>&, const std::vector<vec4<T>>&);
}
#endif
