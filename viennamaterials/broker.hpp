/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef BROKER_HPP_
#define BROKER_HPP_

#include <string>
#include "viennamaterials/platform.hpp"
#include "viennamaterials/attributeentity.hpp"
#include "viennamaterials/xmldatatypes.h"

namespace viennamaterials
{

class broker
{
public:
  /**
   * @brief Initiates a broker object by loading a material library object
   * @param filename A string holding the filename of the material library XML file
   */
  broker(std::string const& filename);

  /**
   * @brief Loads all relevant data from xml backend and returns a attribute entity handle representing the attribute pointed by the query.
   * @param xpath_query_to_attribute The XPath query given as string pointing to a attribute XML element
   * @return A smartpointer pointing to an attribute_entity object
   */
  attribute_handle query(std::string const& xpath_query_to_attribute);

private:
  library_handle lib_;

private:
  /**
   * @brief Evaluates the type of the attribute pointed by the query
   * @param xpath_query_to_attribute The XPath query given as string pointing to a attribute XML element
   * @return The enum xml_attribute_type representation of the attribute
   */
  xml_attribute_type get_attribute_type(std::string const& xpath_query_to_attribute);
};

} /* namespace viennamaterials */
#endif /* BROKER_HPP_ */
