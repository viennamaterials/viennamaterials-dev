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

namespace viennamaterials
{

viennastar_proxy::viennastar_proxy(viennamaterials::library_handle& matlib) :
  viennamaterials::proxy(matlib),
  placeholder_    ("%"),
  token_          ('/'),
  sub_path_       ("/*[id=\""+placeholder_+"\"]"),
  value_path_     ("/value/text()"),
  unit_path_      ("/unit/text()")
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

//viennamaterials::numeric viennastar_proxy::query_value(std::string const& q) //XXX
//{
//  std::string base_path;
//  generate_base_path(q, base_path);
//  base_path += value_path_;
//  return matlib()->query_value(base_path);
//}

xml_bool  viennastar_proxy::query_value_bool(std::string const& q) {} //FIXME
xml_int   viennastar_proxy::query_value_int(std::string const& q) {} //FIXME
xml_float viennastar_proxy::query_value_float(std::string const& q)
{
  std::string base_path;
  generate_base_path(q, base_path);
  base_path += value_path_;
  return matlib()->query_value(base_path); //FIXME use template?, use xml function backend?
}

viennamaterials::quantity<xml_bool>   viennastar_proxy::query_quantity_bool(std::string const& q) {} //FIXME
viennamaterials::quantity<xml_int>    viennastar_proxy::query_quantity_int(std::string const& q) {} //FIXME
viennamaterials::quantity<xml_float>  viennastar_proxy::query_quantity_float(std::string const& q)
{
  return quantity<xml_float>(query_value<xml_float>(q), query_unit(q));
}

void viennastar_proxy::generate_base_path(std::string const& q, std::string & base_path)
{
  query_parts_.clear();
  viennamaterials::split(query_parts_, q, token_);
  for(QueryPartsType::const_iterator iter = query_parts_.begin();
      iter != query_parts_.end(); iter++)
  {
    std::string temp_path = sub_path_;
    viennamaterials::replace_first(temp_path, placeholder_, *iter);
    base_path += temp_path;
  }
}

} // viennamaterials
