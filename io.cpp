#include "io.hpp"

#include "debug.hpp"
#include "except.hpp"

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


using namespace std;


fd_ref::fd_ref (int _fd):
    m_fd (_fd)
{
    if (m_fd < 0)
        throw invalid_argument ("invalid file descriptor");
}


fd_ref::~fd_ref () {
    check (m_fd >= 0);
    close (m_fd);
}


fd_ref::operator int (void) const
    { return m_fd; }


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



