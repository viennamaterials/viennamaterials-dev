#ifndef VIENNAMATERIALS_LIBRARY_HPP
#define VIENNAMATERIALS_LIBRARY_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/forwards.h"
//#include "viennamaterials/quantity.hpp"


namespace viennamaterials {


/**
    @brief Class acts as a base class for dynamic utilization
*/
class library
{

public:

  /** @brief The constructor sets the default placeholder used for processing the accessors */
  virtual ~library() {}

  /** @brief Reads an input material file and polpulates the internal database */
  virtual bool read(std::string const& filename) = 0;

  /** @brief Reads an input material stream and polpulates the internal database */
  virtual bool read(std::stringstream & stream) = 0;

  /** @brief Writes the material database to the output string stream */
  virtual bool write(std::stringstream& stream) = 0;

  /** @brief Writes the material database to the output stream */
  virtual bool write(std::ofstream& ostream) = 0;

  /** @brief Writes the material database to an XML file */
  virtual bool write(std::string const& filename) = 0;

  /** @brief Writes the content of the database to the stream */
  virtual void dump(std::ostream& stream = std::cout) = 0;

  /** @brief Checks whether the query path is available in the database */
  virtual bool has_entry(std::string const& native_query) = 0;

  /** @brief Perform a native query without conversion */
  virtual std::string               query (std::string const& native_query) = 0;

  /** @brief Access the numeric value pointed to by the native query path */
  virtual viennamaterials::numeric  query_value  (std::string const& native_query)                = 0;

};


} // viennamaterials

#endif
