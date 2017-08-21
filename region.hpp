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


#ifndef CRUFT_UTIL_REGION_HPP
#define CRUFT_UTIL_REGION_HPP

#include "./extent.hpp"
#include "./point.hpp"
#include "./vector.hpp"
#include "./types/traits.hpp"

#include <ostream>

namespace util {
    /**
     * A two-dimensional rectangle, with size and position.
     */
    template <size_t S, typename T>
    struct region {
        using extent_t = util::extent<S,T>;
        using point_t  = util::point<S,T>;

        using value_type = T;

        //---------------------------------------------------------------------
        static constexpr size_t dimension = S;
        static constexpr size_t elements = extent_t::elements + point_t::elements;

        point_t  p;
        extent_t e;

        //---------------------------------------------------------------------
        region () = default;
        explicit region (extent_t);
        region (point_t, extent_t);
        region (point_t, point_t);

        //---------------------------------------------------------------------
        template <typename U>
        constexpr region<S,U> cast (void) const;

        //---------------------------------------------------------------------
        T area (void) const;
        T diameter (void) const;
        extent_t magnitude (void) const;
        extent_t magnitude (extent_t);

        bool empty (void) const;

        //---------------------------------------------------------------------
        point_t base    (void) const;
        point_t away    (void) const;
        point_t centre  (void) const;
        point_t closest (point_t) const;

        //---------------------------------------------------------------------
        // exclusive of borders
        bool intersects (region<S,T>) const;
        // Compute binary region combinations
        region intersection (region<S,T>) const;

        // Test if a region lies completely within our space
        bool covers (region<S,T>) const noexcept;
        // Test if a point lies within out space. Inclusive of borders
        bool covers (point<S,T>) const noexcept;

        // Move a point to be within the region bounds
        point_t constrain (point_t) const noexcept;

        //---------------------------------------------------------------------
        // Compute a region `mag` units into the region
        region inset (T mag) const;
        region inset (vector<S,T> mag) const;

        region expand (T mag) const;
        region expand (vector<S,T>) const;

        // arithmetic operators
        region operator+ (vector<S,T>) const;
        region operator- (vector<S,T>) const;

        // Logical comparison operators
        bool operator ==(region<S,T> rhs) const;
        bool operator !=(region<S,T> rhs) const
            { return !(*this == rhs); }

        // Utility constants
        static constexpr region<S,T> max  (void);
        static constexpr region<S,T> unit (void);

        static constexpr region<S,T> zero (void)
        { return { point_t {0}, extent_t {0} }; }

        void sanity (void) const;
    };


    ///////////////////////////////////////////////////////////////////////////
    /// constructs the minimal region that encompasses a region and a point.
    template <typename T, size_t S>
    region<S,T>
    make_union (region<S,T> r, point<S,T> p)
    {
        const auto p0 = select (r.p < p,       r.p,       p);
        const auto p1 = select (r.away () > p, r.away (), p);
        return { p0, p1 };
    }


    ///////////////////////////////////////////////////////////////////////////
    /// construct a point iterator across a given region, generating each
    /// valid point in row-major sequence.
    ///
    /// this is only defined for integral types as it's not clear how to
    /// handle floats; it's _super_ unlikely anyone actually wants to visit
    /// every single floating point value for a region (and if so they can
    /// damn well code that monstrosity themselves).
    template <
        typename T,
        std::size_t S,
        typename = std::enable_if_t<
            std::is_integral_v<T>, void
        >
    >
    auto
    make_range (region<S,T> r)
    {
        using region_t = region<S,T>;
        using point_t = typename region_t::point_t;
        using vector_t = util::vector<S,T>;

        // this range object is mostly a wrapper around the existing
        // extent_range object with a constant offset. it's not going to be as
        // performant, but when we discover this is an issue we can do write a
        // better version of this object & iterator.
        class region_range {
        public:
            class iterator : public std::iterator<std::forward_iterator_tag, point_t, size_t> {
            public:
                iterator (typename extent_range<S,T>::iterator _inner, vector_t _offset):
                    m_inner (_inner),
                    m_offset (_offset)
                { ; }


                point_t operator* (void) const { return *m_inner + m_offset; }

                iterator&
                operator++ (void)
                {
                    ++m_inner;
                    return *this;
                }


                bool operator== (const iterator &rhs) const
                {
                    assert (m_offset == rhs.m_offset);
                    return m_inner == rhs.m_inner;
                }


                bool operator!= (const iterator &rhs) const
                { return !(*this == rhs); }


            private:
                typename extent_range<S,T>::iterator m_inner;
                vector_t m_offset;
            };

            region_range (region_t _r):
                m_range { _r.e + T{1} },
                m_offset { _r.p.template as<util::vector> () }
            { ; }

            iterator begin (void) const { return { m_range.begin (), m_offset }; }
            iterator end   (void) const { return { m_range.end   (), m_offset }; }

            iterator cbegin (void) const { return begin (); }
            iterator cend   (void) const { return end   (); }

        private:
            const extent_range<S,T> m_range;
            const vector_t m_offset;
        };


        return region_range { r };
    };


    template <typename T> using region2 = region<2,T>;
    template <typename T> using region3 = region<3,T>;

    using region2u = region2<unsigned>;
    using region2i = region2<int>;
    using region2f = region2<float>;
    using region2d = region2<double>;

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, const util::region<S,T>&);
}

#include "region.ipp"

#endif
