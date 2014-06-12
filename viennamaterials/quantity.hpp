#ifndef VIENNAMATERIALS_QUANTITY_HP
#define VIENNAMATERIALS_QUANTITY_HP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/forwards.h"

namespace viennamaterials {

struct quantity
{
public:
  quantity() {}
  quantity(numeric const& value, std::string const& unit):
    value_(value), unit_(unit) {}

  numeric&      value() { return value_; }
  std::string&  unit()  { return unit_;  }

private:
  numeric     value_;
  std::string unit_;
};

} // viennamaterials

#endif
