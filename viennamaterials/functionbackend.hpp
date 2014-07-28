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

namespace viennamaterials
{

class function_backend
{
public:
  virtual         ~function_backend() {};
  virtual void    init(std::string code, std::string function_name) = 0;
  virtual double  evaluate() = 0; //FIXME: return object, argument list
};

} /* namespace viennamaterials */
#endif /* FUNCTIONBACKEND_HPP_ */
