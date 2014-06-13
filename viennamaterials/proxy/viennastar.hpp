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

  std::string query          (std::string const& q);

  std::string query_unit     (std::string const& q);

  numeric     query_value    (std::string const& q);

  quantity    query_quantity (std::string const& q);

private:
  void generate_base_path(std::string const& q, std::string & base_path);

  const std::string placeholder_;
  const std::string token_;
  const std::string sub_path_;
  const std::string value_path_;
  const std::string unit_path_;

  QueryPartsType query_parts_;
};


} // viennamaterials

#endif
