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

#include "viennamaterials/library.hpp"

int main(int argc, char * argv[])
{
   // setup an xml based material library
   //
   typedef viennamaterials::library<viennautils::tag::xml>::type   MatlibT;
   MatlibT   matlib;
   
   // load a specific xml input file
   //
   matlib.read("../tests/test.xml");

   // dump the whole datastructure
   //
   matlib.dump();

   // execute a query and print the result to std::cout
   //
   std::string result = matlib.query("/Materials/element[identifier='Si']/data[identifier='mass']/representation/double");
   if(result != "<double> 4.510 </double>")
   {
      std::cerr << "Error: query returned wrong result" << std::endl;
      return -1;
   }

   return 0;
}
