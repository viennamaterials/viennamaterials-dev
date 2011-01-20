/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               Markus Bina                        bina@iue.tuwien.ac.at
               Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    see file LICENSE in the ViennaMaterials base directory
============================================================================= */


#ifndef VIENNAMATERIALS_LIBRARY_HPP
#define VIENNAMATERIALS_LIBRARY_HPP

//
// *** local
//
//#include "ViennaUtils/xml.hpp"
#include "ViennaUtils/settings.hpp"

namespace viennamaterials { 

//namespace tag {
//typedef viennatutils::tag
//}

template<typename Tag>
struct library 
{
   typedef typename viennautils::settings<Tag>::type  type;
};

} //namespace viennamaterials  

#endif


