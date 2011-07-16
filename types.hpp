#ifndef __TYPES_HPP
#define __TYPES_HPP

#include "enable_if.hpp"

#include <cstdint>
#include <string>
#include <limits>
#include <memory>

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


namespace detail {
    template <typename T, typename V>
    T
    _sign_cast (const typename enable_if<sizeof(T) == sizeof(V)     &&
                                         std::is_unsigned<T>::value &&
                                         std::is_signed<V>::value, V>::type v)
    {
        check_hard (v >= 0);
        return static_cast<T> (v);
    }


    template <typename T, typename V>
    T
    _sign_cast (const typename enable_if<sizeof(T) == sizeof(V)   &&
                                         std::is_signed<T>::value &&
                                         std::is_unsigned<V>::value, V>::type v)
    {
        check_hard (v < std::numeric_limits<V>::max () / 2);
        return static_cast<T> (v);
    }
}

/// Safely cast a numeric type to its (un)signed counterpart, aborting if the
/// dynamically checked result is not representable. May be optimised out if
/// NDEBUG is defined.
template <typename T, typename V>
T
sign_cast (const V v)
    { return detail::_sign_cast<T,V>(v); }


namespace detail {
    // Same sign, no possibility of truncation with larger target type
    template <typename T, typename V>
    T
    _trunc_cast (const typename enable_if<sizeof (T) >= sizeof (V) &&
                                          std::is_signed<T>::value == std::is_signed<V>::value, V>::type v)
        { return v; }


    template <typename T, typename V>
    T
    _trunc_cast (const typename enable_if<sizeof (T) < sizeof (V) &&
                                          std::is_signed<T>::value == std::is_signed<V>::value, V>::type v) {
        check_hard (v <= std::numeric_limits<T>::max ());
        checK_hard (v >= std::numeric_limits<T>::min ());

        return static_cast<T> (v);
    }
}


template <typename T, typename V>
T
trunc_cast (V v)
    { return detail::_trunc_cast<T, V> (v); }


/// Returns the number of elements of a statically allocated array
template <typename T, size_t N>  
size_t elems(T (&)[N])
    { return N; }


/// Convert a scalar from host byte order to network byte order
template <typename T>
T hton (T);


/// Convert a scalar from network byte order to host byte order
template <typename T>
T ntoh (T);


template <typename T, typename ...Args>
std::unique_ptr<T>
make_unique (const Args &...args)
    { return std::unique_ptr<T> (new T (args...)); }


#endif // __TYPES_HPP
