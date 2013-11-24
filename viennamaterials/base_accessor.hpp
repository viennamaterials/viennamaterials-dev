#ifndef VIENNAMATERIALS_BASEACCESSOR_HPP
#define VIENNAMATERIALS_BASEACCESSOR_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

namespace viennamaterials {

struct base_accessor
{
public:
  typedef viennamaterials::string result_type;
  virtual result_type operator()() = 0;
};


} // viennamaterials

#endif

