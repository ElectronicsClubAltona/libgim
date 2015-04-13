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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_SIGNAL_HPP
#error
#endif

#include "debug.hpp"

#include <algorithm>

namespace util {
    ///////////////////////////////////////////////////////////////////////////
    template <typename F, template <typename> class C>
    struct signal<F,C>::cookie : public nocopy {
        cookie (typename group::iterator, signal<F,C> &parent);
        cookie (cookie &&rhs);
        ~cookie ();

        void reset (callback &&cb);

        typename group::iterator m_position;
        signal<F,C> &m_parent;
    };


    ///////////////////////////////////////////////////////////////////////////
    template <typename F, template <typename> class C>
    signal<F,C>::cookie::cookie (typename group::iterator _position,
                                 signal<F,C> &_parent):
        m_position (_position),
        m_parent (_parent)
    { ; }


    //-------------------------------------------------------------------------
    template <typename F, template <typename> class C>
    signal<F,C>::cookie::cookie (cookie &&rhs):
        m_position (rhs.m_position),
        m_parent (rhs.m_parent)
    {
        rhs.m_position = rhs.m_parent.m_children.end ();
    }


    //-------------------------------------------------------------------------
    template <typename F, template <typename> class C>
    signal<F,C>::cookie::~cookie ()
    {
        if (m_parent.m_children.end () != m_position)
            m_parent.disconnect (*this);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename F, template <typename> class C>
    void
    signal<F,C>::cookie::reset (callback &&cb)
    {
        *m_position = std::move (cb);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename F, template <typename> class C>
    signal<F,C>::signal ()
    { ; }


    //-------------------------------------------------------------------------
    template <typename F, template <typename> class C>
    signal<F,C>::~signal ()
    {
        CHECK (empty ());
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename F, template <typename> class C>
    typename signal<F,C>::cookie
    signal<F,C>::connect (const callback &_cb)
    {
        return cookie (
            m_children.insert (
                m_children.end (),
                std::move (_cb)
            ),
            *this
        );
    }


    //-------------------------------------------------------------------------
    template <typename F, template <typename> class C>
    typename signal<F,C>::cookie
    signal<F,C>::connect (callback &&_cb)
    {
        return cookie (
            m_children.insert (
                m_children.end (),
                std::move (_cb)
            ),
            *this
        );
    }


    //-------------------------------------------------------------------------
    template <typename F, template <typename> class C>
    void
    signal<F,C>::disconnect (cookie &c)
    {
        m_children.erase (c.m_position);
        c.m_position = m_children.end ();
    }


    //-------------------------------------------------------------------------
    /// Disconnect all callbacks
    template <typename F, template <typename> class C>
    void
    signal<F,C>::clear (void)
    {
        m_children.clear ();
    }


    ///////////////////////////////////////////////////////////////////////////
    /// Returns the number of callbacks connected.
    template <typename F, template <typename> class C>
    unsigned int
    signal<F,C>::size (void) const
    {
        return m_children.size ();
    }


    //-------------------------------------------------------------------------
    template <typename F, template <typename> class C>
    bool
    signal<F,C>::empty (void) const
    {
        return m_children.empty ();
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename F, template <typename> class C>
    template <typename ...Args>
    typename signal<F,C>::R
    signal<F,C>::operator () (Args&&... tail) {
        if (m_children.empty ())
            return R();

        //auto i = m_children.cbegin ();
        //bool looping;

        C<F> combiner;
        return combiner (m_children.begin (), m_children.end (), std::forward<Args> (tail)...);

        //do {
        //    // Increment before we execute so that the caller is able to
        //    // deregister themselves during execution.
        //    auto current = i++;
        //    looping = m_children.cend () != i;

        //    (*current)(std::forward<Args> (tail)...);
        //} while (looping);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    value_signal<T>::value_signal (T t):
        m_value (t)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T>
    value_signal<T>::operator const T&() const
    {
        return m_value;
    }


    //-------------------------------------------------------------------------
    template <typename T>
    value_signal<T>&
    value_signal<T>::operator= (const T &t)
    {
        m_value = t;
        (*this) (m_value);
        return *this;
    }
}

