/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef FUNCTIONBACKEND_HPP_
#define FUNCTIONBACKEND_HPP_

#include <string>
#include <vector>
#include "viennamaterials/xmldatatypes.h"
#include "viennamaterials/xmlvalueentity.hpp"

namespace viennamaterials
{

class function_backend
{
public:
  virtual           ~function_backend() {};
  virtual void      init(std::string const& code, std::string const& function_name) = 0;
  virtual xml_bool  eval(tag_scalar_bool tag,   std::vector<xml_value_entity*> &args)  = 0;
  virtual xml_int   eval(tag_scalar_int tag,    std::vector<xml_value_entity*> &args)  = 0;
  virtual xml_float eval(tag_scalar_float tag,  std::vector<xml_value_entity*> &args)  = 0;
//  virtual void      evaluate(tag_tensor) = 0; //FIXME: argument list, return
};

} /* namespace viennamaterials */
#endif /* FUNCTIONBACKEND_HPP_ */
