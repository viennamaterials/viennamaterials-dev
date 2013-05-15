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
#include "viennamaterials/kernels/pugixml.hpp"

int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    std::cout << "Error - usage: " << argv[0] << "material_file.xml" << std::endl;
    return -1;
  }

  typedef vmat::Library<vmat::tag::pugixml>::type  Library;
  typedef Library::Entries          Entries;
  typedef Library::Entry            Entry;
  typedef Library::EntryIterator    EntryIterator;
  typedef Library::String           String;
  typedef Library::Numeric          Numeric;
  
  Library   matlib;
  matlib.load(argv[1]);
  
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "quick way to dump the whole db to a stream .. " << std::endl;
  std::cout << " ----" << std::endl;  
  matlib.dump(std::cout);

  // we have direct xpath access -> if a convenience function is not available 
  // for our needs .. note: the value has to be put in apostrophies, e.g., 'Si'
  // --> this will be fun for ViennaMOS: the "Material Database" UI can provide
  // an input query field, where the user can run queries against the database
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "perform direct queries without the convenience functions .. " << std::endl;
  std::cout << " ----" << std::endl;  
  Entries all_materials = matlib.query("/materials/material[id='Si']/category");
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
  std::cout << "list all materials of an arbitrary category .. " << std::endl;
  std::cout << " ----" << std::endl;
  Entries indirect = matlib.getMaterialsOfCategory("indirect");
  vmat::printToStream(indirect);

  // materials can have several categories .. we can use XPath to query 
  // for all materials which satisfy a specific categoryset
  // use 'and/or' in the expression
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "checking" << std::endl;
  std::cout << " ----" << std::endl;  
  Entries subset = matlib.query("/materials/material[category='indirect' or category='metal']");
  vmat::printToStream(subset);
  
  std::cout << "-------------------------------- " << std::endl;
  std::cout << "query a specific material .. " << std::endl;
  std::cout << " ----" << std::endl;
  Entry Si = matlib.getMaterial("Si");
  vmat::printToStream(Si);
  
  std::cout << "-------------------------------- " << std::endl;
  std::cout << "quick test if a material is available .. " << std::endl;
  std::cout << " ----" << std::endl;
  std::cout << "db has 'Si'? (should be)      : " << matlib.hasMaterial("Si") << std::endl;
  std::cout << "db has 'X0d'? (should not be) : " << matlib.hasMaterial("X0d") << std::endl;  

  std::cout << " -------------------------------- " << std::endl;
  std::cout << "query a parameter for a specific material on the fly.. " << std::endl;
  std::cout << " ----" << std::endl;  
  Entry Ge_bandgap = matlib.getParameter("Ge", "bandgap");
  vmat::printToStream(Ge_bandgap);

  // we can work with extracted materials without using the 'matlib' object
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "query a parameter from a previously extracted material .. " << std::endl; 
  std::cout << " ----" << std::endl;
  // for repetitive parameter queries, use the parameterextractor object
  // (this object precompiles the queries in advance and holds them in it's state)
  vmat::ParameterExtractor paraext; 
  Entry Si_bandgap = paraext(Si, "bandgap"); 
  vmat::printToStream(Si_bandgap);
  std::cout << "Parameter is available? (should be) :" << paraext.hasParameter(Si, "bandgap") << std::endl;
  // for a single-shot access, use this free function
  Entry Si_bandgap2 = vmat::getParameter(Si, "bandgap"); 
  vmat::printToStream(Si_bandgap2);
  std::cout << "Parameter is available? (should be) :" << vmat::hasParameter(Si, "bandgap") << std::endl;
  
  // if queries return a set of entries, we can travese them via STL-style iterators
  // and extract the 'Entry' objects, which we can further process, e.g., extract parameters
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "traverse entries .. " << std::endl; 
  std::cout << " ----" << std::endl;  
  for(EntryIterator iter = indirect.begin(); iter != indirect.end(); iter++)
  {
    Entry entry = *iter;
    // ".//" -> XPath expression relative to current context node 'entry'
    Entries subtree = vmat::query(entry, ".//id"); 
    vmat::printToStream(subtree);
  }

  // if we have a parameter entry, we can access the data, e.g., the actual numerical value
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "access parameter values of previously extracted parameters.. " << std::endl; 
  std::cout << " ----" << std::endl;  
  
  String name = vmat::name(Si_bandgap);
  std::cout << "name: " << name << std::endl;

  Numeric value = vmat::value(Si_bandgap);
  std::cout << "value: " << value << std::endl;

  String unit = vmat::unit(Si_bandgap);
  std::cout << "unit: " << unit << std::endl;

  String note = vmat::note(Si_bandgap);
  std::cout << "note: " << note << std::endl;
  
  // if we have a parameter entry, we can access the data, e.g., the actual numerical value
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "access parameter values on the fly.. " << std::endl; 
  std::cout << " ----" << std::endl;  

  value = matlib.getParameterValue("Si", "bandgap");
  std::cout << "value: " << value << std::endl;
  
  unit = matlib.getParameterUnit("Si", "bandgap");
  std::cout << "unit: " << unit << std::endl;
  
  note = matlib.getParameterNote("Si", "bandgap");
  std::cout << "note: " << note << std::endl;

  std::cout << " -------------------------------- " << std::endl;
  std::cout << "check if an entry is a 'parameter'.. " << std::endl; 
  std::cout << " ----" << std::endl;  
  std::cout << "is parameter (should be)    : " << vmat::isParameter(Ge_bandgap) << std::endl;
  std::cout << "is parameter (should not be): " << vmat::isParameter(Si) << std::endl;
  
  std::cout << " -------------------------------- " << std::endl;
  std::cout << "check if an entry is a 'material'.. " << std::endl; 
  std::cout << " ----" << std::endl;  
  std::cout << "is material (should not be): " << vmat::isMaterial(Ge_bandgap) << std::endl;
  std::cout << "is material (should be)    : " << vmat::isMaterial(Si) << std::endl;  
}

