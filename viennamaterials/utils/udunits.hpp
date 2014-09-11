#ifndef VIENNAMATERIALS_UNITS_UDUNITS_HPP
#define VIENNAMATERIALS_UNITS_UDUNITS_HPP

/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

/** @file viennamaterials/units/udunits.hpp
    @brief Interface class for the udunits library
*/

#ifdef VIENNAMATERIALS_WITH_UNITS

extern "C"{
#include "udunits2.h"
}

#include <stdexcept>
#include "viennamaterials/forwards.h"
#include "viennamaterials/quantity.hpp"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/xmldatatypes.h"
#include <typeinfo>

namespace viennamaterials {

/** @brief An exception class for the udunits class
  */
class udunits_exception : public std::runtime_error {
public:
  udunits_exception(std::string const & str) : std::runtime_error(str) {}
};

/** @brief The udunits class provides an interface to the udunits library
  */
struct udunits
{
public:

  /** @brief The constructor expects a filename pointing to a udunits-valid XML file containing the unit system
  *          and forwards it to the udunits' backend by instantiating a 'unit_system' object
  *
  * @param filename   The filename to be loaded
  */
  udunits(std::string const& filename);

  /** @brief The destructor releases the 'unit_system' object
  *
  */
  ~udunits();

  /** @brief The function scales a numeric value from a source unit to a new unit, e.g., 1 mV -> 0.001 V
  *
  * @param value       The single numerical value (i.e. double) to be converted
  * @param source_unit The original unit associated with the value
  * @param target_unit The target unit to which the value has to be converted to
  */
  void convert(viennamaterials::numeric& value, std::string const& source_unit, std::string const& target_unit);

  /** @brief The function scales a quantity from a source unit to a new unit, e.g., 1 mV -> 0.001 V
  *
  * @param quan        The quantity to be converted
  * @param target_unit The target unit to which the value has to be converted to
  */
  template<typename T>
  viennamaterials::quantity<T> convert(viennamaterials::quantity<T> const& quan, std::string const& target_unit)
  {
    T val = quan.value();
    this->convert(val, quan.unit(), target_unit);
    return viennamaterials::quantity<T>(val, target_unit);
  }

private:

  /** @brief An auxiliary function generating a udunit value converter
  *
  * @param source_unit The unit of the input value
  * @param target_unit The unit of the target value
  */
  cv_converter* get_converter(std::string const& source_unit, std::string const& target_unit);

  /** @brief An auxiliary function evaluating the string-based unit parsing error code
  *          returned by udunits' 'ut_parse' method
  *
  * @param unit The unit string which has been used as a parser input
  */
  void investigate_parse_error(std::string const& unit);

  /** @brief An auxiliary function evaluating the conversion error raised by
  *          udunits' conversion method
  *
  * @param source_unit The unit of the input value
  * @param target_unit The unit of the target value
  */
  void investigate_conversion_error(std::string const& source_unit, std::string const& target_unit);

private:

  /** @brief A pointer to the udunits' unit system facility.
  *
  */
  ut_system*          unit_system_;
};


/// Generic implementation for not supported datatypes
//template<typename T>
//void udunits::convert(viennamaterials::quantity<T>& quan, std::string const& target_unit)
//{
//  std::string type = typeid(quan.value()).name();
//  throw udunits_error("Datatype (" + type + ") not supported for conversion");
//}

} // viennamaterials

#endif
#endif
