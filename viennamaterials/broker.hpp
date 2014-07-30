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
  broker(std::string const& filename);
//  void              load_file(std::string const& filename);
  attribute_entity* query(std::string const& xpath_query_to_attribute);

private:
  library_handle lib_;

private:
  xml_attribute_type get_attribute_type(std::string const& xpath_query_to_attribute);
};

} /* namespace viennamaterials */
#endif /* BROKER_HPP_ */
