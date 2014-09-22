/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef IPD2XML_HPP_
#define IPD2XML_HPP_

#include <iostream>
#include <sstream>
#include <exception>

#define VERBOSE_MODE

struct ipd2xml_error : public std::exception
{
  ipd2xml_error(std::string const& info) : info(info) {}
  ~ipd2xml_error() throw() {}

  virtual const char* what() const throw()
  {
    return info.c_str();
  }

  std::string info;
};

template < typename T >
std::string converter(T t)
{
   std::stringstream ss;
   ss << t;
   return ss.str();
}


#endif /* IPD2XML_HPP_ */
