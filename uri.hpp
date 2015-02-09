
#ifndef __UTIL_URI_HPP
#define __UTIL_URI_HPP

#include <string>
#include <cstdlib>
#include <stdexcept>


namespace util {
    class uri {
    public:
        uri (std::string &&);
        uri (const char *str);
        uri (const char *first, const char *last);

        class parse_error : public std::runtime_error
        { using runtime_error::runtime_error; };

        enum component : unsigned {
            SCHEME,
            AUTHORITY,
            PATH,
            QUERY,
            FRAGMENT,

            NUM_COMPONENTS
        };

        struct span {
            span ();
            span (const char*);

            const char *begin;
            const char *end;

            bool empty () const;
            size_t size (void) const;
            const char& operator[] (size_t) const;
        };

        span get (component);

        static std::string percent_decode (span);

    private:
        span m_spans[NUM_COMPONENTS];
        std::string m_value;
    };

    std::ostream& operator<< (std::ostream&, uri::component);
    std::ostream& operator<< (std::ostream&, uri::span);
}


#endif
