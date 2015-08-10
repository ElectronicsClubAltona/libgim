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

#ifndef __UTIL_THREADS_MUTEX_HPP
#define __UTIL_THREADS_MUTEX_HPP

#include <pthread.h>

namespace util { namespace threads {
    class mutex {
    public:
        mutex ();
        ~mutex ();

        void lock (void);
        bool try_lock (void);
        void unlock (void);

        pthread_mutex_t native (void);

    private:
        pthread_mutex_t m_id;
    };
} }

#endif
