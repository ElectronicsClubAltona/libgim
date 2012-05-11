
#include <string>
#include <vector>

#include "../version.hpp"
#include "../debug.hpp"

using namespace std;
using namespace util;


struct parsed_version {
    string                str;
    vector <unsigned int> parts;
};


int
main (int, char **) {
    vector <parsed_version> tests ({
        { "1",          { 1          } },

        { "1.2",        { 1, 2       } },
        { "1.2.3",      { 1, 2, 3    } },
        { "1.2.3.4",    { 1, 2, 3, 4 } },

        { "9.5a",       { 9, 5       } },
        { "8.2.5b",     { 8, 2, 5    } },

        /*
        { "1.4.1-p8",   { 1, 4, 1    } },
        { "4.2.0-r4",   { 4, 2, 0    } },

        { "1.4 RC1",    { 1, 4       } }
        */
    });

    for (auto i = tests.begin (); i != tests.end (); ++i) {
        version v (i->str);

        if (i->parts.size () > 0) CHECK (v.major () == i->parts[0]);
        if (i->parts.size () > 1) CHECK (v.minor () == i->parts[1]);
        if (i->parts.size () > 2) CHECK (v.point () == i->parts[2]);
        if (i->parts.size () > 3) CHECK (v.build () == i->parts[3]);

        CHECK_HARD (i->parts.size () <= 4);
    }

    return EXIT_SUCCESS;
}
