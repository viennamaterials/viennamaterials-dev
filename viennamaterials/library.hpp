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

#include "viennautils/xml.hpp"
#include "viennautils/file.hpp"

extern "C" {
#include "ipd.h"
}
 
namespace vmat { 


struct library 
{
  typedef viennautils::xml<viennautils::tag::pugixml>::type MaterialDatabase;

  bool load(std::string const& filename)
  {
    if(!viennautils::file_exists(filename)) return false;
    
    if(viennautils::file_extension(filename) == "xml")  // native
    {
      mdb.read(filename);
      //mdb.dump();
      return true;
    }
    else if(viennautils::file_extension(filename) == "ipd")  // conversion required
    {
      ipdInit(NULL, NULL);
      
      // todo: 
      
      return true;
    }
    else return false;
  }
  
  


  MaterialDatabase mdb;
};

} //namespace viennamaterials  

#endif


