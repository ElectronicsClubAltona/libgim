/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORDS_OPS
#define __UTIL_COORDS_OPS

#include "../preprocessor.hpp"
#include "../maths.hpp"
#include "../types/bits.hpp"

#include <cstdlib>

namespace util {
    // forward declerations for traits
    template <size_t,typename> struct point;
    template <size_t,typename> struct extent;
    template <size_t,typename> struct vector;
    template <size_t,typename> struct colour;

    ///////////////////////////////////////////////////////////////////////
    // operation traits
    namespace coord {
        template <
            size_t S,
            typename T,
            typename U,
            template <size_t,typename> class A,
            template <size_t,typename> class B
        >
        struct traits { };

        //-------------------------------------------------------------------------
        template <size_t S, typename T, typename U> struct traits<S,T,U,colour,colour> { typedef colour<S,typename std::common_type<T,U>::type> result; };
        template <size_t S, typename T, typename U> struct traits<S,T,U,extent,extent> { typedef extent<S,typename std::common_type<T,U>::type> result; };
        template <size_t S, typename T, typename U> struct traits<S,T,U,extent,vector> { typedef extent<S,typename std::common_type<T,U>::type> result; };
        template <size_t S, typename T, typename U> struct traits<S,T,U,point,extent>  { typedef point <S,typename std::common_type<T,U>::type> result; };
        template <size_t S, typename T, typename U> struct traits<S,T,U,point,vector>  { typedef point <S,typename std::common_type<T,U>::type> result; };
        template <size_t S, typename T, typename U> struct traits<S,T,U,vector,point>  { typedef point <S,typename std::common_type<T,U>::type> result; };
        template <size_t S, typename T, typename U> struct traits<S,T,U,vector,vector> { typedef vector<S,typename std::common_type<T,U>::type> result; };
    }

    ///////////////////////////////////////////////////////////////////////////
    // vector operators
#define ELEMENT_OP(OP)                                  \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class A,             \
        template <size_t,typename> class B              \
    >                                                   \
    typename coord::traits<S,T,U,A,B>::result           \
    operator OP (A<S,T> a, B<S,U> b)                    \
    {                                                   \
        typename coord::traits<S,T,U,A,B>::result out;  \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = a[i] OP b[i];                      \
        return out;                                     \
    }                                                   \
                                                        \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        template <size_t,typename> class A,             \
        template <size_t,typename> class B              \
    >                                                   \
    typename coord::traits<S,T,T,A,B>::result&          \
    operator PASTE(OP,=) (A<S,T>& a, B<S,T> b)          \
    {                                                   \
        for (size_t i = 0; i < S; ++i)                  \
            a[i] PASTE(OP,=) b[i];                      \
        return a;                                       \
    }

    ELEMENT_OP(+)
    ELEMENT_OP(-)
    ELEMENT_OP(*)
    ELEMENT_OP(/)
#undef ELEMENT_OP

    ///////////////////////////////////////////////////////////////////////////
    // scalar operators
#define SCALAR_OP(OP)                                   \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class K              \
    >                                                   \
    typename std::enable_if<std::is_fundamental<U>::value, K<S,T>>::type                                              \
    operator OP (U u, K<S,T> k)                         \
    {                                                   \
        K<S,T> out;                                     \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = u OP k[i];                         \
        return out;                                     \
    }                                                   \
                                                        \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        typename U,                                     \
        template <size_t,typename> class K              \
    >                                                   \
    typename std::enable_if<std::is_fundamental<U>::value, K<S,T>>::type                                              \
    operator OP (K<S,T> k, U u)                         \
    {                                                   \
        K<S,T> out;                                     \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = k[i] OP u;                         \
        return out;                                     \
    }

    SCALAR_OP(+)
    SCALAR_OP(-)
    SCALAR_OP(*)
    SCALAR_OP(/)

#undef SCALAR_OP


    //-------------------------------------------------------------------------
#define SCALAR_OP(OP)                           \
    template <                                  \
        size_t S,                               \
        typename T,                             \
        template <size_t,typename> class K      \
    >                                           \
    K<S,T>&                                     \
    operator OP (K<S,T> &k, T t)                \
    {                                           \
        for (size_t i = 0; i < S; ++i)          \
            k[i] OP t;                          \
        return k;                               \
    }

    SCALAR_OP(+=)
    SCALAR_OP(-=)
    SCALAR_OP(*=)
    SCALAR_OP(/=)
#undef SCALAR_OP

    //-------------------------------------------------------------------------
    // negation
    template <size_t S, typename T, template<size_t,typename> class K>
    K<S,T> operator- (K<S,T> k)
    {
        return k * T{-1};
    }


    ///////////////////////////////////////////////////////////////////////////
    // logic operators

    /// elementwise equality operator
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    bool
    operator== (K<S,T> a, K<S,T> b)
    {
        return std::equal (std::begin (a),
                           std::end   (a),
                           std::begin (b));
    }

    ///------------------------------------------------------------------------
    /// elementwise inquality operator
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    bool
    operator!= (K<S,T> a, K<S,T> b)
    {
        return !(a == b);
    }


    ///////////////////////////////////////////////////////////////////////////
    // special operators

    /// point-point subtraction giving a vector difference
    template <
        size_t S,
        typename T,
        typename U
    >
    vector<S,typename std::common_type<T,U>::type>
    operator- (point<S,T> a, point<S,U> b)
    {
        vector<S,typename std::common_type<T,U>::type> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = a[i] - b[i];
        return out;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        typename U
    >
    vector<S,typename std::common_type<T,U>::type>
    operator- (U u, point<S,T> p)
    {
        return point<S,U> {u} - p;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class A,
        template <size_t,typename> class B
    >
    T dot (A<S,T> a, B<S,T> b)
    {
        T sum { 0 };
        for (size_t i = 0; i < S; ++i)
            sum += a[i] * b[i];
        return sum;
    }

    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    abs (K<S,T> k)
    {
        for (auto &v: k)
            v = std::abs (v);
        return k;
    }


    ///////////////////////////////////////////////////////////////////////////
    // logical element operators

    /// return a coord type containing the max element at each offset
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    min (K<S,T> a, K<S,T> b)
    {
        K<S,T> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = min (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    // /return a coord type containing the max element at each offset
    template <
        size_t S,
        typename T,
        template <size_t,typename> class K
    >
    K<S,T>
    max (K<S,T> a, K<S,T> b)
    {
        K<S,T> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = max (a[i], b[i]);
        return out;
    }


    ///------------------------------------------------------------------------
    /// return the minimum element of the coordinate type
    template <size_t S, typename T, template<size_t,typename> class K>
    T
    min (K<S,T> k)
    { return *std::min_element (k.begin (), k.end ()); }


    ///------------------------------------------------------------------------
    /// return the maximum element of the coordinate type
    template <size_t S, typename T, template<size_t,typename> class K>
    T
    max (K<S,T> k)
    { return *std::max_element (k.begin (), k.end ()); }


    template <size_t S, typename T, template<size_t,typename> class K>
    bool
    operator>= (K<S,T> k, T t)
    { return min (k) >= t; }


    template <size_t S, typename T, template<size_t,typename> class K>
    bool
    operator<= (K<S,T> k, T t)
    { return max (k) <= t; }
}

#endif
