#ifndef VIENNAMATERIALS_PROXY_HP
#define VIENNAMATERIALS_PROXY_HP

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
#include "viennamaterials/quantity.hpp"

namespace viennamaterials {

struct proxy
{
public:
  proxy(viennamaterials::library_handle& matlib) : matlib_(matlib) {}
  virtual ~proxy() {}

  viennamaterials::library_handle& matlib() { return matlib_; }

//  virtual std::string query         (std::string const& q) = 0;
//  virtual std::string query_unit    (std::string const& q) = 0;
//  virtual numeric     query_value   (std::string const& q) = 0;
//  virtual quantity    query_quantity(std::string const& q) = 0;

private:
  viennamaterials::library_handle& matlib_;
};

} // viennamaterials

#endif
