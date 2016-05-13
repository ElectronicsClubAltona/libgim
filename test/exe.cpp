#include "./exe.hpp"
#include "./tap.hpp"

#include <iostream>

int
main (void)
{
	util::TAP::logger tap;
	tap.expect_eq (util::image_path ().stem (), "exe", "identify executable path");
	return tap.status ();
}
