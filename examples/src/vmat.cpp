/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/library.hpp"

int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    std::cout << "Error - usage: " << argv[0] << "material_file.{ipd,xml}" << std::endl;
    return -1;
  }


  typedef vmat::Library   MaterialLibrary;
  typedef vmat::Entries   Entries;
  typedef vmat::Entry     Entry;  
  MaterialLibrary   matlib;
  matlib.load(argv[1]);

  std::cout << " -------------------------------- " << std::endl;
  std::cout << "quick way to dump the whole db to an ostream .. " << std::endl;
  std::cout << " ----" << std::endl;  
  matlib.dump(std::cout);

  // we also have direct xpath access -> if a convenience function is not available 
  // for our needs ..

  std::cout << " -------------------------------- " << std::endl;
  std::cout << "perform direct queries without the convenience functions .. " << std::endl;
  std::cout << " ----" << std::endl;  
  Entries all_materials = matlib.query("/materials/material[id=\"Si\"]/category");
  vmat::printToStream(all_materials);

  // now, let's use the convenience functions with precompiled queries ..

  std::cout << "-------------------------------- " << std::endl;
  std::cout << "list all semiconductors .. " << std::endl;
  std::cout << " ----" << std::endl;
  Entries semiconductors = matlib.getSemiconductors();
  vmat::printToStream(semiconductors);

  std::cout << "-------------------------------- " << std::endl;
  std::cout << "list all metals .. " << std::endl;
  std::cout << " ----" << std::endl;
  Entries metals = matlib.getMetals();
  vmat::printToStream(metals);

  std::cout << "-------------------------------- " << std::endl;
  std::cout << "list all oxides .. " << std::endl;
  std::cout << " ----" << std::endl;
  Entries oxides = matlib.getOxides();
  vmat::printToStream(oxides);

  std::cout << "-------------------------------- " << std::endl;
  std::cout << "list all materials of customer category .. " << std::endl;
  std::cout << " ----" << std::endl;
  Entries uber = matlib.getMaterialsOfCategory("uber");
  vmat::printToStream(uber);

  std::cout << "-------------------------------- " << std::endl;
  std::cout << "query a specific material .. " << std::endl;
  std::cout << " ----" << std::endl;
  Entries Si = matlib.getMaterial("Si");
  vmat::printToStream(Si);
  
  std::cout << "-------------------------------- " << std::endl;
  std::cout << "quick test if a material is available .. " << std::endl;
  std::cout << " ----" << std::endl;
  std::cout << "db has 'Si'? (should have)      : " << matlib.hasMaterial("Si") << std::endl;
  std::cout << "db has 'X0d'? (should not have) : " << matlib.hasMaterial("X0d") << std::endl;  

  std::cout << " -------------------------------- " << std::endl;
  std::cout << "query a parameter from a given material .. " << std::endl;
  std::cout << " ----" << std::endl;  
  Entries Si_category = matlib.getParameter(Si, "bandgap");
  vmat::printToStream(Si_category);


}

