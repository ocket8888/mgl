#include "serial.h"

template unsigned int min::read_le(const std::vector<uint8_t>&, size_t&);
template int min::read_le(const std::vector<uint8_t>&, size_t&);
template unsigned short min::read_le(const std::vector<uint8_t>&, size_t&);
template char min::read_le(const std::vector<uint8_t>&, size_t&);

template <typename T>
T min::read_le(const std::vector<uint8_t> &stream, size_t &next)
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
T min::read_be(const std::vector<uint8_t> &stream, size_t &next)
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

template void min::write_le(std::vector<uint8_t>&, const char);
template void min::write_le(std::vector<uint8_t>&, const unsigned int);
template void min::write_le(std::vector<uint8_t>&, const int);

template <typename T>
void min::write_le(std::vector<uint8_t> &stream, const T data)
{
    // Pointer to data
    uint8_t *const ptr = (uint8_t *)&data;

    // Size of T in bytes
    const size_t size = sizeof(T);
    for (size_t i = 0; i < size; i++)
    {
        // Pack little endian stream
        stream.push_back(ptr[i]);
    }
}

template void min::write_be(std::vector<uint8_t>&, const int);
template void min::write_be(std::vector<uint8_t>&, const unsigned int);
template void min::write_be(std::vector<uint8_t>&, const char);
template void min::write_be(std::vector<uint8_t>&, const unsigned short);
template <typename T>
void min::write_be(std::vector<uint8_t> &stream, const T data)
{
    // Pointer to data
    uint8_t *const ptr = (uint8_t *)&data;

    // Size of T in bytes
    const size_t size = sizeof(T);
    const size_t offset = size - 1;
    for (size_t i = 0; i < size; i++)
    {
        // Pack big endian stream
        stream.push_back(ptr[offset - i]);
    }
}

template void min::write_le(std::vector<uint8_t>&, const unsigned int, const size_t);

template <typename T>
void min::write_le(std::vector<uint8_t> &stream, const T data, const size_t offset)
{
    // Pointer to data
    uint8_t *const ptr = (uint8_t *)&data;

    // Size of T in bytes
    const size_t size = sizeof(T);
    for (size_t i = 0; i < size; i++)
    {
        // Pack little endian stream
        stream[offset + i] = ptr[i];
    }
}

template <typename T>
void min::write_be(std::vector<uint8_t> &stream, const T data, const size_t offset)
{
    // Pointer to data
    uint8_t *const ptr = (uint8_t *)&data;

    // Size of T in bytes
    const size_t size = sizeof(T);
    const size_t be_offset = size - 1;
    for (size_t i = 0; i < size; i++)
    {
        // Pack big endian stream
        stream[offset + i] = ptr[be_offset - i];
    }
}

template std::vector<unsigned short> min::read_le_vector(const std::vector<uint8_t>&, size_t&);
template std::vector<unsigned int> min::read_le_vector(const std::vector<uint8_t>&, size_t&);
template <typename T>
std::vector<T> min::read_le_vector(const std::vector<uint8_t> &stream, size_t &next)
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
std::vector<T> min::read_be_vector(const std::vector<uint8_t> &stream, size_t &next)
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

template void min::write_le_vector(std::vector<uint8_t>&, const std::vector<unsigned short>&);
template void min::write_le_vector(std::vector<uint8_t>&, const std::vector<unsigned int>&);
template <typename T>
void min::write_le_vector(std::vector<uint8_t> &stream, const std::vector<T> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_le<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_le<T>(stream, data[i]);
    }
}

template <typename T>
void min::write_be_vector(std::vector<uint8_t> &stream, const std::vector<T> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_be<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_be<T>(stream, data[i]);
    }
}

template <typename T>
min::vec2<T> min::read_le_vec2(const std::vector<uint8_t> &stream, size_t &next)
{
    const T x = read_le<T>(stream, next);
    const T y = read_le<T>(stream, next);
    return min::vec2<T>(x, y);
}

template <typename T>
min::vec2<T> min::read_be_vec2(const std::vector<uint8_t> &stream, size_t &next)
{
    const T x = read_be<T>(stream, next);
    const T y = read_be<T>(stream, next);
    return min::vec2<T>(x, y);
}

template <typename T>
min::vec3<T> min::read_le_vec3(const std::vector<uint8_t> &stream, size_t &next)
{
    const T x = read_le<T>(stream, next);
    const T y = read_le<T>(stream, next);
    const T z = read_le<T>(stream, next);
    return min::vec3<T>(x, y, z);
}

template <typename T>
min::vec3<T> min::read_be_vec3(const std::vector<uint8_t> &stream, size_t &next)
{
    const T x = read_be<T>(stream, next);
    const T y = read_be<T>(stream, next);
    const T z = read_be<T>(stream, next);
    return min::vec3<T>(x, y, z);
}

template <typename T>
min::vec4<T> min::read_le_vec4(const std::vector<uint8_t> &stream, size_t &next)
{
    const T x = read_le<T>(stream, next);
    const T y = read_le<T>(stream, next);
    const T z = read_le<T>(stream, next);
    const T w = read_le<T>(stream, next);
    return min::vec4<T>(x, y, z, w);
}

template <typename T>
min::vec4<T> min::read_be_vec4(const std::vector<uint8_t> &stream, size_t &next)
{
    const T x = read_be<T>(stream, next);
    const T y = read_be<T>(stream, next);
    const T z = read_be<T>(stream, next);
    const T w = read_be<T>(stream, next);
    return min::vec4<T>(x, y, z, w);
}

template <typename T>
void min::write_le_vec2(std::vector<uint8_t> &stream, const min::vec2<T> &v)
{
    write_le<T>(stream, v.x);
    write_le<T>(stream, v.y);
}

template <typename T>
void min::write_be_vec2(std::vector<uint8_t> &stream, const min::vec2<T> &v)
{
    write_be<T>(stream, v.x);
    write_be<T>(stream, v.y);
}

template <typename T>
void min::write_le_vec3(std::vector<uint8_t> &stream, const min::vec3<T> &v)
{
    write_le<T>(stream, v.x);
    write_le<T>(stream, v.y);
    write_le<T>(stream, v.z);
}

template <typename T>
void min::write_be_vec3(std::vector<uint8_t> &stream, const min::vec3<T> &v)
{
    write_be<T>(stream, v.x);
    write_be<T>(stream, v.y);
    write_be<T>(stream, v.z);
}

template <typename T>
void min::write_le_vec4(std::vector<uint8_t> &stream, const min::vec4<T> &v)
{
    write_le<T>(stream, v.x());
    write_le<T>(stream, v.y());
    write_le<T>(stream, v.z());
    write_le<T>(stream, v.w());
}

template <typename T>
void min::write_be_vec4(std::vector<uint8_t> &stream, const min::vec4<T> &v)
{
    write_be<T>(stream, v.x());
    write_be<T>(stream, v.y());
    write_be<T>(stream, v.z());
    write_be<T>(stream, v.w());
}

template std::vector<min::vec2<float>> min::read_le_vector_vec2(const std::vector<uint8_t>&, size_t&);
template std::vector<min::vec2<double>> min::read_le_vector_vec2(const std::vector<uint8_t>&, size_t&);
template <typename T>
std::vector<min::vec2<T>> min::read_le_vector_vec2(const std::vector<uint8_t> &stream, size_t &next)
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
std::vector<min::vec2<T>> min::read_be_vector_vec2(const std::vector<uint8_t> &stream, size_t &next)
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

template std::vector<min::vec3<float>> min::read_le_vector_vec3(const std::vector<uint8_t>&, size_t&);
template std::vector<min::vec3<double>> min::read_le_vector_vec3(const std::vector<uint8_t>&, size_t&);
template <typename T>
std::vector<min::vec3<T>> min::read_le_vector_vec3(const std::vector<uint8_t> &stream, size_t &next)
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
std::vector<min::vec3<T>> min::read_be_vector_vec3(const std::vector<uint8_t> &stream, size_t &next)
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

template std::vector<min::vec4<float>> min::read_le_vector_vec4(const std::vector<uint8_t>&, size_t&);
template std::vector<min::vec4<double>> min::read_le_vector_vec4(const std::vector<uint8_t>&, size_t&);
template <typename T>
std::vector<min::vec4<T>> min::read_le_vector_vec4(const std::vector<uint8_t> &stream, size_t &next)
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
std::vector<min::vec4<T>> min::read_be_vector_vec4(const std::vector<uint8_t> &stream, size_t &next)
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

template void min::write_le_vector_vec2(std::vector<uint8_t>&, const std::vector<min::vec2<double>>&);
template void min::write_le_vector_vec2(std::vector<uint8_t>&, const std::vector<min::vec2<float>>&);
template <typename T>
void min::write_le_vector_vec2(std::vector<uint8_t> &stream, const std::vector<vec2<T>> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_le<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_le_vec2<T>(stream, data[i]);
    }
}

template <typename T>
void min::write_be_vector_vec2(std::vector<uint8_t> &stream, const std::vector<vec2<T>> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_be<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_be_vec2<T>(stream, data[i]);
    }
}

template void min::write_le_vector_vec3(std::vector<uint8_t>&, const std::vector<vec3<float>>&);
template void min::write_le_vector_vec3(std::vector<uint8_t>&, const std::vector<vec3<double>>&);
template <typename T>
void min::write_le_vector_vec3(std::vector<uint8_t> &stream, const std::vector<vec3<T>> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_le<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_le_vec3<T>(stream, data[i]);
    }
}

template <typename T>
void min::write_be_vector_vec3(std::vector<uint8_t> &stream, const std::vector<vec3<T>> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_be<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_be_vec3<T>(stream, data[i]);
    }
}

template void min::write_le_vector_vec4(std::vector<uint8_t>&, const std::vector<min::vec4<float>>&);
template void min::write_le_vector_vec4(std::vector<uint8_t>&, const std::vector<min::vec4<double>>&);
template <typename T>
void min::write_le_vector_vec4(std::vector<uint8_t> &stream, const std::vector<vec4<T>> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_le<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_le_vec4<T>(stream, data[i]);
    }
}

template <typename T>
void min::write_be_vector_vec4(std::vector<uint8_t> &stream, const std::vector<vec4<T>> &data)
{
    // Get data size, must be less than 2^32-1
    const uint32_t size = data.size();

    // Write vector size to stream, zero vector is allowed
    write_be<uint32_t>(stream, size);

    // Write each data element
    for (uint32_t i = 0; i < size; i++)
    {
        write_be_vec4<T>(stream, data[i]);
    }
}
