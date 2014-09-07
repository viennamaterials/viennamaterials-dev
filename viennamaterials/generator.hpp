#ifndef VIENNAMATERIALS_GENERATOR_HPP
#define VIENNAMATERIALS_GENERATOR_HPP

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
#include "viennamaterials/backend.hpp"

namespace viennamaterials {

viennamaterials::backend_handle generator(std::string const& filename);

} // viennamaterials

#endif
