//
// Copyright (c) 2017, 2018 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENCE_1_0.txt)
//
// "A Replacement Call for Random"
// https://man.openbsd.org/arc4random.3
//

#include <boost/move/core.hpp>
#include <stdlib.h> 

namespace boost {
namespace uuids {
namespace detail {

class random_provider_base
{
  private:
    // while this implementation is safely copyable, we disable
    // it to ensure code written against it is portable to
    // other implementations that are not safely copyable
    BOOST_MOVABLE_BUT_NOT_COPYABLE(random_provider_base)

  public:
    random_provider_base() { }

    //! Obtain entropy and place it into a memory location
    //! \param[in]  buf  the location to write entropy
    //! \param[in]  siz  the number of bytes to acquire
    void get_random_bytes(void *buf, size_t siz)
    {
        arc4random_buf(buf, siz);
    }
};

} // detail
} // uuids
} // boost
