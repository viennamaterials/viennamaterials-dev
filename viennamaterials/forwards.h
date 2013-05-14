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

#include "viennautils/xml.hpp"
#include "viennautils/file.hpp"

extern "C" {
#include "ipd.h"
}


namespace vmat {


typedef viennautils::xml<viennautils::tag::pugixml>::type           MaterialDatabase;
typedef MaterialDatabase::NodeSet                                   Entries;
typedef MaterialDatabase::Node                                      Entry;
  

namespace key {

  static const std::string id             = "id";
  static const std::string category       = "category";
  static const std::string parameter      = "parameter";  
  
  static const std::string semiconductor  = "semiconductor";
  static const std::string oxide          = "oxide";
  static const std::string metal          = "metal";    
}



} // end vmat


#endif

