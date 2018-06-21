#include "serial_mem.h"


using namespace min;

// min::mem_file methods
const uint8_t &mem_file::operator[](const size_t index) const
{
    return (*_data)[_offset + index];
}

uint8_t &mem_file::operator[](const size_t index)
{
    return (*_data)[_offset + index];
}

size_t mem_file::offset() const
{
    return _offset;
}

size_t mem_file::size() const
{
    return _size;
}

std::string mem_file::to_string() const
{
    // Allocate space for the bytes
    std::string out(_size, 0);

    // Copy data into string
    std::memcpy(&out[0], &(*_data)[_offset], _size);

    // Return the copied string
    return out;
}

