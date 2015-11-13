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

#ifdef __UTIL_ALLOC_ARENA_IPP
#error
#endif

#define __UTIL_ALLOC_ARENA_IPP


///////////////////////////////////////////////////////////////////////////////
template <class T>
util::alloc::arena<T>::arena (T &store):
    m_store (store)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <class T>
template <typename U, typename ...Args>
U*
util::alloc::arena<T>::acquire (Args&& ...args)
{
    U *data = m_store.allocate (sizeof (U), alignof (U));
    
    try {
        new (data) U (std::forward (args)...);
    } catch (...) {
        m_store.deallocate (data, sizeof (U));
        throw;
    }
}


//-----------------------------------------------------------------------------
template <class T>
template <typename U, typename ...Args>
std::unique_ptr<U>
util::alloc::arena<T>::unique (Args&& ...args)
{
    return std::unique_ptr<U> (acquire (std::forward (args)...));
}


//-----------------------------------------------------------------------------
template <class T>
template <typename U>
void
util::alloc::arena<T>::release (U *u)
{
    u->~U ();
    m_store.deallocate (u);
}


