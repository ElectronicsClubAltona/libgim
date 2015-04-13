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
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_ITERATOR_HPP
#define __UTIL_ITERATOR_HPP

#include "types/traits.hpp"


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

        const value_type& operator*() const
            { return **m_base; }
        reference operator*()
            { return **m_base; }

        difference_type operator-(const referencing_iterator<Base>& rhs) const { return m_base - rhs.m_base; }
        referencing_iterator<Base> operator-(int rhs) const { return referencing_iterator (m_base - rhs); }
        referencing_iterator<Base> operator+(int rhs) const { return referencing_iterator (m_base + rhs); }

};

#endif
