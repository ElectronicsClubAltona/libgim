
#ifdef __UTIL_IO_IPP
#error "multiple inclusions"
#else
#define __UTIL__IO_IPP
#endif

namespace util {
    //-------------------------------------------------------------------------
    template <typename T>
    void
    write (const fd &_fd, const T *restrict data, size_t count)
    {
        auto first = reinterpret_cast<const char*> (data);
        write (_fd, first, first + sizeof (T) * count);
    }

    //-------------------------------------------------------------------------
    template <typename T>
    indented<T>::indented (const T &_data):
        data (_data)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T>
    indented<T>
    make_indented (const T &_data) {
        return indented<T> (_data);
    }


    //-------------------------------------------------------------------------
    template <typename T>
    std::ostream&
    operator<< (std::ostream &os, const util::indented<T> &&v) {
        util::indenter raii (os);
        os << v.data;
        return os;
    }
}
