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

#ifndef __UTIL_ALLOC_DYNAMIC_HPP
#define __UTIL_ALLOC_DYNAMIC_HPP

#include <cstddef>
#include <memory>

namespace util::alloc {
    // wraps an allocator given at construction time, forwarding all calls to
    // the inner object. used to allow virtual dispatch of the non-virtual
    // allocator interface.
    class dynamic {
    public:
        //---------------------------------------------------------------------
        // disable copying, but allow moving (required for calls to 'make')
        dynamic (const dynamic&) = delete;
        dynamic& operator= (const dynamic&) = delete;

        dynamic (dynamic &&rhs) = default;
        dynamic& operator= (dynamic&&) = default;

        //---------------------------------------------------------------------
        // construct an inner wrapper for type T. used to get around lack of
        // ambiguous template constructors.
        template <typename T, typename ...Args>
        static dynamic
        make (Args &&...args)
        {
            return dynamic (
                std::make_unique<child<T>> (
                    std::forward<Args> (args)...
                )
            );
        }

        //---------------------------------------------------------------------
        // allocation management
        auto allocate (size_t bytes)                   { return m_child->allocate (bytes); }
        auto allocate (size_t bytes, size_t alignment) { return m_child->allocate (bytes, alignment); }

        auto deallocate (void *ptr, size_t bytes)                   { return m_child->deallocate (ptr, bytes); }
        auto deallocate (void *ptr, size_t bytes, size_t alignment) { return m_child->deallocate (ptr, bytes, alignment); }

        //---------------------------------------------------------------------
        auto base (void)       { return m_child->base (); }
        auto base (void) const { return m_child->base (); }

        auto offset (const void *ptr) const
        { return m_child->offset (ptr); }

        //---------------------------------------------------------------------
        auto reset (void) { return m_child->reset (); }

        //---------------------------------------------------------------------
        // capacity queries
        auto capacity (void) const { return m_child->capacity (); }
        auto used     (void) const { return m_child->used ();     }
        auto remain   (void) const { return m_child->remain ();   }


    private:
        // Internal base for arbitrary allocator types. Necessary for
        // type ellision in super-client classes.
        class interface {
        public:
            interface () = default;
            interface (const interface&) = delete;
            interface (interface&&) = delete;
            interface& operator= (const interface&) = delete;
            interface& operator= (interface&&) = delete;

            virtual ~interface () { ; }

            // allocation management
            virtual void* allocate (size_t bytes) = 0;
            virtual void* allocate (size_t bytes, size_t alignment) = 0;

            virtual void deallocate (void *ptr, size_t bytes) = 0;
            virtual void deallocate (void *ptr, size_t bytes, size_t alignment) = 0;

            virtual void*       base (void) = 0;
            virtual const void* base (void) const = 0;
            virtual size_t      offset (const void*) const = 0;

            virtual void reset (void) = 0;

            // capacity queries
            virtual size_t capacity (void) const = 0;
            virtual size_t used     (void) const = 0;
            virtual size_t remain   (void) const = 0;
        };


        template <typename T>
        class child final : public interface {
        public:
            template <typename ...Args>
            child (Args &&...args):
                interface (),
                m_target (std::forward<Args> (args)...)
            { ; }

            // allocation management
            void*
            allocate (size_t bytes) override
            { return m_target.allocate (bytes); }

            void*
            allocate (size_t bytes, size_t alignment) override
            { return m_target.allocate (bytes, alignment); }

            void
            deallocate (void *ptr, size_t bytes) override
            { m_target.deallocate (ptr, bytes); }

            void
            deallocate (void *ptr, size_t bytes, size_t alignment) override
            { m_target.deallocate (ptr, bytes, alignment); }

            const void*
            base (void) const override
            { return m_target.base (); }

            void*
            base (void) override
            { return m_target.base (); }

            size_t
            offset (const void *ptr) const override
            { return m_target.offset (ptr); }

            void reset (void) override
            { return m_target.reset (); }

            // capacity queries
            size_t capacity (void) const override { return m_target.capacity (); }
            size_t used     (void) const override { return m_target.used (); }
            size_t remain   (void) const override { return m_target.remain (); }

        private:
            T m_target;
        };


        dynamic (std::unique_ptr<interface> _child):
            m_child (std::move (_child))
        { ; }

        std::unique_ptr<interface> m_child;
    };
}

#endif
