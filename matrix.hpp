 /****************************************************************************
                              __         .__  .__  .__                  .___
   ___________ ___.__. _______/  |______  |  | |  | |__| ______ ____   __| _/
 _/ ___\_  __ <   |  |/  ___/\   __\__  \ |  | |  | |  |/  ___// __ \ / __ |
 \  \___|  | \/\___  |\___ \  |  |  / __ \|  |_|  |_|  |\___ \\  ___// /_/ |
  \___  >__|   / ____/____  > |__| (____  /____/____/__/____  >\___  >____ |
      \/       \/         \/            \/                  \/     \/     \/

  Copyright:
    Danny Robson, 2011
 *****************************************************************************/

#ifndef __UTIL_MATRIX_HPP
#define __UTIL_MATRIX_HPP

#include "point.hpp"

#include <iostream>

namespace util {
    struct matrix {
        double values[4][4];

        void scale     (double x, double y, double z);
        void translate (double x, double y, double z);

        matrix  inverse (void) const;
        matrix& invert  (void);

        matrix operator* (const matrix&) const;

        point to_local  (const point &p) const;
        point to_global (const point &p) const;

        bool is_affine (void) const;

        static const matrix IDENTITY;
        static const matrix ZEROES;
    };
}

std::ostream& operator<< (std::ostream&, const util::matrix&);

#endif
