#ifndef VIENNAMATERIALS_QUANTITY_HPP
#define VIENNAMATERIALS_QUANTITY_HPP

/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/forwards.h"

namespace viennamaterials {

template<typename T>
struct quantity
{
public:
  quantity() {}
  quantity(T const& value, std::string const& unit) : value_(value), unit_(unit) {}

  // provide read/write access
  //
  T     & value() { return value_; }
  std::string & unit() { return unit_;  }

  // provide const read access
  //
  T     const& value() const { return value_; }
  std::string const& unit()  const { return unit_;  }


private:
  T     value_;
  std::string unit_;
};

} // viennamaterials

template<typename T>
std::ostream& operator<<(std::ostream& os, viennamaterials::quantity<T> const& quan)
{
  os << quan.value() << " " << quan.unit();
  return os;
}

#endif
