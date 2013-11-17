#ifndef VIENNAMATERIALS_UTILS_HPP
#define VIENNAMATERIALS_UTILS_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include <fstream>

namespace viennamaterials {


inline bool file_exists(std::string const& filename)
{
   std::ifstream ifile(filename.c_str());
   return ifile;
}

inline std::string file_extension(std::string const& filename)
{
   return filename.substr(filename.rfind(".")+1, filename.size());
}


} // viennamaterials

#endif

