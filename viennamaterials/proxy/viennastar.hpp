#ifndef VIENNAMATERIALS_PROXY_VIENNASTAR_HPP
#define VIENNAMATERIALS_PROXY_VIENNASTAR_HPP

/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// System includes
//
#include <vector>

// ViennaMaterials includes
//
#include "viennamaterials/proxy.hpp"

namespace viennamaterials
{

struct viennastar_proxy : public viennamaterials::proxy
{
private:
  typedef std::vector<std::string>    QueryPartsType;

public:
  viennastar_proxy(viennamaterials::library_handle& matlib);
  ~viennastar_proxy();

  std::string query          (std::string const& q);

  std::string query_unit     (std::string const& q);

//  viennamaterials::numeric     query_value    (std::string const& q); //XXX

private:
  void generate_base_path(std::string const& q, std::string & base_path);

  xml_bool  query_value_bool(std::string const& q);
  xml_int   query_value_int(std::string const& q);
  xml_float query_value_float(std::string const& q);

  viennamaterials::quantity<xml_bool>   query_quantity_bool(std::string const& q);
  viennamaterials::quantity<xml_int>    query_quantity_int(std::string const& q);
  viennamaterials::quantity<xml_float>  query_quantity_float(std::string const& q);

  const std::string placeholder_;
  const char        token_;
  const std::string sub_path_;
  const std::string value_path_;
  const std::string unit_path_;

  QueryPartsType query_parts_;
};


} // viennamaterials

#endif
