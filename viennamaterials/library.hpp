/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef LIBRARY_HPP_
#define LIBRARY_HPP_

#include <string>
//#include "viennamaterials/platform.hpp"
#include "viennamaterials/forwards.h"
#include "viennamaterials/attributeentity.hpp"

namespace viennamaterials
{

/**
 * @brief Interface class for specialized library classes
 */
class library_interface
{
public:
  virtual ~library_interface() {};

  /**
   * @brief Loads all relevant data from backend and returns a attribute entity handle representing the attribute pointed by the query.
   * @param query_to_attribute The query given as string pointing to a attribute element
   * @return A smartpointer pointing to an attribute_entity object
   */
  virtual attribute_handle query(std::string const& query_to_attribute) = 0;

protected:
  backend_handle backend_;
};

/**
 * @brief Class library provides a high level query function to access material data
 */
class library
{
public:
  /**
   * @brief Initiates a library object by loading a material frontend. Frontend is chosen by filename extension.
   * @param filename A string referencing the input file
   */
  library(std::string const& filename);

  /**
   * @brief Loads all relevant data from backend and returns a attribute entity handle representing the attribute pointed by the query.
   * @param query_to_attribute The query given as string pointing to a attribute element
   * @return A smartpointer pointing to an attribute_entity object
   */
  attribute_handle query(std::string const& query_to_attribute);

private:
  shared_ptr<library_interface> frontend_;
};

} /* namespace viennamaterials */
#endif /* LIBRARY_HPP_ */
