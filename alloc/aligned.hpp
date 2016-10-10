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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __CRUFT_UTIL_ALLOC_ALIGNED_HPP
#define __CRUFT_UTIL_ALLOC_ALIGNED_HPP

namespace util::alloc {
    /// wraps a child allocator and enforces a fixed alignment
    template <typename Successor>
    class aligned {
    public:
        ///////////////////////////////////////////////////////////////////////
        template <typename ...Args>
        aligned (std::size_t _alignment, Args &&...args):
            m_successor (std::forward<Args> (args)...),
            m_alignment (_alignment)
        { ; }


        ///////////////////////////////////////////////////////////////////////
        auto
        allocate (size_t bytes)
        {
            return m_successor.allocate (bytes, m_alignment);
        }

        //---------------------------------------------------------------------
        void
        deallocate (void *ptr, size_t bytes)
        {
            return m_successor.deallocate (ptr, bytes);
        }


        ///////////////////////////////////////////////////////////////////////
        auto base (void)
        {
            return m_successor.base ();
        }


        //---------------------------------------------------------------------
        auto
        offset (const void *ptr)
        {
            return m_successor.offset (ptr);
        }


        ///////////////////////////////////////////////////////////////////////
        auto reset (void) { return m_successor.reset (); }


        ///////////////////////////////////////////////////////////////////////
        auto capacity (void) const { return m_successor.capacity (); }
        auto used     (void) const { return m_successor.used     (); }
        auto remain   (void) const { return m_successor.remain   (); }


    private:
        Successor   m_successor;
        std::size_t m_alignment;
    };
}

#endif
