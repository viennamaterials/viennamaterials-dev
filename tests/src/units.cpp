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

#ifdef NDEBUG
  #undef NDEBUG
#endif
#include <assert.h>

int main(int argc, char * argv[])
{
  /** Import the unit database file and create a unit object **/
  viennamaterials::udunits   units_converter("../../units/units.xml");

  /** Prepare an input value and an unit **/
  double value = 1.0;
  std::string source_unit = "m";

  /** Now you can convert the value to a new target unit **/
  std::string target_unit = "cm";
  units_converter.convert(value, source_unit, target_unit);
  assert(value == 100);

  /** You can also directly use ViennaMaterials quantity objects **/
  viennamaterials::quantity<double> quan(1.0, "V");
  std::cout << "converting quantity: " << quan << " --> ";
  units_converter.convert(quan, "mV");
  assert(quan.value() == 1000);
  assert(quan.unit() == "mV");

  return 0;
}
