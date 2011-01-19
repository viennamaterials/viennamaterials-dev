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
#include "ViennaUtils/xml.hpp"

namespace viennamaterials { 

//
// settings: default case 
//
template<typename Tag>
struct library 
{
   typedef viennautils::XML   type;
};

//
// settings: xml based
//
template<>
struct library <viennautils::tag::xml>
{
   typedef viennautils::XML   type;
};


} //namespace viennamaterials  

#endif


