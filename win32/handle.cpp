#include "handle.hpp"

#include "../except.hpp"

using util::win32::handle;


///////////////////////////////////////////////////////////////////////////////
handle::handle ():
    m_native (INVALID_HANDLE_VALUE)
{ ; }


//-----------------------------------------------------------------------------
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
    if (m_native != INVALID_HANDLE_VALUE)
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
void
handle::reset (HANDLE rhs)
{
    if (m_native != INVALID_HANDLE_VALUE)
        CloseHandle (m_native);
    m_native = rhs;
}


///////////////////////////////////////////////////////////////////////////////
handle
handle::current_process (void)
{
    return handle (GetCurrentProcess ());
}
