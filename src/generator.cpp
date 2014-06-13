/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/generator.hpp"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/utils/file_extension.hpp"

namespace viennamaterials {

viennamaterials::library_handle generator(std::string const& filename)
{
  std::string extension = viennamaterials::file_extension(filename);
  if(extension == "xml")
    return viennamaterials::library_handle(new viennamaterials::pugixml(filename));
  else
    return viennamaterials::library_handle();
}

} // viennamaterials
