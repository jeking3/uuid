// Boost random_generator.hpp header file  ----------------------------------------------//

// Copyright 2010 Andy Tompkins.
// Copyright 2017 James E. King III
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UUID_RANDOM_GENERATOR_HPP
#define BOOST_UUID_RANDOM_GENERATOR_HPP

#include <boost/assert.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/detail/seed_rng.hpp>
#include <boost/uuid/uuid.hpp>
#include <limits>

namespace boost {
namespace uuids {

//! \brief generates a random-based uuid
//! \param[in]  UniformRandomNumberGenerator  concept defined in the Boost.Random documentation
template <typename UniformRandomNumberGenerator>
class basic_random_generator {
private:
    typedef uniform_int<unsigned long> distribution_type;
    typedef variate_generator<UniformRandomNumberGenerator*, distribution_type> generator_type;

public:
    typedef uuid result_type;

    //! Default constructor acquires a scoped RNG and manages the lifetime of it.
    //! \note if using Visual Studio 2013 or earlier, the inputs are limited
    //!       to being a PseudoRandomNumberGenerator or boost::random_device
    //!       otherwise any UniformRandomNumberGenerator can be used.
    //! \post if the RNG is a PseudoRandomNumberGenerator with a seed method
    //!       then it is properly seeded.
    basic_random_generator()
        : pURNG(new UniformRandomNumberGenerator)
        , generator
          ( pURNG.get()
          , distribution_type
            ( (std::numeric_limits<unsigned long>::min)()
            , (std::numeric_limits<unsigned long>::max)()
            )
          )
    {
        detail::seed(*pURNG);
    }

    //! Use a provided RNG by reference.
    //! \param[in]  gen  the provided RNG
    explicit basic_random_generator(UniformRandomNumberGenerator& gen)
        : pURNG(&gen, boost::null_deleter())
        , generator
          ( pURNG.get()
          , distribution_type
            ( (std::numeric_limits<unsigned long>::min)()
            , (std::numeric_limits<unsigned long>::max)()
            )
          )
    {}

    //! Use a provided RNG by pointer.
    //! \param[in]  pGen  the provided RNG
    explicit basic_random_generator(UniformRandomNumberGenerator* pGen)
        : pURNG(pGen, boost::null_deleter())
        , generator
          ( pURNG.get()
          , distribution_type
            ( (std::numeric_limits<unsigned long>::min)()
            , (std::numeric_limits<unsigned long>::max)()
            )
          )
    {
        BOOST_ASSERT(pURNG);
    }

    //! Generate a random uuid.
    //! \returns a random uuid
    uuid operator()()
    {
        uuid u;

        int i=0;
        unsigned long random_value = generator();
        for (uuid::iterator it=u.begin(); it!=u.end(); ++it, ++i) {
            if (i==sizeof(unsigned long)) {
                random_value = generator();
                i = 0;
            }

            // static_cast gets rid of warnings of converting unsigned long to boost::uint8_t
            *it = static_cast<uuid::value_type>((random_value >> (i*8)) & 0xFF);
        }

        // set variant
        // must be 0b10xxxxxx
        *(u.begin()+8) &= 0xBF;
        *(u.begin()+8) |= 0x80;

        // set version
        // must be 0b0100xxxx
        *(u.begin()+6) &= 0x4F; //0b01001111
        *(u.begin()+6) |= 0x40; //0b01000000

        return u;
    }

private:
    shared_ptr<UniformRandomNumberGenerator> pURNG;
    generator_type generator;
};

typedef basic_random_generator<mt19937> random_generator;

}} // namespace boost::uuids

#endif // BOOST_UUID_RANDOM_GENERATOR_HPP

