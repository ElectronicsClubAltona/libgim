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

#ifndef CRUFT_UTIL_ALLOC_RAW_ALIGNED_HPP
#define CRUFT_UTIL_ALLOC_RAW_ALIGNED_HPP

namespace util::alloc::raw {
    /// wraps a child allocator and enforces a fixed alignment
    template <typename ChildT>
    class aligned {
    public:
        ///////////////////////////////////////////////////////////////////////
        template <typename ...Args>
        aligned (size_t _alignment, Args &&...args):
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
        auto
        allocate (size_t bytes, size_t alignment)
        {
            (void)alignment;
            CHECK_EQ (alignment, m_alignment);
            return m_successor.allocate (bytes, m_alignment);
        }


        //---------------------------------------------------------------------
        auto
        deallocate (void *ptr, size_t bytes)
        {
            return m_successor.deallocate (ptr, bytes);
        }


        //---------------------------------------------------------------------
        auto
        deallocate (void *ptr, size_t bytes, size_t alignment)
        {
            (void)alignment;
            CHECK_EQ (alignment, m_alignment);
            return m_successor.deallocate (ptr, bytes, m_alignment);
        }


        ///////////////////////////////////////////////////////////////////////
        auto begin (void)       { return m_successor.begin (); }
        auto begin (void) const { return m_successor.begin (); }


        //---------------------------------------------------------------------
        auto
        offset (const void *ptr) const
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
        ChildT m_successor;
        size_t m_alignment;
    };
}

#endif
