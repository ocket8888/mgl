#include "serial_mem.h"


// min::mem_file methods
const uint8_t &min::mem_file::operator[](const size_t index) const
{
    return (*_data)[_offset + index];
}

uint8_t &min::mem_file::operator[](const size_t index)
{
    return (*_data)[_offset + index];
}

size_t min::mem_file::offset() const
{
    return _offset;
}

size_t min::mem_file::size() const
{
    return _size;
}

std::string min::mem_file::to_string() const
{
    // Allocate space for the bytes
    std::string out(_size, 0);

    // Copy data into string
    std::memcpy(&out[0], &(*_data)[_offset], _size);

    // Return the copied string
    return out;
}

template unsigned int min::read_le(const min::mem_file&, size_t&);
template unsigned short min::read_le(const min::mem_file&, size_t&);

template <typename T>
T min::read_le(const min::mem_file &stream, size_t &next)
{
    // Check type is compatible
    static_assert(sizeof(long long) >= sizeof(T), "Invalid type size, sizeof(T) <= sizeof(long long)");

    // Size of T in bytes
    const size_t size = sizeof(T);
    long long temp = 0;
    uint8_t shift = 0;
    for (size_t i = 0; i < size; i++)
    {
        // Unpack little endian stream
        temp |= ((long long)stream[next + i] << shift);
        shift += 8;
    }

    // Change next position
    next += size;

    // Avoiding strict aliasing rules of C/C++
    const T *out = reinterpret_cast<T *>(&temp);
    return *out;
}

template <typename T>
T min::read_be(const mem_file &stream, size_t &next)
{
    // Check type is compatible
    static_assert(sizeof(long long) >= sizeof(T), "Invalid type size, sizeof(T) <= sizeof(long long)");

    // Size of T in bytes
    const size_t size = sizeof(T);
    long long temp = 0;
    uint8_t shift = (sizeof(T) - 1) * 8;
    for (size_t i = 0; i < size; i++)
    {
        // Unpack big endian stream
        temp |= ((long long)stream[next + i] << shift);
        shift -= 8;
    }

    // Change next position
    next += size;

    // Avoiding strict aliasing rules of C/C++
    const T *out = reinterpret_cast<T *>(&temp);
    return *out;
}

template <typename T>
std::vector<T> min::read_le_vector(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_le<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<T> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(T) * size) > ssize)
    {
        throw std::runtime_error("read_le_vector: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const T data = read_le<T>(stream, next);
        out.push_back(data);
    }

    return out;
}

template <typename T>
std::vector<T> min::read_be_vector(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_be<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<T> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(T) * size) > ssize)
    {
        throw std::runtime_error("read_be_vector: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const T data = read_be<T>(stream, next);
        out.push_back(data);
    }

    return out;
}

template <typename T>
min::vec2<T> min::read_le_vec2(const mem_file &stream, size_t &next)
{
    const T x = read_le<T>(stream, next);
    const T y = read_le<T>(stream, next);
    return min::vec2<T>(x, y);
}

template <typename T>
min::vec2<T> min::read_be_vec2(const mem_file &stream, size_t &next)
{
    const T x = read_be<T>(stream, next);
    const T y = read_be<T>(stream, next);
    return min::vec2<T>(x, y);
}

template <typename T>
min::vec3<T> min::read_le_vec3(const mem_file &stream, size_t &next)
{
    const T x = read_le<T>(stream, next);
    const T y = read_le<T>(stream, next);
    const T z = read_le<T>(stream, next);
    return min::vec3<T>(x, y, z);
}

template <typename T>
min::vec3<T> min::read_be_vec3(const mem_file &stream, size_t &next)
{
    const T x = read_be<T>(stream, next);
    const T y = read_be<T>(stream, next);
    const T z = read_be<T>(stream, next);
    return min::vec3<T>(x, y, z);
}

template <typename T>
min::vec4<T> min::read_le_vec4(const mem_file &stream, size_t &next)
{
    const T x = read_le<T>(stream, next);
    const T y = read_le<T>(stream, next);
    const T z = read_le<T>(stream, next);
    const T w = read_le<T>(stream, next);
    return min::vec4<T>(x, y, z, w);
}

template <typename T>
min::vec4<T> min::read_be_vec4(const mem_file &stream, size_t &next)
{
    const T x = read_be<T>(stream, next);
    const T y = read_be<T>(stream, next);
    const T z = read_be<T>(stream, next);
    const T w = read_be<T>(stream, next);
    return min::vec4<T>(x, y, z, w);
}

template <typename T>
std::vector<min::vec2<T>> min::read_le_vector_vec2(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_le<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<min::vec2<T>> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(min::vec2<T>) * size) > ssize)
    {
        throw std::runtime_error("read_le_vector_vec2: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const min::vec2<T> data = read_le_vec2<T>(stream, next);
        out.push_back(data);
    }

    return out;
}

template <typename T>
std::vector<min::vec2<T>> min::read_be_vector_vec2(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_be<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<min::vec2<T>> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(min::vec2<T>) * size) > ssize)
    {
        throw std::runtime_error("read_be_vector_vec2: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const min::vec2<T> data = read_be_vec2<T>(stream, next);
        out.push_back(data);
    }

    return out;
}

template <typename T>
std::vector<min::vec3<T>> min::read_le_vector_vec3(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_le<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<min::vec3<T>> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(min::vec3<T>) * size) > ssize)
    {
        throw std::runtime_error("read_le_vector_vec3: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const min::vec3<T> data = read_le_vec3<T>(stream, next);
        out.push_back(data);
    }

    return out;
}

template <typename T>
std::vector<min::vec3<T>> min::read_be_vector_vec3(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_be<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<min::vec3<T>> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(min::vec3<T>) * size) > ssize)
    {
        throw std::runtime_error("read_be_vector_vec3: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const min::vec3<T> data = read_be_vec3<T>(stream, next);
        out.push_back(data);
    }

    return out;
}

template <typename T>
std::vector<min::vec4<T>> min::read_le_vector_vec4(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_le<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<min::vec4<T>> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(min::vec4<T>) * size) > ssize)
    {
        throw std::runtime_error("read_le_vector_vec4: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const min::vec4<T> data = read_le_vec4<T>(stream, next);
        out.push_back(data);
    }

    return out;
}

template <typename T>
std::vector<min::vec4<T>> min::read_be_vector_vec4(const mem_file &stream, size_t &next)
{
    const uint32_t size = read_be<uint32_t>(stream, next);

    // Create output vector and reserve memory
    std::vector<min::vec4<T>> out;
    out.reserve(size);

    // Check that the stream has enough data
    const size_t ssize = stream.size();
    if ((next + sizeof(min::vec4<T>) * size) > ssize)
    {
        throw std::runtime_error("read_be_vector_vec4: ran out of data in stream");
    }

    // Read all vector elements
    for (uint32_t i = 0; i < size; i++)
    {
        const min::vec4<T> data = read_be_vec4<T>(stream, next);
        out.push_back(data);
    }

    return out;
}
