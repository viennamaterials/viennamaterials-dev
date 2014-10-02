/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/library.hpp"
#include "viennamaterials/libraryxpath.hpp"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/utils/file_extension.hpp"

namespace viennamaterials
{

library::library(std::string const& filename)
{
  std::string extension = file_extension(filename);
  if(extension == "xml")
    frontend_ = shared_ptr<library_interface>(new library_xpath(filename));
  else
    throw library_error("Invalid file extension encountered (extension found: " + extension + ")");
}

attribute_handle library::query(std::string const& query_to_attribute)
{
  return frontend_->query(query_to_attribute);
}

} /* namespace viennamaterials */
