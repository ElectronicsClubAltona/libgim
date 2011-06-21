#ifndef __TYPES_HPP
#define __TYPES_HPP

#include <cstdint>
#include <string>
#include <limits>

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


template <typename T>
struct sign_types;


template <> struct sign_types <signed int> {
    typedef   signed int with_sign;
    typedef unsigned int without_sign;
};


template <> struct sign_types <unsigned int> {
    typedef   signed int with_sign;
    typedef unsigned int without_sign;
};


/// Safely cast a numeric type to its signed comparison, aborting if the
/// result is not representable.
template <typename T>
typename sign_types<T>::with_sign
sign_cast (const typename sign_types<T>::without_sign v)
{
    check_hard (v <= std::numeric_limits<typename sign_types<T>::without_sign>::max () >> 1);
    return static_cast <typename sign_types<T>::with_sign> (v);
}


/// Safely cast a numeric type to its unsigned comparison, aborting if the
/// result is not representable.
template <typename T>
typename sign_types<T>::without_sign
sign_cast (const typename sign_types<T>::with_sign v)
{
    check_hard (v >= 0);
    return static_cast <typename sign_types<T>::without_sign> (v);
}


/// Returns the number of elements of a statically allocated array
template <typename T, size_t N>  
size_t elems(T (&)[N])
    { return N; }


template <typename T>
T hton (T);


template <typename T>
T ntoh (T);

#endif // __TYPES_HPP
