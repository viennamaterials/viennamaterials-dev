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

// System includes
//
#include <iostream>
#include <vector>

// ViennaMaterials includes
//
#include "viennamaterials/forwards.h"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/check.hpp"
#include "viennamaterials/make_query.hpp"
#include "viennamaterials/base_accessor.hpp"
#include "viennamaterials/utils/file_extension.hpp"
#include "viennamaterials/utils/convert.hpp"

// Boost includes
//
#include "boost/algorithm/string/replace.hpp"

namespace viennamaterials {


/**
    @brief Class acts as a base class for dynamic utilization
*/
class library
{
private:
  typedef std::vector<base_accessor*>      accessor_container_type;

public:

  /** @brief The constructor sets the default placeholder used for processing the accessors */
  library() : placeholder_("%") {}
  virtual ~library() {}

  /** @brief Allows to register query accessors, returns unique id required for query entries */
  accessor_handle register_accessor(base_accessor* accessor)
  {
    accessors_.push_back(accessor);
    return accessors_.size()-1;
  }

  /** @brief Allows to externally set the placeholder in the accessor string */
  string& placeholder() { return placeholder_; }

  accessor_container_type& accessors()                     { return accessors_; }

  base_accessor&           get_accessor(std::size_t index) { return *accessors_[index]; }


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
  virtual bool has_entry(viennamaterials::query const& query) = 0;

  /** @brief Perform a generic query regardless of the backend, returns a string object holding the result */
  virtual viennamaterials::string   query  (viennamaterials::query const& query)                      = 0;

  /** @brief Perform a generic query regardless of the backend, performs automatic conversion to numeric type */
  virtual viennamaterials::numeric  query_value  (viennamaterials::query const& query)                = 0;

  /** @brief Perform a native query, i.e., the method expects a backend-specifc encoded path string */
  virtual viennamaterials::string   query_native (viennamaterials::string const& native_query_string) = 0;

private:
  accessor_container_type accessors_;
  string                  placeholder_;
};


} // viennamaterials

#endif
