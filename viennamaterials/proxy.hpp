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
#include "viennamaterials/backend.hpp"
#include "viennamaterials/quantity.hpp"
#include "viennamaterials/xmldatatypes.h"
#include "viennamaterials/exceptions.hpp"

namespace viennamaterials {

struct proxy
{
public:
  proxy(viennamaterials::backend_handle& matlib) : matlib_(matlib) {}
  virtual ~proxy() {}

  viennamaterials::backend_handle& matlib() { return matlib_; }

  std::string         query_native  (std::string const& q)
  {
    return matlib_->query(q);
  }

  virtual std::string               query          (std::string const& q) = 0;
  virtual std::string               query_unit     (std::string const& q) = 0;

  template<typename T>
  T                                 query_value (std::string const& q)
  {
    return this->query_value_dispatch<T>(q);
  }

  template<typename T>
  viennamaterials::quantity<T>      query_quantity (std::string const& q)
  {
    return this->query_quantity_dispatch<T>(q);
  }

private:
  template<typename T>
  T                   query_value_dispatch (std::string const& q);

  virtual xml_bool    query_value_bool (std::string const& q)   = 0;
  virtual xml_int     query_value_int (std::string const& q)    = 0;
  virtual xml_float   query_value_float (std::string const& q)  = 0;
  virtual xml_string  query_value_string (std::string const& q) = 0;

  template<typename T>
  viennamaterials::quantity<T>                  query_quantity_dispatch (std::string const& q);

  virtual viennamaterials::quantity<xml_bool>   query_quantity_bool(std::string const& q)   = 0;
  virtual viennamaterials::quantity<xml_int>    query_quantity_int(std::string const& q)    = 0;
  virtual viennamaterials::quantity<xml_float>  query_quantity_float(std::string const& q)  = 0;
  virtual viennamaterials::quantity<xml_string> query_quantity_string(std::string const& q) = 0;

private:
  viennamaterials::backend_handle& matlib_;
};

} // viennamaterials

#endif
