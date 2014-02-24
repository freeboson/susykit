
#pragma once
#ifndef HEPSTATS_H
#define HEPSTATS_H

#if GCC_VERSION < 40600
const                        // this is a const object...
class {
public:
  template<class T>          // convertible to any type
    operator T*() const      // of null non-member
    { return 0; }            // pointer...
  template<class C, class T> // or any type of null
    operator T C::*() const  // member pointer...
    { return 0; }
private:
  void operator&() const;    // whose address can't be taken
} nullptr = {};              // and whose name is nullptr
#endif

namespace hepstats
{
	class loglike;	
	class likedatum;
	class likeconfig;

	namespace likedist
	{
		enum distribution
		{
			Gaussian,
			Poisson,
			Upper,
			Lower,
			Upper_Gaussian
		};
	}

	const double logZero = -1e90; // embarassing
}

#include "loglike.hpp"
#include "likedatum.hpp"
#include "likeconfig.hpp"

#endif

