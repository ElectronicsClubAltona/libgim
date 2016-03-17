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

namespace util { namespace alloc {
    // wraps an allocator given at construction time, forwarding all calls to
    // the inner object. used to allow virtual dispatch of the non-virtual
    // allocator interface.
    class dynamic {
    public:
        static constexpr auto DEFAULT_ALIGNMENT = alignof (std::max_align_t);

        // disable copying, but allow moving (required for calls to 'make')
        dynamic (const dynamic&) = delete;
        dynamic& operator= (const dynamic&) = delete;

        dynamic (dynamic &&rhs) = default;
        dynamic& operator= (dynamic&&) = default;

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

        // allocation management
        void*
        allocate (size_t bytes,
                  size_t alignment = DEFAULT_ALIGNMENT) &
        {
            return m_child->allocate (bytes, alignment);
        }

        void
        deallocate (void *ptr,
                    size_t bytes,
                    size_t alignment = DEFAULT_ALIGNMENT)
        {
            return m_child->deallocate (ptr, bytes, alignment);
        }

        void reset (void) { m_child->reset (); }

        // capacity queries
        size_t capacity (void) const { return m_child->capacity (); }
        size_t used     (void) const { return m_child->used ();     }
        size_t remain   (void) const { return m_child->remain ();   }

    private:
        // Internal base for arbitrary allocator types. Necessary for
        // type ellision in super-client classes.
        class base {
        public:
            base () = default;
            base (const base&) = delete;
            base (base&&) = delete;
            base& operator= (const base&) = delete;
            base& operator= (base&&) = delete;

            virtual ~base () { ; }

            // allocation management
            virtual void*
            allocate (size_t bytes,
                      size_t alignment = DEFAULT_ALIGNMENT) = 0;

            virtual void
            deallocate (void *ptr,
                        size_t bytes,
                        size_t alignment = DEFAULT_ALIGNMENT) = 0;

            virtual void reset (void) = 0;

            // capacity queries
            virtual size_t capacity (void) const = 0;
            virtual size_t used     (void) const = 0;
            virtual size_t remain   (void) const = 0;
        };


        template <typename T>
        class child final : public base {
        public:
            template <typename ...Args>
            child (Args &&...args):
                base (),
                m_target (std::forward<Args> (args)...)
            { ; }

            // allocation management
            void*
            allocate (size_t bytes,
                      size_t alignment = DEFAULT_ALIGNMENT) override
            {
                return m_target.allocate (bytes, alignment);
            }

            void
            deallocate (void *ptr,
                        size_t bytes,
                        size_t alignment = DEFAULT_ALIGNMENT) override
            {
                m_target.deallocate (ptr, bytes, alignment);
            }

            void reset (void) override
            { m_target.reset (); }

            // capacity queries
            size_t capacity (void) const override { return m_target.capacity (); }
            size_t used     (void) const override { return m_target.used (); }
            size_t remain   (void) const override { return m_target.remain (); }

        private:
            T m_target;
        };


        dynamic (std::unique_ptr<base> _child):
            m_child (std::move (_child))
        { ; }

        std::unique_ptr<base> m_child;
    };
} }

#endif
