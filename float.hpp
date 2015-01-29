#ifndef __FLOAT_HPP
#define __FLOAT_HPP

#include "types/bits.hpp"
#include "debug.hpp"


template <unsigned int EXPONENT, unsigned int SIGNIFICAND>
class ieee_float {
    public:
        static const unsigned int EXPONENT_BITS    = EXPONENT;
        static const unsigned int SIGNIFICAND_BITS = SIGNIFICAND;
        static const unsigned int TOTAL_BITS       = 1 + EXPONENT + SIGNIFICAND;

        static const unsigned int BIAS = (1 << (EXPONENT - 1)) - 1;

        typedef typename bits_type<TOTAL_BITS>::sint     sint_t;
        typedef typename bits_type<TOTAL_BITS>::uint     uint_t;
        typedef typename bits_type<TOTAL_BITS>::floating floating_t;

    protected:
        union {
            uint_t     m_bits;
            floating_t m_floating;

            struct {
                uint_t sign        : 1;
                uint_t exponent    : EXPONENT;
                uint_t significand : SIGNIFICAND;
            } m_components;
        };

    public:
        ieee_float (void);
        ieee_float (floating_t  _floating);
        ieee_float (const ieee_float &rhs);

        static unsigned int bias (void)
            { return BIAS; }

        void   set_bits (uint_t _bits) { m_bits = _bits; }
        uint_t bits     (void) const   { return m_bits;  }

        bool negative (void) const { return m_components.sign; }
        void negate   (void)       { m_components.sign ^= m_components.sign; }

        bool is_zero      (void) const;
        bool is_subnormal (void) const;
        bool is_inifinity (void) const;
        bool is_nan       (void) const;
        bool operator==   (floating_t) const;

        static bool almost_equal (floating_t, floating_t);
        static bool almost_equal (floating_t, floating_t, unsigned ulps);
};


typedef ieee_float< 5,  10> ieee_half;
typedef ieee_float< 8,  23> ieee_single;
typedef ieee_float<11,  52> ieee_double;


static_assert (sizeof(ieee_half   ) == 2, "ieee_half must be 2 bytes");
static_assert (sizeof(ieee_single ) == 4, "ieee_single must be 4 bytes");
static_assert (sizeof(ieee_double ) == 8, "ieee_double must be 8 bytes");


#endif // __FLOAT_HPP
