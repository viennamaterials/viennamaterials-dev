#ifndef VIENNAMATERIALS_UTILS_STRING_HPP
#define VIENNAMATERIALS_UTILS_STRING_HPP

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
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace viennamaterials {

inline std::string& ltrim(std::string &s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

inline std::string& rtrim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

inline std::string& trim(std::string &s)
{
  return ltrim(rtrim(s));
}

} // viennamaterials


#endif
