/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef LIBRARYXPATH_H_
#define LIBRARYXPATH_H_

#include "viennamaterials/library.hpp"
#include "viennamaterials/forwards.h"
#include "viennamaterials/xmldatatypes.h"

namespace viennamaterials
{

/**
 * @brief Specialized library class utilizing XPath query language to access an XML backend
 */
class library_xpath : public library_interface
{

public:
  /**
   * @brief Initiates a XPATH library object by loading a XPATH material backend object
   * @param filename A string holding the filename of the input file for the backend
   */
  library_xpath(std::string const& filename);

  /**
   * @brief Loads all relevant data from xml backend and returns a attribute entity handle representing the attribute pointed by the query.
   * @param xpath_query_to_attribute The XPath query given as string pointing to a attribute XML element
   * @return A smartpointer pointing to an attribute_entity object
   */
  attribute_handle query(std::string const& xpath_query_to_attribute);

  void dump(std::ostream& stream);

private:
  /**
   * @brief Evaluates the type of the attribute pointed by the query
   * @param xpath_query_to_attribute The XPath query given as string pointing to a attribute XML element
   * @return The enum xml_attribute_type representation of the attribute
   */
  xml_attribute_type get_attribute_type(std::string const& xpath_query_to_attribute);
};

} /* namespace viennamaterials */
#endif /* LIBRARYXPATH_H_ */
