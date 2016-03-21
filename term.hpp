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

#ifndef __UTIL_TERM_HPP
#define __UTIL_TERM_HPP

#include <ostream>

namespace util { namespace term {
    bool has_csi_support (void);

    namespace csi {
        struct code {
            static constexpr char CSI = '\x1B';
        };


        struct graphics : public code {
            static constexpr char terminator = 'm';

            enum layer {
                FOREGROUND = 30,
                BACKGROUND = 40
            };

            enum hue {
                BLACK   = 0,
                RED     = 1,
                GREEN   = 2,
                YELLOW  = 3,
                BLUE    = 4,
                MAGENTA = 5,
                CYAN    = 6,
                WHITE   = 7,
            };

            graphics (layer, hue);
            explicit graphics (char value);

            char value (void) const;
            char value (char);

            static const graphics RESET;

        private:
            char m_value;
        };


        std::ostream& operator<< (std::ostream&, code);
        std::ostream& operator<< (std::ostream&, graphics);
    }
} }

#endif
