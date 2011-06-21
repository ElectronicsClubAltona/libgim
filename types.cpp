#include "types.hpp"

#include <type_traits>

using namespace std;


#define do_type_to_string(T)                                                    \
template <> std::string type_to_string <T>       (void) { return #T; }          \
template <> std::string type_to_string <const T> (void) { return "const " #T; }

do_type_to_string (float)
do_type_to_string (double)

do_type_to_string (  int8_t)
do_type_to_string ( int16_t)
do_type_to_string ( int32_t)
do_type_to_string ( int64_t)

do_type_to_string ( uint8_t)
do_type_to_string (uint16_t)
do_type_to_string (uint32_t)
do_type_to_string (uint64_t)

#ifdef WORDS_BIGENDIAN
/* Big endian doesn't need swapping */
template <typename T>
T hton (T val) {
    static_assert (is_fundamental <T>(), "hton implementation assumes fundamental types");
    static_assert (is_unsigned    <T>(), "hton implementation does not handle signed");
    return val;
}

template <typename T>
T ntoh (T val) {
    static_assert (is_fundamental <T>(), "ntoh implementation assumes fundamental types");
    static_assert (is_unsigned    <T>(), "ntoh implementation does not handle signed");
    return val;
}

template <>  uint8_t hton ( uint8_t);
template <> uint16_t hton (uint16_t);
template <> uint32_t hton (uint32_t);
template <> uint64_t hton (uint64_t);

template <>  uint8_t ntoh ( uint8_t);
template <> uint16_t ntoh (uint16_t);
template <> uint32_t ntoh (uint32_t);
template <> uint64_t ntoh (uint64_t);

#else

static void
byte_swap (uint8_t *restrict dst, uint8_t *restrict src, size_t len) {
    for (unsigned int i = 0; i < len; ++i)
        dst[len - i - 1] = src[i];
}


template <typename T>
T
hton (T i) {
    // Unsure if this will really be sensible for non-intrinsic types, or types larger than 8 bytes.
    static_assert (is_fundamental <T>::value, "hton implementation assumes fundamental types");
    static_assert (is_unsigned    <T>::value, "hton implementation does not handle signed");

    T swapped;
    byte_swap (reinterpret_cast <uint8_t*>(&swapped),
               reinterpret_cast <uint8_t*>(&i),
               sizeof (T));
    return swapped;
}


template <typename T>
T
ntoh (T i) {
    // Unsure if this will really be sensible for non-intrinsic types, or types larger than 8 bytes.
    static_assert (is_fundamental <T>::value, "ntoh implementation assumes fundamental types");
    static_assert (is_unsigned    <T>::value, "ntoh implementation does not handle signed");

    T swapped;
    byte_swap (reinterpret_cast <uint8_t*>(&swapped),
               reinterpret_cast <uint8_t*>(&i),
               sizeof (T));
    return swapped;
}

template <> uint8_t hton (uint8_t i) { return i; }
template <> uint8_t ntoh (uint8_t i) { return i; }

template uint16_t ntoh<uint16_t> (uint16_t);
template uint32_t ntoh<uint32_t> (uint32_t);
template uint64_t ntoh<uint64_t> (uint64_t);

template uint16_t hton<uint16_t> (uint16_t);
template uint32_t hton<uint32_t> (uint32_t);
template uint64_t hton<uint64_t> (uint64_t);

#endif

