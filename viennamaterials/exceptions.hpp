#ifndef VIENNAMATERIALS_EXCEPTIONS_HPP
#define VIENNAMATERIALS_EXCEPTIONS_HPP

/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// System includes
//
#include <iostream>
#include <exception>

namespace viennamaterials
{
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

  struct entry_not_found_exception : public std::exception
  {
    entry_not_found_exception(std::string const& info) : info(info) {}
    ~entry_not_found_exception() throw() {}

    virtual const char* what() const throw()
    {
      return info.c_str();
    }

    std::string info;
  };



} // viennamaterials

#endif
