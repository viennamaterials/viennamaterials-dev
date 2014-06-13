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
#include "viennamaterials/quantity.hpp"

namespace viennamaterials {

quantity::quantity() {}

quantity::quantity(numeric const& value, std::string const& unit) :
    value_(value), unit_(unit)
{
}

viennamaterials::numeric & quantity::value() { return value_; }
std::string              & quantity::unit()  { return unit_;  }

viennamaterials::numeric const& quantity::value() const { return value_; }
std::string              const& quantity::unit()  const { return unit_;  }

} // viennamaterials

std::ostream& operator<<(std::ostream& os, viennamaterials::quantity const& quan)
{
    os << quan.value() << " " << quan.unit();
    return os;
}
