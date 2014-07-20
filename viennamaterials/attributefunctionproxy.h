/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef ATTRIBUTEFUNCTIONPROXY_H_
#define ATTRIBUTEFUNCTIONPROXY_H_

#include "viennamaterials/attributefunction.h"
#include "viennamaterials/attributefunctionpython.h"

namespace viennamaterials
{

class attribute_function_proxy
{
public:
  attribute_function_proxy();
  virtual ~attribute_function_proxy();

  std::vector<FunctionArgumentBase>   init(viennamaterials::library_handle& lib, std::string& xpath_query, xml_code_lang lang);
  void                                deinit();
  FunctionArgumentBase                evaluate(std::vector<FunctionArgumentBase> args);


private:
  IAttributeFunction* backend_; //FIXME: can this be done with reference?
};

} /* namespace viennamaterials */
#endif /* ATTRIBUTEFUNCTIONPROXY_H_ */
