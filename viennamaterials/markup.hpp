#ifndef VIENNAMATERIALS_LAYOUT_H
#define VIENNAMATERIALS_LAYOUT_H

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

struct markup
{
  virtual void operator()(query const& q) = 0;
};

} // viennamaterials

#endif
