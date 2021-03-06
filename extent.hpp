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
 * Copyright 2010-2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_EXTENT_HPP
#define __UTIL_EXTENT_HPP

#include "coord/fwd.hpp"
#include "coord/base.hpp"
#include "vector.hpp"
#include "point.hpp"

#include <cstddef>

namespace util {
    /**
     * A pure n-dimensional size, without positioning
     */
    template <size_t S, typename T>
    struct extent : public ::util::coord::base<S,T,::util::extent<S,T>>
    {
        using ::util::coord::base<S,T,::util::extent<S,T>>::base;

        extent () = default;
        explicit extent (::util::vector<S,T>);

        constexpr T    area  (void) const;
        constexpr T diameter (void) const;

        template <typename U = float>
        constexpr
        U aspect (void) const;

        /// tests whether a point would lie within:
        ///     region { origin, *this }, inclusive of borders.
        ///
        /// included for parity with util::region.
        constexpr bool
        inclusive (util::point<S,T> p) const
        {
            return all (p >= T{0} && p <= *this);
        }


        /// tests whether a point would like within:
        ///     region { origin, *this }, exclusive of the bottom-right border
        /// included for parity with util::region
        constexpr bool
        exclusive (point<S,T> p) const
        {
            return all (p >= T{0} && p < *this);
        }

        ::util::extent<S,T> expanded (::util::vector<S,T>) const;
        ::util::extent<S,T> expanded (T) const;
        ::util::extent<S,T> contracted (::util::vector<S,T>) const;
        ::util::extent<S,T> contracted (T) const;

        bool empty (void) const;

        static constexpr ::util::extent<S,T> max (void);
        static constexpr ::util::extent<S,T> min (void);
    };

    template <size_t S, typename T>
    struct extent_range {
    public:
        struct iterator : public std::iterator<
            std::forward_iterator_tag,
            ::util::point<S,T>,
            size_t
        > {
        public:
            iterator (::util::extent<S,T>, ::util::point<S,T>);

            point<S,T> operator* () const;
            iterator& operator++ (void);

            bool operator!= (const iterator &rhs) const;
            bool operator== (const iterator &rhs) const;

        private:
            point<S,T>  m_cursor;
            extent<S,T> m_target;
        };

        explicit extent_range (extent<S,T> target);

        iterator begin (void) const;
        iterator end   (void) const;

    private:
        extent<S,T> m_target;
    };


    ///////////////////////////////////////////////////////////////////////////
    template <size_t S, typename T>
    extent_range<S,T>
    make_range (extent<S,T> e)
    {
        return extent_range<S,T> {e};
    }


    ///////////////////////////////////////////////////////////////////////////
    // convenience typedefs
    template <typename T> using extent2 = extent<2,T>;
    template <typename T> using extent3 = extent<3,T>;

    template <size_t S> using extentu = extent<S,unsigned>;
    template <size_t S> using extenti = extent<S,int>;
    template <size_t S> using extentf = extent<S,float>;
    template <size_t S> using extentd = extent<S,double>;


    typedef extent2<int> extent2i;
    typedef extent2<unsigned> extent2u;
    typedef extent2<float> extent2f;
    typedef extent2<double> extent2d;


    typedef extent3<unsigned> extent3u;
    typedef extent3<float> extent3f;


    //-------------------------------------------------------------------------
    template <typename T> using extent_range2 = extent_range<2,T>;
    template <typename T> using extent_range3 = extent_range<3,T>;


    using extent_range2u = extent_range2<typename extent2u::value_type>;
    using extent_range2i = extent_range2<typename extent2i::value_type>;
    using extent_range3u = extent_range2<typename extent3u::value_type>;
}


#include "extent.ipp"

#endif
