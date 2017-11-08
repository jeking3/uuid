//
// Copyright (c) 2017 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENCE_1_0.txt)
//
// Platform-specific random entropy provider platform detection
//

#ifndef BOOST_UUID_DETAIL_RANDOM_PROVIDER_PLATFORM_DETECTION_HPP
#define BOOST_UUID_DETAIL_RANDOM_PROVIDER_PLATFORM_DETECTION_HPP

#include <boost/predef/library/c/gnu.h>
#include <boost/predef/platform/cloudabi.h>
// #include <boost/predef/os/bsd/open.h> re-enable when Boost.Predef #66 clears
#include <boost/predef/os/windows.h>

//
// Platform Detection - will load in the correct header and
// will define the class <tt>random_provider_base</tt>.
//

#if BOOST_PLAT_CLOUDABI
# define BOOST_UUID_RANDOM_PROVIDER_CLOUDABI
# define BOOST_UUID_RANDOM_PROVIDER_NAME random_provider_cloudabi

#elif BOOST_OS_WINDOWS
# include <boost/winapi/config.hpp>
# if BOOST_WINAPI_PARTITION_APP_SYSTEM && \
     !defined(BOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT) && \
     !defined(_WIN32_WCE) && \
     (defined(BOOST_WINAPI_IS_MINGW_W64) || \
        (BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN6))
#  define BOOST_UUID_RANDOM_PROVIDER_BCRYPT
#  define BOOST_UUID_RANDOM_PROVIDER_NAME random_provider_bcrypt

# elif BOOST_WINAPI_PARTITION_DESKTOP || BOOST_WINAPI_PARTITION_SYSTEM
#  define BOOST_UUID_RANDOM_PROVIDER_WINCRYPT
#  define BOOST_UUID_RANDOM_PROVIDER_NAME random_provider_wincrypt
# else
#  error Unable to find a suitable windows entropy provider
# endif

#elif BOOST_LIB_C_GNU >= BOOST_VERSION_NUMBER(2, 25, 0) \
   && !defined(BOOST_UUID_RANDOM_PROVIDER_FORCE_POSIX)
/* when Boost.Prefef #6 merges, allow this based on OpenBSD documentation:
BOOST_OS_BSD_OPEN >= BOOST_VERSION_NUMBER(5, 6, 0)) */
# define BOOST_UUID_RANDOM_PROVIDER_GETENTROPY
# define BOOST_UUID_RANDOM_PROVIDER_NAME random_provider_getentropy

#else
# define BOOST_UUID_RANDOM_PROVIDER_POSIX
# define BOOST_UUID_RANDOM_PROVIDER_NAME random_provider_posix

#endif

#define BOOST_UUID_RANDOM_PROVIDER_STRINGIFY2(X) #X
#define BOOST_UUID_RANDOM_PROVIDER_STRINGIFY(X) BOOST_UUID_RANDOM_PROVIDER_STRINGIFY2(X)

#define BOOST_UUID_RANDOM_PROVIDER_BASE_INCLUDE() <boost/uuid/detail/BOOST_UUID_RANDOM_PROVIDER_NAME.ipp>

#if defined(BOOST_UUID_RANDOM_PROVIDER_SHOW)
#pragma message("BOOST_UUID_RANDOM_PROVIDER_NAME " BOOST_UUID_RANDOM_PROVIDER_STRINGIFY(BOOST_UUID_RANDOM_PROVIDER_NAME))
#pragma message("BOOST_UUID_RANDOM_PROVIDER_BASE_INCLUDE " BOOST_UUID_RANDOM_PROVIDER_STRINGIFY(BOOST_UUID_RANDOM_PROVIDER_BASE_INCLUDE()))
#endif

#endif // BOOST_UUID_DETAIL_RANDOM_PROVIDER_PLATFORM_DETECTION_HPP
