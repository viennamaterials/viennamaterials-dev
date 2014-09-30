#ifndef VIENNAMATERIALS_FORWARDS_H
#define VIENNAMATERIALS_FORWARDS_H

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

// ViennaMaterials includes
//
#include "viennamaterials/utils/shared_ptr.hpp"

namespace viennamaterials {

typedef double                                                numeric;

class backend;
class proxy;
class attribute_entity;
class function_backend;

typedef viennamaterials::shared_ptr<viennamaterials::backend>           backend_handle;
typedef viennamaterials::shared_ptr<viennamaterials::proxy>             proxy_handle;
typedef viennamaterials::shared_ptr<viennamaterials::attribute_entity>  attribute_handle;
typedef viennamaterials::shared_ptr<function_backend>                   function_backend_handle;


} // viennamaterials

#endif
