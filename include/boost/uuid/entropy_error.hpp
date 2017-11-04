//
// Copyright (c) 2017 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENCE_1_0.txt)
//
// Entropy error class
//

#ifndef BOOST_UUID_RANDOM_ENTROPY_ERROR_HPP
#define BOOST_UUID_RANDOM_ENTROPY_ERROR_HPP

#include <boost/cstdint.hpp>
#include <stdexcept>

namespace boost {
namespace uuids {

//! \brief Given boost::system::system_error is in a module that
//!        is not header-only, we define our own exception type
//!        to handle entropy provider errors instead,
class entropy_error : public std::runtime_error
{
public:
    entropy_error(boost::intmax_t errCode, const std::string& message)
        : std::runtime_error(message)
        , m_errcode(errCode)
    {
    }

    virtual boost::intmax_t errcode() const
    {
        return m_errcode;
    }

private:
    boost::intmax_t m_errcode;
};

} // uuids
} // boost

#endif // BOOST_UUID_RANDOM_ENTROPY_ERROR_HPP
