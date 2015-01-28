/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */


#include "image.hpp"

#include "debug.hpp"
#include "except.hpp"

#include <fstream>


//-----------------------------------------------------------------------------
static void
write_netpbm (const uint8_t *restrict pixels,
              size_t width,
              size_t height,
              size_t stride,
              const boost::filesystem::path &path,
              const char* MAGIC) {
    CHECK (pixels);
    CHECK_GT (width, 0);
    CHECK_GE (stride, width);
    CHECK_GT (height, 0);

    // Establish an output stream
    std::ofstream output (path.string ());
    if (!output.good ())
        throw util::output_error ("Unable to open output file");

    // Write the PPM header.
    output << MAGIC  << "\n"
           << width  << "\n"
           << height << "\n"
           << (size_t)std::numeric_limits<uint8_t>::max () << "\n";

    // Write the data rows
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x)
            output << pixels[y * stride + x];
    }
}


//-----------------------------------------------------------------------------
void
util::pgm::write (const uint8_t *restrict pixels,
                  size_t width,
                  size_t height,
                  size_t stride,
                  const boost::filesystem::path &path) {
    // TODO: We should switch between P2 (ascii) and P5 (binary)
    static const char MAGIC[] = "P5";
    write_netpbm (pixels, width, height, stride, path, MAGIC);
}


//-----------------------------------------------------------------------------
void
util::ppm::write (const uint8_t *restrict pixels,
                  size_t width,
                  size_t height,
                  size_t stride,
                  const boost::filesystem::path &path) {
    // TODO: We should switch between P3 (ascii) and P6 (binary)
    static const char MAGIC[] = "P6";
    write_netpbm (pixels, width, height, stride, path, MAGIC);
}
