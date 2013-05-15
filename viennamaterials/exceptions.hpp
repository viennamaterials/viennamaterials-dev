#ifndef VIENNAMATERIALS_EXCEPTIONS_HPP
#define VIENNAMATERIALS_EXCEPTIONS_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include <iostream>
#include <exception>

namespace vmat
{

//struct VMATNonUniqueMaterialException : public std::runtime_error 
//{
//   VMATNonUniqueMaterialException(std::string const & str) : std::runtime_error(str) {}
//};

struct VMATNonUniqueMaterialException: public std::exception
{
  VMATNonUniqueMaterialException(std::string const& info) : info(info) {}
  ~VMATNonUniqueMaterialException() throw() {}

  virtual const char* what() const throw()
  {
    return std::string("Material \""+info+"\" is not unique").c_str();
  }
  
  std::string info;
};

struct VMATNonUniqueParameterException: public std::exception
{
  VMATNonUniqueParameterException(std::string const& info) : info(info) {}
  ~VMATNonUniqueParameterException() throw() {}

  virtual const char* what() const throw()
  {
    return std::string("Parameter \""+info+"\" is not unique").c_str();
  }
  
  std::string info;
};

} // end namespace vmat

#endif
