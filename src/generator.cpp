/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/generator.hpp"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/utils/file_extension.hpp"

namespace viennamaterials {

viennamaterials::library* generator(std::string const& filename)
{
  std::string extension = viennamaterials::file_extension(filename);
  if(extension == "xml")
    return new viennamaterials::pugixml(filename);
  else
    return NULL;
}

} // viennamaterials


