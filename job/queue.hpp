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

#ifndef CRUFT_UTIL_JOB_QUEUE_HPP
#define CRUFT_UTIL_JOB_QUEUE_HPP

#include <array>
#include <deque>
#include <thread>
#include <vector>
#include <new>
#include <cstddef>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace util::job {
    class queue {
    public:
        queue ();
        explicit queue (unsigned thread_count);
        ~queue ();

        using cookie = int;

        template <class Function, typename ...Args>
        cookie
        submit (cookie &parent, Function&&, Args&&...);

        /// record a functor and a set of parameters to execute at some point
        /// in the future by an arbitrary available thread.
        template <class Function, typename ...Args>
        cookie
        submit (Function &&func, Args &&...params)
        {
            {
                std::unique_lock<std::mutex> lk (m_store.mutex);
                m_store.pending.emplace_back (
                    std::forward<Function> (func),
                    std::forward<Args> (params)...
                );
            }

            m_store.cv.notify_one ();

            // TODO: return a useful identifier to allow waiting
            return 0;
        }

        void wait (cookie);

        // HACK: this doesn't actually implement a proper barrier and may not
        // even guarantee that all jobs are flushed. it's provided to make
        // some amount of testing slightly more convenient by draining most
        // of the queue on demand.
        void
        flush (void)
        {
            // setup a cv and completion flag. this is a poor man's barrier.
            std::mutex m;
            std::unique_lock<std::mutex> lk (m);
            std::condition_variable cv;
            std::atomic<bool> done = false;

            // submit a job to the back of the queue that sets the done flag
            // and wakes us back up again.
            submit ([&] (void) {
                {
                    std::lock_guard<std::mutex> _{m};
                    done.store (true);
                }

                cv.notify_one ();
            });

            // wait until the flag is set then exit.
            do {
                cv.wait (lk, [&] () { return done.load (); });
            } while (!done.load ());
        }


    private:
        /// stores a functor and associated arguments in a fixed size buffer
        /// for later execution.
        ///
        /// for ease of implementation the arguments are currently restricted
        /// as follows:
        ///   * trivial types (memcpy'able)
        ///   * a fixed total maximum size of around one cache line
        /// these limitations will be eliminated at some point in the future
        ///
        /// the user supplied functor is wrapped with our own that unpacks and
        /// forwards the arguments from the data buffer. this function must
        /// be passed a copy of the current arg object as the only argument.
        struct args {
            args () = default;

            template <class Function, typename ...Args>
            args (Function &&func, Args&&...params)
            {
                using tuple_t = std::tuple<std::decay_t<Args>...>;
                static_assert ((std::is_trivial_v<std::decay_t<decltype(params)>> && ...));
                static_assert (sizeof (tuple_t) <= sizeof data);

                union {
                    decltype(data) *byte_ptr;
                    tuple_t        *args_ptr;
                };
                 byte_ptr = &data;
                *args_ptr = std::make_tuple (params...);

                function = [func] (args &base) {
                    std::apply (func, *reinterpret_cast<tuple_t*> (&base.data));
                };
            };

            // GCC: switch to hardware_destructive_interference_size when it
            // becomes available in libstdc++. Until then we use a sensible
            // guess.
            std::array<char,64> data;

            std::function<void(args&)> function;
        };

        struct store {
            std::atomic<bool> stopping = false;
            std::deque<args> pending;

            std::condition_variable cv;
            std::mutex mutex;
        };

        store m_store;
        std::function<void(store&)> m_loop;
        std::vector<std::thread> m_threads;
    };
}

#endif
