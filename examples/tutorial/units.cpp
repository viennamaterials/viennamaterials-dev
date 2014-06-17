/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/utils/udunits.hpp"
#include "viennamaterials/quantity.hpp"

int main(int argc, char * argv[])
{
  if(argc != 2)
  {
    std::cerr << "Error - Usage: " << argv[0] << " input-units-file.xml" << std::endl;
    std::cerr << "Example:       " << argv[0] << " ../units/units.xml" << std::endl;
    std::cerr << "Aborting .." << std::endl;
    return -1;
  }

  /** Import the unit database file and create a unit object **/
  viennamaterials::udunits   units_converter(argv[1]);

  /** Prepare an input value and an unit **/
  double value = 1.0;
  std::string source_unit = "m";
  std::cout << value << " " << source_unit << std::endl;

  /** Now you can convert the value to a new target unit **/
  std::string target_unit = "cm";
  units_converter.convert(value, source_unit, target_unit);
  std::cout << value << " " << target_unit << std::endl;

  /** You can also directly use ViennaMaterials quantity objects **/
  viennamaterials::quantity quan(1.0, "V");
  std::cout << "converting quantity: " << quan << " --> ";
  units_converter.convert(quan, "mV");
  std::cout << quan << std::endl;

  return 0;
}
