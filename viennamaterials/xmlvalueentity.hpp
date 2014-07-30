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
#include "xmldatatypes.h"

namespace viennamaterials
{

enum xml_types //TODO ?move to xmldatatypes.hpp? , ?replace with tags?
{
  type_bool,
  type_int,
  type_float,
  type_tensor
};
typedef enum xml_types  XmlType;
typedef enum xml_types  xml_type;


/**
 * class xml_value_entity is used as interface for XML scalar and tensor elements
 */
class xml_value_entity
{
public:
  virtual           ~xml_value_entity() {};
  void              set_name(std::string const& name);
  std::string       get_name() const;
  xml_type          get_type();
  //TODO: bool is_bool, is_int, ...
  void              set_index(size_t index);
  size_t            get_index();

  virtual void      set_value(xml_bool value)    = 0;
  virtual void      set_value(xml_int value)     = 0;
  virtual void      set_value(xml_float value)   = 0;
//  virtual void      set_value(void value)             = 0; //TODO arg

  virtual xml_bool  get_value(tag_scalar_bool tag)  = 0;
  virtual xml_int   get_value(tag_scalar_int tag)   = 0;
  virtual xml_float get_value(tag_scalar_float tag) = 0;
//  virtual void      get_value(tag_tensor tag)       = 0; //TODO return

protected:
  void              set_type(xml_type type);

private:
  size_t        index_; /// Argument index for the function backend
  std::string   name_;  /// Used as identifier for simulator
  xml_type      type_;  /// XML type of this value
};

} /* namespace viennamaterials */
#endif /* XMLVALUEENTITY_HPP_ */
