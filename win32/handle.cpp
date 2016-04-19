#include "./handle.hpp"

#include "../except.hpp"

using util::win32::handle;


///////////////////////////////////////////////////////////////////////////////
handle::handle (HANDLE &&h):
    m_native (h)
{ ; }


//-----------------------------------------------------------------------------
handle::handle (handle &&rhs):
    m_native (rhs.m_native)
{
    rhs.m_native = nullptr;
}


//-----------------------------------------------------------------------------
handle::~handle ()
{
    CloseHandle (m_native);
}


///////////////////////////////////////////////////////////////////////////////
handle::operator HANDLE& (void) &
{
    return native ();
}


//-----------------------------------------------------------------------------
HANDLE&
handle::native (void) &
{
    return m_native;
}


//-----------------------------------------------------------------------------
const HANDLE&
handle::native (void) const &
{
    return m_native;
}


///////////////////////////////////////////////////////////////////////////////
handle
handle::current_process (void)
{
    auto process = GetCurrentProcess ();
    if (!process)
        util::win32_error::throw_code ();

    return process;
}
