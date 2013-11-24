#ifndef VIENNAMATERIALS_FORWARDS_H
#define VIENNAMATERIALS_FORWARDS_H

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include <iostream>
#include <vector>
#include <utility>

namespace viennamaterials {

class library;

typedef double                                    numeric;
typedef std::string                               string;
typedef std::size_t                               accessor_handle;
typedef std::pair<accessor_handle, string>        entry;
typedef std::vector<entry>                        query;

} // viennamaterials


#endif

