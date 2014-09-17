/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */


#include <iostream>
#include "tools/ipd2xml/ViennaIPD/src/include/ipd.h"

int main(void)
{
  std::cout << "hello world" << std::endl;

  // Initialize the Database.
  // Normally we can use the default values, so we don't need parameters.
  ipdInit(NULL, NULL);

  // Let's create a database
  ipdCreateBase("NameOfTheDatabase", 0);

  // Read a file in the database
  ipdConstString  filename;
  filename = "examples/data/input1.ipd"; ////FIXME: hardcoded for testing
  ipdReadInputDeck(filename);

  // Get the value of 'a' in the root section
  ipdChar        *s;
  ipdGetStringByName("~a", &s);
  printf("Value of ~a: %s\n", s);

  // Get the value of 'b' in the root section
  ipdDouble       d;
  ipdGetRealByName("~b", &d);
  printf("Value of ~b: %g\n", d);


  // Free the ViennaIPD datastructures
  ipdFreeAll();

  return 0;
}

