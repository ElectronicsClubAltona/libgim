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
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_ITERATOR_HPP
#define __UTIL_ITERATOR_HPP

#include "types.hpp"


template <typename Base>
class referencing_iterator {
    protected:
        typedef typename std::enable_if<
            is_dereferencable<
                typename Base::value_type
            >::value,
            typename Base::value_type
        >::type base_value_type;

    public:
        typedef typename dereferenced_type<base_value_type>::type value_type        ;
        typedef typename Base::difference_type                    difference_type   ;
        typedef value_type&                                       reference         ;
        typedef value_type*                                       pointer;
        typedef typename Base::iterator_category                  iterator_category; 

    protected:
        Base m_base;

    public:
        explicit referencing_iterator (Base _base):
            m_base (_base)
        { ; }

        referencing_iterator& operator++()    {                   ++m_base; return *this; }
        referencing_iterator  operator++(int) { auto val = *this; ++m_base; return   val; }

        bool operator== (const referencing_iterator<Base> &rhs) { return m_base == rhs.m_base; }
        bool operator!= (const referencing_iterator<Base> &rhs) { return m_base != rhs.m_base; }
        bool operator>= (const referencing_iterator<Base> &rhs) { return m_base >= rhs.m_base; }
        bool operator<= (const referencing_iterator<Base> &rhs) { return m_base <= rhs.m_base; }
        bool operator>  (const referencing_iterator<Base> &rhs) { return m_base >  rhs.m_base; }
        bool operator<  (const referencing_iterator<Base> &rhs) { return m_base <  rhs.m_base; }

        const reference operator*() const
            { return **m_base; }
        reference operator*()
            { return **m_base; }

        difference_type operator-(const referencing_iterator<Base>& rhs) const { return m_base - rhs.m_base; }
        referencing_iterator<Base> operator-(int rhs) const { return referencing_iterator (m_base - rhs); }
        referencing_iterator<Base> operator+(int rhs) const { return referencing_iterator (m_base + rhs); }

};

#endif
