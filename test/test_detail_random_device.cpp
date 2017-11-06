/* boost uuid/detail/random/random_device various tests
 *
 * Copyright (c) 2010 Steven Watanabe
 * Copyright (c) 2017 James E. King III
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENCE_1_0.txt)
 *
 * $Id$
 */

#include <boost/detail/lightweight_test.hpp>
#include <boost/uuid/detail/random/random_device.hpp>

int main(int, char*[])
{
    //
    // Basic concept implementation
    //

    using boost::uuids::detail::random::random_device;
    random_device rng;

    BOOST_TEST_EQ(0u, rng.min());
    BOOST_TEST_EQ(~0u, rng.max());

    for (int i = 0; i < 100; ++i) 
    {
        random_device::result_type val = rng();
        BOOST_TEST_GE(val, 0u);
        BOOST_TEST_LE(val, ~0u);
    }

    return boost::report_errors();
}
