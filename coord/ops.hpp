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
            template <size_t,typename> class A,
            template <size_t,typename> class B
        >
        struct traits { };

        //-------------------------------------------------------------------------
        template <size_t S, typename T> struct traits<S,T,colour,colour> { typedef colour<S,T>  result; };
        template <size_t S, typename T> struct traits<S,T,extent,extent> { typedef extent<S,T>  result; };
        template <size_t S, typename T> struct traits<S,T,extent,vector> { typedef extent<S,T>  result; };
        template <size_t S, typename T> struct traits<S,T,point,extent>  { typedef point<S,T>   result; };
        template <size_t S, typename T> struct traits<S,T,point,vector>  { typedef point<S,T>   result; };
        template <size_t S, typename T> struct traits<S,T,vector,point>  { typedef point<S,T>   result; };
        template <size_t S, typename T> struct traits<S,T,vector,vector> { typedef vector<S,T>  result; };
    }

    ///////////////////////////////////////////////////////////////////////////
    // vector operators
#define ELEMENT_OP(OP)                                  \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        template <size_t,typename> class A,             \
        template <size_t,typename> class B              \
    >                                                   \
    typename coord::traits<S,T,A,B>::result             \
    operator OP (A<S,T> a, B<S,T> b)                    \
    {                                                   \
        typename coord::traits<S,T,A,B>::result out;    \
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
    typename coord::traits<S,T,A,B>::result&            \
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
        template <size_t,typename> class K              \
    >                                                   \
    K<S,T>                                              \
    operator OP (T t, K<S,T> k)                         \
    {                                                   \
        K<S,T> out;                                     \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = t OP k[i];                         \
        return out;                                     \
    }                                                   \
                                                        \
    template <                                          \
        size_t S,                                       \
        typename T,                                     \
        template <size_t,typename> class K              \
    >                                                   \
    K<S,T>                                              \
    operator OP (K<S,T> k, T t)                         \
    {                                                   \
        K<S,T> out;                                     \
        for (size_t i = 0; i < S; ++i)                  \
            out[i] = k[i] OP t;                         \
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
        typename T
    >
    vector<S,T>
    operator- (point<S,T> a, point<S,T> b)
    {
        vector<S,T> out;
        for (size_t i = 0; i < S; ++i)
            out[i] = a[i] - b[i];
        return out;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T
    >
    vector<S,T> operator- (T t, point<S,T> p)
    {
        return point<S,T> {t} - p;
    }


    //-------------------------------------------------------------------------
    template <
        size_t S,
        typename T,
        template<size_t,typename> class A,
        template <size_t,typename> class B
    >
    T dot (A<S,T> a, B<S,T> b)
    {
        T sum { 0 };
        for (size_t i = 0; i < S; ++i)
            sum += a[i] * b[i];
        return sum;
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
}

#endif
