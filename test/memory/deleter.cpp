#include "memory/deleter.hpp"
#include "tap.hpp"

#include <memory>


struct setter {
    setter (bool &_flag):
        flag (_flag)
    { ; }

    void release (unsigned *)
    { flag = true; }

    bool &flag;
};


int
main (void)
{
    bool flag = false;

    {
        // setup a pretend resource. we're not going to delete it, it's just
        // to satisfy some required parameters. importantly it can't be null
        // otherwise release is skipped.
        unsigned *foo = reinterpret_cast<unsigned*> (0x01);

        // setup the owner who will set a flag when the release operation is
        // called
        setter bar (flag);

        // setup our non-default unique_ptr
        std::unique_ptr<
            unsigned,
            util::memory::owner_deleter<
                unsigned,
                setter,
                &setter::release
            >
        > qux (foo, bar);
    }

    // the flag should have been implicitly set to true via the unique_ptr
    // callback above
    util::TAP::logger tap;
    tap.expect (flag, "owner_deleter calls registered callback");
    return tap.status ();
}
