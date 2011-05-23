/*
 * This file is part of waif.
 *
 * Waif is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * Waif is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with waif.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2011 Danny Robson <danny@blubinc.net>
 */

#ifndef __UTIL_VECTOR_HPP
#define __UTIL_VECTOR_HPP

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
