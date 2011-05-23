#ifndef __TYPES_HPP
#define __TYPES_HPP

#include <cstdint>
#include <string>

template <int BITS>
struct bits_type;


template <> struct bits_type< 8> {
    static const bool has_floating = false;

    typedef    uint8_t uint;
    typedef     int8_t sint;
    typedef    uint8_t floating;
};


template <> struct bits_type<16> {
    static const bool has_floating = false;

    typedef   uint16_t uint;
    typedef    int16_t sint;
    typedef   uint16_t floating;
};


template <> struct bits_type<32> {
    static const bool has_floating = true;

    typedef uint32_t uint;
    typedef  int32_t sint;
    typedef    float floating;
};


template <> struct bits_type<64> {
    static const bool has_floating = true;

    typedef uint64_t uint;
    typedef  int64_t sint;
    typedef   double floating;
};


template <typename T>
struct sized_type : public bits_type<sizeof(T) * 8>
{ };


template <typename T>
std::string type_to_string (void);


#endif // __TYPES_HPP
