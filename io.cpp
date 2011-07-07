#include "io.hpp"

#include "debug.hpp"
#include "except.hpp"

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


using namespace std;

//----------------------------------------------------------------------------
uint8_t *
slurp (const boost::filesystem::path& path)  {
    fd_ref fd(open (path.string ().c_str (), O_RDONLY)); // | O_CLOEXEC));
    
    // Calculate the total file size
    off_t size = lseek (fd, 0, SEEK_END);
    if (size == (off_t)-1)
        throw errno_error();

    if (lseek (fd, 0, SEEK_SET) == (off_t)-1)
        throw errno_error ();

    // Allocate a buffer, and keep reading until it's full. We provide a null
    // padding at the tail as a 'just in case' measure for string manipulation.
    unique_ptr <uint8_t[]> buffer (new uint8_t[size + 1]);
    buffer.get()[size] = '\0';

    check_hard (size >= 0);
    size_t remaining = (size_t)size;
    uint8_t *cursor = buffer.get();

    while (remaining) {
        ssize_t consumed = read (fd, cursor, remaining);
        if (consumed == -1)
            throw errno_error();
        check_hard (        consumed >= 0);
        check_hard ((size_t)consumed <= remaining);

        remaining -= (size_t)consumed;
        cursor    += consumed;
    }

    return buffer.release();
}

//----------------------------------------------------------------------------
fd_ref::fd_ref (int _fd):
    fd (_fd)
{
    if (fd < 0) 
        throw invalid_argument ("invalid file descriptor");
}


fd_ref::~fd_ref () {
    check (fd >= 0);
    close (fd);
}


fd_ref::operator int (void) const
    { return fd; }


#if defined(HAVE_MMAP)
#include <sys/mman.h>

//----------------------------------------------------------------------------
mapped_file::mapped_file (const char *_path):
    m_fd (open (_path, O_RDONLY))
{ load_fd (); } 


mapped_file::mapped_file (const std::string &_path):
    m_fd (open (_path.c_str (), O_RDONLY))
{ load_fd (); } 


mapped_file::mapped_file (const boost::filesystem::path &_path):
    m_fd (open (_path.native ().c_str (), O_RDONLY))
{ load_fd (); } 


mapped_file::~mapped_file () {
    check (m_data != NULL);
    munmap (m_data, m_size);
}


void
mapped_file::load_fd (void) {
    struct stat meta;
    if (fstat (m_fd, &meta) < 0)
        throw errno_error ();

    m_size = (size_t)meta.st_size;
    m_data = (uint8_t *)mmap (NULL, m_size, PROT_READ, MAP_PRIVATE, m_fd, 0);
    if (m_data == MAP_FAILED)
        throw errno_error ();
}


size_t
mapped_file::size (void) const {
    check (m_size > 0);
    check (m_data != NULL);

    return m_size;
}


const uint8_t*
mapped_file::data (void) const {
    check (m_size > 0);
    check (m_data != NULL);

    return m_data;
}
#endif


