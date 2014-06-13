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

struct quantity
{
public:
  quantity();
  quantity(numeric const& value, std::string const& unit);

  // provide read/write access
  //
  numeric     & value();
  std::string & unit();

  // provide const read access
  //
  numeric     const& value() const;
  std::string const& unit()  const;


private:
  numeric     value_;
  std::string unit_;
};

} // viennamaterials

std::ostream& operator<<(std::ostream& os, viennamaterials::quantity const& quan);

#endif
