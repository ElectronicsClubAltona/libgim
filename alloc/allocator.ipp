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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#ifdef __UTIL_ALLOC_ALLOCATOR_IPP
#error
#endif

#define __UTIL_ALLOC_ALLOCATOR_IPP

#include <utility>

///////////////////////////////////////////////////////////////////////////////
template <class B, class T>
template <typename ...Args>
util::alloc::allocator<B,T>::allocator (Args&& ...args):
    m_backing (std::forward (args)...)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <class B, class T>
T*
util::alloc::allocator<B,T>::allocate (std::size_t count)
{
    return m_backing.template allocate<T> (count);
}


//-----------------------------------------------------------------------------
template <class B, class T>
void
util::alloc::allocator<B,T>::deallocate (T *t, std::size_t count)
{
    return m_backing.template deallocate (t, count);
}
