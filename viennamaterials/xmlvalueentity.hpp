/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef XMLVALUEENTITY_HPP_
#define XMLVALUEENTITY_HPP_

#include <string>
#include "viennamaterials/platform.hpp"

namespace viennamaterials
{

enum xml_types
{
  scalar_bool,
  scalar_int,
  scalar_float,
  tensor
};
typedef enum xml_types  XmlType;
typedef enum xml_types  xml_type;


typedef bool            xml_bool;
typedef bool            XmlBool;

typedef int             xml_int;
typedef int             XmlInt;

typedef numeric         xml_float;
typedef numeric         XmlFloat;


/**
 * class xml_value_entity is used as interface for XML scalar and tensor elements
 */
class xml_value_entity
{
public:
  virtual       ~xml_value_entity() {};
  void          set_name(std::string name);
  std::string   get_name() const;
  void          set_type(xml_type type);
  xml_type      get_type();
  virtual void* get_value_ptr() = 0;
  virtual void  set_value(void* value) = 0;

private:
  std::string   name;
  xml_type      type;
};

} /* namespace viennamaterials */
#endif /* XMLVALUEENTITY_HPP_ */
