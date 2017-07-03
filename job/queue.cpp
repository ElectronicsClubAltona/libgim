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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#include "./queue.hpp"

#include <iostream>

using util::job::queue;


///////////////////////////////////////////////////////////////////////////////
queue::queue ():
    queue (std::thread::hardware_concurrency () ?: 1)
{ ; }


//-----------------------------------------------------------------------------
queue::queue (unsigned thread_count):
    m_loop ([] (store &s) {
        args obj;

        while (true) {
            // acquire the work lock and see if we need to quit, continue,
            // or sleep
            std::unique_lock<std::mutex> lk (s.mutex);
            if (s.pending.empty ()) {
                s.cv.wait (lk, [&] () {
                    return s.stopping.load () || !s.pending.empty ();
                });
            }

            if (s.stopping.load ())
                break;

            // extract the arguments and forward them to the functor
            obj = std::move (s.pending.front ());
            s.pending.pop_front ();
            lk.unlock ();
            s.cv.notify_one ();

            obj.function (obj);
        }
    }),
    m_threads (thread_count)
{
    for (auto &t: m_threads)
        t = std::thread (m_loop, std::ref (m_store));
}


//-----------------------------------------------------------------------------
queue::~queue ()
{
    // tell everyone we want to quit
    {
        std::lock_guard<std::mutex> lk {m_store.mutex};
        m_store.stopping.store (true);
    }
    m_store.cv.notify_all ();

    // wait for everyone to tidy up. perhaps we'd like to use a timeout, but
    // if things deadlock then it's the users fault currently.
    std::for_each (
        std::begin (m_threads),
        std::end (m_threads),
        [] (auto &t)
    {
        t.join ();
    });
}

