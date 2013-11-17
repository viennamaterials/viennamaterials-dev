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

#include <exception>

namespace viennamaterials
{
  struct non_unique_material_exception: public std::exception
  {
    non_unique_material_exception(std::string const& info) : info(info) {}
    ~non_unique_material_exception() throw() {}

    virtual const char* what() const throw()
    {
      return std::string("Material \""+info+"\" is not unique").c_str();
    }
    
    std::string info;
  };

  struct non_unique_parameter_exception: public std::exception
  {
    non_unique_parameter_exception(std::string const& info) : info(info) {}
    ~non_unique_parameter_exception() throw() {}

    virtual const char* what() const throw()
    {
      return std::string("Parameter \""+info+"\" is not unique").c_str();
    }
    
    std::string info;
  };

  struct dtd_load_error: public std::exception
  {
    virtual const char* what() const throw()
    {
      return std::string("Input material database valiadation could not be loaded").c_str();
    }
  };

  struct xml_load_error: public std::exception
  {
    virtual const char* what() const throw()
    {
      return std::string("Input material database could not be loaded").c_str();
    }
  };

  struct dtd_error: public std::exception
  {
    virtual const char* what() const throw()
    {
      return std::string("Input material database is not valid").c_str();
    }
  };

  struct xml_file_load_error: public std::exception
  {
    virtual const char* what() const throw()
    {
      return std::string("Input material database XML file does not exist").c_str();
    }
  };

  struct no_xml_file_error: public std::exception
  {
    virtual const char* what() const throw()
    {
      return std::string("Input file is not an XML file").c_str();
    }
  };

  struct xml_parse_error: public std::exception
  {
    xml_parse_error(std::string const& info) : info(info) {}
    ~xml_parse_error() throw() {}

    virtual const char* what() const throw()
    {
      return std::string("XML parse error: ").c_str();
    }
    
    std::string info;
  };


//  struct XmlQueryException : public std::runtime_error 
//  {
//     XmlQueryException(std::string const & str) : std::runtime_error(str) {}
//  };

//  struct InvalidXmlFileException : public std::runtime_error 
//  {
//     InvalidXmlFileException(std::string const & str) : std::runtime_error(str) {}
//  };

} // viennamaterials

#endif

