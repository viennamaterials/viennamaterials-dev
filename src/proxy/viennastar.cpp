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
#include "viennamaterials/proxy/viennastar.hpp"
#include "viennamaterials/utils/string.hpp"
#include "viennamaterials/utils/convert.hpp"

namespace viennamaterials
{

viennastar_proxy::viennastar_proxy(viennamaterials::backend_handle& matlib) :
  viennamaterials::proxy(matlib),
  placeholder_    ("%"),
  token_          ('/'),
  sub_path_       ("/*[id=\""+placeholder_+"\"]"),
  value_path_     ("/scalar/text()"),
  unit_path_      ("/unit/text()"),
  path_prefix_    ("/database")
{
}

viennastar_proxy::~viennastar_proxy()
{
}

std::string viennastar_proxy::query(std::string const& q)
{
  std::string base_path;
  generate_base_path(q, base_path);
  return matlib()->query(base_path);
}

std::string viennastar_proxy::query_unit(std::string const& q)
{
  std::string base_path;
  generate_base_path(q, base_path);
  base_path += unit_path_;
  return matlib()->query(base_path);
}

xml_bool  viennastar_proxy::query_value_bool(std::string const& q)
{
  std::string base_path;
  generate_base_path(q, base_path);
  base_path += value_path_;

  std::string value = matlib()->query(base_path);
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);
  if(value.compare("true") == 0)
    return true;
  else if(value.compare("false") == 0)
    return false;
  else
    throw proxy_error("Invalid boolean value encountered (query: " + base_path+ ")");
}

xml_int   viennastar_proxy::query_value_int(std::string const& q)
{
  std::string base_path;
  generate_base_path(q, base_path);
  base_path += value_path_;
  return convert<xml_int>(matlib()->query(base_path));
}

xml_float viennastar_proxy::query_value_float(std::string const& q)
{
  std::string base_path;
  generate_base_path(q, base_path);
  base_path += value_path_;
  return convert<xml_float>(matlib()->query(base_path));
}

viennamaterials::quantity<xml_bool>   viennastar_proxy::query_quantity_bool(std::string const& q)
{
  return quantity<xml_bool>(query_value<xml_bool>(q), query_unit(q));
}

viennamaterials::quantity<xml_int>    viennastar_proxy::query_quantity_int(std::string const& q)
{
  return quantity<xml_int>(query_value<xml_int>(q), query_unit(q));
}

viennamaterials::quantity<xml_float>  viennastar_proxy::query_quantity_float(std::string const& q)
{
  return quantity<xml_float>(query_value<xml_float>(q), query_unit(q));
}

void viennastar_proxy::generate_base_path(std::string const& q, std::string & base_path)
{
  query_parts_.clear();
  viennamaterials::split(query_parts_, q, token_);
  base_path = path_prefix_;
  for(QueryPartsType::const_iterator iter = query_parts_.begin();
      iter != query_parts_.end(); iter++)
  {
    std::string temp_path = sub_path_;
    viennamaterials::replace_first(temp_path, placeholder_, *iter);
    base_path += temp_path;
  }
}

} // viennamaterials
