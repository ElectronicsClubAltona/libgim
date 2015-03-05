/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORD_HPP
#define __UTIL_COORD_HPP

#include "../preprocessor.hpp"
#include "../platform.hpp"

#include <algorithm>
#include <iterator>
#include <type_traits>

#include <cstdlib>

namespace util {
    template <size_t,typename> class point;
    template <size_t,typename> class extent;
    template <size_t,typename> class vector;

    namespace detail {
        ///////////////////////////////////////////////////////////////////////
        // tags for accessor names
        struct rgba { };
        struct xyzw { };
        struct stpq { };
        struct whd  { };


        ///////////////////////////////////////////////////////////////////////
        // Disable GCC warnings about validity of anonyous structures in
        // unions. Push comes to shove I'll manually redsign everything to
        // keep this syntax anyway.
#pragma GCC diagnostic push
#if defined(COMPILER_GCC)
    #pragma GCC diagnostic ignored "-pedantic"
#endif
#if defined(COMPILER_CLANG)
    #pragma GCC diagnostic ignored "-Wgnu"
#endif

        template <size_t S, typename T, typename...>
        struct coord_base {
            T data[S];
        };


        template <typename T>
        struct coord_base<3,T,rgba> {
            union {
                T data[3];
                struct { T r,g,b; };
            };
        };

        template <typename T>
        struct coord_base<4,T,rgba> {
            union {
                T data[4];
                struct { T r,g,b,a; };
            };
        };


        template <typename T>
        struct coord_base<2,T,xyzw> {
            union {
                T data[2];
                struct { T x,y; };
            };
        };


        template <typename T>
        struct coord_base<3,T,xyzw> {
            union {
                T data[3];
                struct { T x,y,z; };
            };
        };


        template <typename T>
        struct coord_base<4,T,xyzw> {
            union {
                T data[4];
                struct { T x,y,z,w; };
            };
        };


        template <typename T>
        struct coord_base<2,T,xyzw,stpq> {
            union {
                T data[2];
                struct { T x,y; };
                struct { T s,t; };
            };
        };


        template <typename T>
        struct coord_base<3,T,xyzw,stpq> {
            union {
                T data[3];
                struct { T x,y,z; };
                struct { T s,t,p; };
            };
        };


        template <typename T>
        struct coord_base<4,T,xyzw,stpq> {
            union {
                T data[4];
                struct { T x,y,z,w; };
                struct { T s,t,p,q; };
            };
        };


        template <typename T>
        struct coord_base<2,T,whd> {
            union {
                T data[2];
                struct { T w,h; };
            };
        };


        template <typename T>
        struct coord_base<3,T,whd> {
            union {
                T data[3];
                struct { T w,h,d; };
            };
        };

#pragma GCC diagnostic pop

        ///////////////////////////////////////////////////////////////////////
        // coord types are not really intended to have arbitrary dimension, so
        // don't add specialisations (or a general case) without a decent
        // reason.
        template <size_t S, typename T, typename...>
        struct coord_init;

        ////---------------------------------------------------------------------
        template <typename T, typename ...tags>
        struct coord_init<1,T,tags...> : public coord_base<1,T,tags...>
        {
            using coord_base<1,T,tags...>::coord_base;
            coord_init () = default;
            coord_init (T v0):
                coord_base<1,T,tags...> ({v0})
            { ; }
        };


        ////---------------------------------------------------------------------
        template <typename T, typename ...tags>
        struct coord_init<2,T,tags...> : public coord_base<2,T,tags...>
        {
            using coord_base<2,T,tags...>::coord_base;
            coord_init () = default;
            coord_init (T v0, T v1):
                coord_base<2,T,tags...> ({ v0, v1 })
            { ; }
        };


        ////---------------------------------------------------------------------
        template <typename T, typename ...tags>
        struct coord_init<3,T,tags...> : public coord_base<3,T,tags...>
        {
            using coord_base<3,T,tags...>::coord_base;
            coord_init () = default;
            coord_init (T v0, T v1, T v2):
                coord_base<3,T,tags...> ({v0, v1, v2})
            { ; }
        };


        ////---------------------------------------------------------------------
        template <typename T, typename ...tags>
        struct coord_init<4,T,tags...> : public coord_base<4,T,tags...>
        {
            using coord_base<4,T,tags...>::coord_base;
            coord_init () = default;
            coord_init (T v0, T v1, T v2, T v3):
                coord_base<4,T,tags...> ({ v0, v1, v2, v3 })
            { ; }
        };

        /////////////////////////////////////////////////////////////////////////
        template <size_t S, typename T, typename ...tags>
        struct coord : public coord_init<S,T,tags...> {
            static_assert (S > 0, "coord dimensions must be strictly positive");

            typedef T value_type;
            static constexpr size_t dimension = S;
            static constexpr size_t elements = S;

            size_t size (void) const { return S; }

            // constructors
            using coord_init<S,T,tags...>::coord_init;
            coord () = default;

            explicit coord (T v)
            { std::fill (std::begin (this->data), std::end (this->data), v); }

            coord (const coord<S,T,tags...> &rhs) = default;
            coord& operator= (const coord<S,T,tags...> &rhs) = default;

            // element accessors
            T& operator[] (size_t i)       { return this->data[i]; }
            T  operator[] (size_t i) const { return this->data[i]; }

            const T* begin (void) const { return std::begin (this->data); }
            const T* end   (void) const { return std::end   (this->data); }

            T* begin (void) { return std::begin (this->data); }
            T* end   (void) { return std::end   (this->data); }
        };


        ///////////////////////////////////////////////////////////////////////
        template <
            size_t S,
            typename T,
            template <size_t,typename> class A,
            template <size_t,typename> class B
        >
        struct coord_traits { };


        template <size_t S, typename T> struct coord_traits<S,T,extent,extent> { typedef extent<S,T> result; };
        template <size_t S, typename T> struct coord_traits<S,T,extent,vector> { typedef extent<S,T> result; };
        template <size_t S, typename T> struct coord_traits<S,T,point,extent>  { typedef point<S,T> result; };
        template <size_t S, typename T> struct coord_traits<S,T,point,vector>  { typedef point<S,T> result; };
        template <size_t S, typename T> struct coord_traits<S,T,vector,vector> { typedef vector<S,T> result; };
    }


    ///////////////////////////////////////////////////////////////////////////
    // vector operators
#define ELEMENT_OP(OP)                                          \
    template <                                                  \
        size_t S,                                               \
        typename T,                                             \
        template <size_t,typename> class A,                     \
        template <size_t,typename> class B                      \
    >                                                           \
    typename detail::coord_traits<S,T,A,B>::result              \
    operator OP (A<S,T> a, B<S,T> b)                            \
    {                                                           \
        typename detail::coord_traits<S,T,A,B>::result out;     \
        for (size_t i = 0; i < S; ++i)                          \
            out[i] = a[i] OP b[i];                              \
        return out;                                             \
    }                                                           \
                                                                \
    template <                                                  \
        size_t S,                                               \
        typename T,                                             \
        template <size_t,typename> class A,                     \
        template <size_t,typename> class B                      \
    >                                                           \
    typename detail::coord_traits<S,T,A,B>::result&             \
    operator PASTE(OP,=) (A<S,T>& a, B<S,T> b)                  \
    {                                                           \
        for (size_t i = 0; i < S; ++i)                          \
            a[i] PASTE(OP,=) b[i];                              \
        return a;                                               \
    }

    ELEMENT_OP(+)
    ELEMENT_OP(-)
    ELEMENT_OP(*)
    ELEMENT_OP(/)
#undef ELEMENT_OP

    ///////////////////////////////////////////////////////////////////////////
    // scalar operators
#define SCALAR_OP(OP)                                                       \
    template <size_t S, typename T, template <size_t,typename> class K>     \
    K<S,T>                                                                  \
    operator OP (T t, K<S,T> k)                                             \
    {                                                                       \
        K<S,T> out;                                                         \
        for (size_t i = 0; i < S; ++i)                                      \
            out[i] = t OP k[i];                                             \
        return out;                                                         \
    }                                                                       \
                                                                            \
    template <size_t S, typename T, template <size_t,typename> class K>     \
    K<S,T>                                                                  \
    operator OP (K<S,T> k, T t)                                             \
    {                                                                       \
        K<S,T> out;                                                         \
        for (size_t i = 0; i < S; ++i)                                      \
            out[i] = t OP k[i];                                             \
        return out;                                                         \
    }

    SCALAR_OP(+)
    SCALAR_OP(-)
    SCALAR_OP(*)
    SCALAR_OP(/)

#undef SCALAR_OP


#define SCALAR_OP(OP)                                                       \
    template <size_t S, typename T, template <size_t,typename> class K>     \
    K<S,T>&                                                                 \
    operator OP (K<S,T> &k, T t)                                            \
    {                                                                       \
        for (size_t i = 0; i < S; ++i)                                      \
            k[i] OP t;                                                      \
        return k;                                                           \
    }

    SCALAR_OP(+=)
    SCALAR_OP(-=)
    SCALAR_OP(*=)
    SCALAR_OP(/=)
#undef SCALAR_OP


    ///////////////////////////////////////////////////////////////////////////
    // logic operators
    template <size_t S, typename T, template <size_t,typename> class K>
    bool operator== (K<S,T> a, K<S,T> b)
    { return std::equal (std::begin (a), std::end (a), std::begin (b)); }

    template <size_t S, typename T, template <size_t,typename> class K>
    bool operator!= (K<S,T> a, K<S,T> b)
    { return !(a == b); }


    ///////////////////////////////////////////////////////////////////////////
    // special operators
    template <size_t S, typename T> vector<S,T> operator- (point<S,T> a, point<S,T> b)
    {
        vector<S,T> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = a[i] - b[i];
        return out;
    }

    template <size_t S, typename T, template<size_t,typename> class A, template <size_t,typename> class B>
    T dot (A<S,T> a, B<S,T> b)
    {
        T sum { 0 };
        for (size_t i = 0; i < S; ++i)
            sum += a[i] * b[i];
        return sum;
    }
}

#endif
