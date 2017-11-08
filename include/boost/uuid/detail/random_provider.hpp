//
// Copyright (c) 2017 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENCE_1_0.txt)
//
// Platform-specific random entropy provider
//

#ifndef BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP
#define BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP

#include <boost/core/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/limits.hpp>
#include <boost/static_assert.hpp>
#include <boost/uuid/entropy_error.hpp>

#include <boost/uuid/detail/random_provider_detect_platform.hpp>
#include BOOST_UUID_RANDOM_PROVIDER_BASE_INCLUDE()

namespace boost {
namespace uuids {
namespace detail {

//! \brief Contains code common to all random_provider implementations.
//! \note  random_provider_base is required to provide this method:
//!        void get_random_bytes(void *buf, size_t siz);
//! \note  noncopyable because of some base implementations so
//!        this makes it uniform across platforms to avoid any  
//!        porting surprises
class random_provider
    : public detail::random_provider_base,
      public noncopyable
{
public:
    //! Leverage the provider as a SeedSeq for
    //! PseudoRandomNumberGeneration seeing.
    //! \note: See Boost.Random documentation for more details
    template<class Iter>
    void generate(Iter first, Iter last)
    {
        BOOST_STATIC_ASSERT(sizeof(Iter) >= sizeof(unsigned int));
        for (; first != last; ++first)
        {
            get_random_bytes(&*first, sizeof(*first));
            *first &= (std::numeric_limits<unsigned int>::max)();
        }
    }

    //! Return the name of the selected provider
    const char * name() const
    {
        return BOOST_UUID_RANDOM_PROVIDER_STRINGIFY(BOOST_UUID_RANDOM_PROVIDER_NAME);
    }
};

} // detail
} // uuids
} // boost

#endif // BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP
