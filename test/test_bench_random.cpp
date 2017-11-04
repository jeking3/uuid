/* boost uuid/detail/random benchmark tests
 *
 * Copyright (c) 2010 Steven Watanabe
 * Copyright (c) 2017 James E. King III
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENCE_1_0.txt)
 *
 * $Id$
 */

#include <boost/core/ignore_unused.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/timer/timer.hpp>
#include <boost/uuid/detail/random/random_device.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

BOOST_CONSTEXPR_OR_CONST size_t REUSE_LOOPS(1000000);
BOOST_CONSTEXPR_OR_CONST size_t GEN_LOOPS  (  10000);

template<class Generator>
boost::timer::cpu_times run_generator(size_t count)
{
    boost::timer::cpu_timer t;
    Generator gen;
    for (size_t i = 0; i < count; ++i)
    {
        boost::ignore_unused(gen());
    }
    return t.elapsed();
}

int main(int, char*[])
{
    boost::uuids::uuid u;

    //
    // Two common use cases:
    //
    // Use an OS provided RNG which has no seed code but is slower to reuse
    // Use a PRNG which is expensive to seed once but fast to reuse
    //
    // Measure the default selections of the library

    std::cout << "Benchmark boost::uuids::random_generator "
              << "(reused for " << REUSE_LOOPS << " loops):" << std::endl;
    {
        boost::timer::auto_cpu_timer t;
        boost::uuids::random_generator gen;
        for (int i = 0; i < REUSE_LOOPS; ++i)
        {
            boost::uuids::uuid u2 = gen();
            BOOST_TEST_NE(u, u2);
            u = u2;
        }
    }

    std::cout << std::endl;
    std::cout << "Benchmark boost::uuids::random_generator_bulk "
        << "(reused for " << REUSE_LOOPS << " loops):" << std::endl;
    {
        boost::timer::auto_cpu_timer t;
        boost::uuids::random_generator_bulk gen;
        for (int i = 0; i < REUSE_LOOPS; ++i)
        {
            boost::uuids::uuid u2 = gen();
            BOOST_TEST_NE(u, u2);
            u = u2;
        }
    }

    std::cout << std::endl;
    std::cout << "Benchmark boost::uuids::random_generator "
              << "(new generator each loop for " << GEN_LOOPS << " loops):" << std::endl;
    {
        boost::timer::auto_cpu_timer t;
        for (int i = 0; i < 20000; ++i)
        {
            boost::uuids::random_generator gen;
            boost::uuids::uuid u2 = gen();
            BOOST_TEST_NE(u, u2);
            u = u2;
        }
    }

    std::cout << std::endl;
    std::cout << "Benchmark boost::uuids::random_generator_bulk "
              << "(new generator each loop for " << GEN_LOOPS << " loops):" << std::endl;
    {
        boost::timer::auto_cpu_timer t;
        for (int i = 0; i < 20000; ++i)
        {
            boost::uuids::random_generator_bulk gen;
            boost::uuids::uuid u2 = gen();
            BOOST_TEST_NE(u, u2);
            u = u2;
        }
    }

    //
    // Determines the cutoff point where it is more wall-clock efficient to
    // use the bulk generator over the standard one.
    //

    size_t answer = 0;
    for (size_t count = 1; !answer; ++count)
    {
        boost::timer::cpu_times standard = run_generator<boost::uuids::random_generator     >(count);
        boost::timer::cpu_times bulk     = run_generator<boost::uuids::random_generator_bulk>(count);
        if (standard.wall > bulk.wall)
        {
            answer = count;
        }
    }
    std::cout << std::endl;
    std::cout << "For this platform, random_generator_bulk outperforms "
              << "random_generator after " << answer << " generations."
              << std::endl;

    return boost::report_errors();
}
