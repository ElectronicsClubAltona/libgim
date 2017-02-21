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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "pascal.hpp"

#include "debug.hpp"

#include <iterator>
#include <stdexcept>

using util::parray;


///////////////////////////////////////////////////////////////////////////////
template <typename DataT, typename SizeT>
parray<DataT,SizeT>::parray (SizeT _size, DataT *_data):
    m_size (_size),
    m_data (_data)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <typename DataT, typename SizeT>
DataT&
parray<DataT, SizeT>::operator[] (SizeT idx)
{
    CHECK_LIMIT(idx, 0u, m_size);
    return data ()[idx];
}


//-----------------------------------------------------------------------------
template <typename DataT, typename SizeT>
const DataT&
parray<DataT, SizeT>::operator[] (SizeT idx) const
{
    CHECK_LIMIT(idx, 0u, m_size);
    return data ()[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <typename DataT, typename SizeT>
DataT&
parray<DataT, SizeT>::at (SizeT idx)
{
    if (idx >= size ())
        throw std::out_of_range ("invalid index for parray");

    return (*this)[idx];
}


//-----------------------------------------------------------------------------
template <typename DataT, typename SizeT>
const DataT&
parray<DataT, SizeT>::at (SizeT idx) const
{
    if (idx >= size ())
        throw std::out_of_range ("invalid index for parray");

    return (*this)[idx];
}


///////////////////////////////////////////////////////////////////////////////
template <typename DataT, typename SizeT>
DataT*
parray<DataT, SizeT>::begin (void)
{
    return data ();
}


//-----------------------------------------------------------------------------
template <typename DataT, typename SizeT>
DataT*
parray<DataT, SizeT>::end (void)
{
    return data () + size ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename DataT, typename SizeT>
const DataT*
parray<DataT, SizeT>::cbegin (void) const
{
    return data ();
}


//-----------------------------------------------------------------------------
template <typename DataT, typename SizeT>
const DataT*
parray<DataT, SizeT>::cend (void) const
{
    return data () + size ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename DataT, typename SizeT>
const DataT*
parray<DataT, SizeT>::data (void) const
{
    return m_data;
}


//-----------------------------------------------------------------------------
template <typename DataT, typename SizeT>
DataT*
parray<DataT, SizeT>::data (void)
{
    return m_data;
}


//-----------------------------------------------------------------------------
template <typename DataT, typename SizeT>
SizeT
parray<DataT, SizeT>::size (void) const
{
    return m_size;
}


///////////////////////////////////////////////////////////////////////////////
template <typename SizeT>
std::ostream&
util::operator<< (std::ostream &os, parray<const char,SizeT> p)
{
    std::copy_n (
        p.cbegin (),
        p.size (),
        std::ostream_iterator<const char> (os)
    );

    return os;
}


//-----------------------------------------------------------------------------
template <typename SizeT>
std::ostream&
util::operator<< (std::ostream &os, parray<char,SizeT> p)
{
    std::copy_n (
        p.cbegin (),
        p.size (),
        std::ostream_iterator<char> (os)
    );

    return os;
}


///////////////////////////////////////////////////////////////////////////////
template <typename DataT, typename SizeT>
std::ostream&
util::operator<< (std::ostream &os, parray<DataT,SizeT> p)
{
    auto size = p.size ();
    uintptr_t ptr = reinterpret_cast<uintptr_t> (p.data ());
    os << "[" << +size << ", " << std::hex << ptr << std::dec << "]";
    return os;
}




///////////////////////////////////////////////////////////////////////////////
#define INSTANTIATE_D_S(D,S)                                                \
template std::ostream& util::operator<< (std::ostream&, parray<      D,S>); \
template std::ostream& util::operator<< (std::ostream&, parray<const D,S>); \
                                                                            \
template class util::parray<      D,S>;                                     \
template class util::parray<const D,S>;

//-----------------------------------------------------------------------------
#define INSTANTIATE_D(D)        \
INSTANTIATE_D_S(D, uint16_t)    \
INSTANTIATE_D_S(D, uint32_t)    \
INSTANTIATE_D_S(D, uint64_t)


//-----------------------------------------------------------------------------
INSTANTIATE_D (char)
INSTANTIATE_D (uint16_t)
INSTANTIATE_D (uint32_t)
INSTANTIATE_D (uint64_t)
