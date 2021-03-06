#ifndef VIENNAMATERIALS_PROXY_HPP
#define VIENNAMATERIALS_PROXY_HPP

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
#include "viennamaterials/library.hpp"
#include "viennamaterials/quantity.hpp"

namespace viennamaterials {

struct proxy
{
public:
  proxy(viennamaterials::library_handle& matlib) : matlib_(matlib) {}
  virtual ~proxy() {}

  viennamaterials::library_handle& matlib() { return matlib_; }

  std::string         query_native  (std::string const& q)
  {
    return matlib_->query(q);
  }

  virtual std::string query          (std::string const& q) = 0;
  virtual std::string query_unit     (std::string const& q) = 0;
  virtual viennamaterials::numeric     query_value    (std::string const& q) = 0;
  virtual viennamaterials::quantity    query_quantity (std::string const& q) = 0;

private:
  viennamaterials::library_handle& matlib_;
};

} // viennamaterials

#endif
