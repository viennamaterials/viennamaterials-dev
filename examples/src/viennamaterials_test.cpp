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

#include "ViennaMaterials/vmat.hpp"

int main(int argc, char * argv[])
{
   if(argc != 2)
   {
      std::cerr << "Wrong number of parameters: example ./" << argv[0] << " input.xml" << std::endl;
      return -1;
   }

   // create an instance of the material library
   //
   viennamaterials::Library   matlib;
   
   // load a specific xml input file
   //
   matlib.read(argv[1]);
   std::cout << "-----------------------------------------------" << std::endl;

   // dump the whole xml tree
   //
   matlib.dump();
   std::cout << "-----------------------------------------------" << std::endl;

   // execute a query and print the result to std::cout
   //
   std::cout << matlib.query("/Materials/element[identifier='Si']/data[identifier='mass']/representation/double");
   std::cout << "-----------------------------------------------" << std::endl;

   return 0;
}
