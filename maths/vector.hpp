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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_MATHS_VECTOR_HPP
#define __UTIL_MATHS_VECTOR_HPP

#include <vector>
#include <initializer_list>

namespace maths {
    class vector {
        protected:
            std::vector<double> m_data;

        public:
            vector (const std::initializer_list<double> &_data);
            explicit
            vector (unsigned int                         _size);
            vector (const double *restrict               _data,
                    unsigned int                         _size);

            vector (const vector  &rhs);
            vector (const vector &&rhs);

            ~vector (void);

            const double* data (void) const;
            double&       operator[] (unsigned int);
            const double& operator[] (unsigned int) const;

            unsigned int size (void) const;


            static double dot (const double *restrict A,
                               const double *restrict B,
                               unsigned int size);

            static vector cross (const double *restrict A,
                                 const double *restrict B,
                                 unsigned int size);
    };
}


#endif
