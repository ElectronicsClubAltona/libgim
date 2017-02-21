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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_PASCAL_HPP
#define __UTIL_PASCAL_HPP

#include <cstdlib>
#include <ostream>

namespace util {
    template <typename DataT, typename SizeT = std::size_t>
    class parray {
    public:
        parray (SizeT size, DataT *data);

        DataT& operator[] (SizeT idx);
        const DataT& operator[] (SizeT idx) const;

        DataT& at (SizeT idx);
        const DataT& at (SizeT idx) const;

        DataT* begin (void);
        DataT* end   (void);

        const DataT* cbegin (void) const;
        const DataT* cend   (void) const;

        const DataT* data (void) const;
        DataT* data (void);
        SizeT size (void) const;

    private:
        const SizeT m_size;
        DataT *m_data;
    };

    template <typename SizeT>
    std::ostream&
    operator<< (std::ostream&, util::parray<const char, SizeT>);

    template <typename SizeT>
    std::ostream&
    operator<< (std::ostream&, util::parray<char, SizeT>);

    template <typename DataT, typename SizeT>
    std::ostream&
    operator<< (std::ostream&, util::parray<DataT, SizeT>);
}


#endif
