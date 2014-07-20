/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "../viennamaterials/attributefunctionproxy.h"

namespace viennamaterials
{

std::vector<FunctionArgumentBase> attribute_function_proxy::init(viennamaterials::library_handle& lib, std::string& xpath_query, xml_code_lang lang)
{
  if(lang == python)
  {
    backend_ = new attribute_function_python;
    return backend_->init(lib, xpath_query);
  }

  std::vector<FunctionArgumentBase> empty_vector;
  return empty_vector; //empty vector indicates invalid init
}

void attribute_function_proxy::deinit()
{
  backend_->deinit();
  delete backend_;
}

FunctionArgumentBase attribute_function_proxy::evaluate(std::vector<FunctionArgumentBase> args)
{
  return backend_->evaluate(args);
}

} /* namespace viennamaterials */
