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

#include "boost/shared_ptr.hpp"

namespace viennamaterials {

class library;

typedef double                                    numeric;
typedef std::string                               string;
typedef std::size_t                               accessor_handle;
typedef std::pair<accessor_handle, string>        entry;
typedef std::vector<entry>                        query;

typedef boost::shared_ptr<viennamaterials::library> library_handle;

void write_query(viennamaterials::library* matlib, query& some_query, std::ostream& stream = std::cout);
void write_query(library_handle& matlib, query& some_query, std::ostream& stream = std::cout);

viennamaterials::string generate_query_string(viennamaterials::library* matlib,                    query const& some_query);
viennamaterials::string generate_query_string(library_handle& matlib, query const& some_query);

} // viennamaterials

#endif

