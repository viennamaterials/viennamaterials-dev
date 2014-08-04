/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef XMLDATATYPES_H_
#define XMLDATATYPES_H_

#include "viennamaterials/platform.hpp"

namespace viennamaterials
{

/// Mapping of basic XML data types to C/C++ types

typedef bool            xml_bool;
typedef long            xml_int;
typedef numeric         xml_float;



/// Tags representing the XML datatypes

struct tag_xml_attribute_type {};

struct tag_scalar_bool      : tag_xml_attribute_type {};
struct tag_scalar_int {};
struct tag_scalar_float     : tag_xml_attribute_type {};

struct tag_tensor           : tag_xml_attribute_type {};

struct tag_function_bool    : tag_xml_attribute_type {};
struct tag_function_int     : tag_xml_attribute_type {};
struct tag_function_float   : tag_xml_attribute_type {};
struct tag_function_tensor  : tag_xml_attribute_type {};



/// Enum representing the XML datatypes

enum xml_attribute_types
{
  scalar_bool,
  scalar_int,
  scalar_float,
  tensor,
  function_bool,
  function_int,
  function_float,
  function_tensor,

  invalid
};
typedef enum xml_attribute_types xml_attribute_type;
typedef enum xml_attribute_types XmlAttributeType;


} /* namespace viennamaterials */
#endif /* XMLDATATYPES_H_ */
