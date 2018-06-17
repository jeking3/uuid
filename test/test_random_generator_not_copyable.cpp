//
// Copyright (c) 2018 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENCE_1_0.txt)
//
// Positive and negative testing for detail::random_provider
//

#include <boost/detail/lightweight_test.hpp>
#include <boost/uuid/random_generator.hpp>

int main(int, char*[])
{
    // random generator (pure, default type) cannot be copied as
    // it carries operating system resources on some platforms
    boost::uuids::random_generator gen;
    boost::uuids::random_generator gen2(gen);
    /* NOTREACHED */ return 0;
}
