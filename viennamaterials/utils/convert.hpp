#ifndef VIENNAMATERIALS_UTILS_CONVERT_HPP
#define VIENNAMATERIALS_UTILS_CONVERT_HPP

/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// System includes
//
#include <sstream>
#include <string>

namespace viennamaterials {

template<typename TargetT, typename SourceT>
TargetT convert(SourceT const& source)
{
  TargetT target;
  std::stringstream sstr;
  sstr << source;
  sstr >> target;
  return target;
}

} // end namespace viennamaterials

#endif
