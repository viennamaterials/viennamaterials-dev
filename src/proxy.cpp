/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/proxy.hpp"

namespace viennamaterials
{

template<typename T>
T proxy::query_value_dispatch(std::string const& q)
{
  throw proxy_error("Template type not supported");
}

template<>
xml_bool proxy::query_value_dispatch<xml_bool>(std::string const& q)
{
  return query_value_bool(q);
}

template<>
xml_int proxy::query_value_dispatch<xml_int>(std::string const& q)
{
  return query_value_int(q);
}

template<>
xml_float proxy::query_value_dispatch<xml_float>(std::string const& q)
{
  return query_value_float(q);
}

template<typename T>
viennamaterials::quantity<T> proxy::query_quantity_dispatch(std::string const& q)
{
  throw proxy_error("Template type not supported");
}

template<>
viennamaterials::quantity<xml_bool> proxy::query_quantity_dispatch<xml_bool>(std::string const& q)
{
  return query_quantity_bool(q);
}

template<>
viennamaterials::quantity<xml_int> proxy::query_quantity_dispatch<xml_int>(std::string const& q)
{
  return query_quantity_int(q);
}

template<>
viennamaterials::quantity<xml_float> proxy::query_quantity_dispatch<xml_float>(std::string const& q)
{
  return query_quantity_float(q);
}

} /* namespace viennamaterials */
