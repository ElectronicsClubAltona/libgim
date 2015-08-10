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

#ifndef __UTIL_THREADS_THREAD_POSIX_HPP
#define __UTIL_THREADS_THREAD_POSIX_HPP

#include <functional>
#include <pthread.h>

namespace util { namespace threads {
    class thread {
    public:
        template <typename Func, typename ...Args>
        thread (Func &&_func, Args &&..._args);
        thread (thread&&);
        ~thread ();

        thread (const thread&) = delete;
        thread& operator= (const thread&) = delete;


        void join (void);
        void kill (void);

        static void yield (void);
        static unsigned concurrency (void);

        pthread_t native (void);

    private:
        static void* dispatch (void*);

        std::function<void(void)> m_func;
        pthread_t m_id;
    };
} }

#include "thread_posix.ipp"

#endif

