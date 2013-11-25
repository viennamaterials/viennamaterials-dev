#ifndef VIENNAMATERIALS_MAKEQUERY_HPP
#define VIENNAMATERIALS_MAKEQUERY_HPP

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

#include "viennamaterials/forwards.h"

namespace viennamaterials { 

inline entry make_entry(accessor_handle handle, string const& value)
{
  return std::make_pair(handle, value);
}

namespace detail {
  template <typename T, size_t N>
  T* begin(T(&arr)[N]) { return &arr[0]; }

  template <typename T, size_t N>
  T* end(T(&arr)[N]) { return &arr[0]+N; }
} // detail

inline viennamaterials::query make_query(viennamaterials::entry const& l0)
{
  viennamaterials::entry array[] = { l0 };
  std::vector<viennamaterials::entry> query_vector(detail::begin(array), detail::end(array));
  return query_vector;
}

inline viennamaterials::query make_query(viennamaterials::entry const& l0, viennamaterials::entry const& l1)
{
  viennamaterials::entry array[] = { l0, l1 };
  std::vector<viennamaterials::entry> query_vector(detail::begin(array), detail::end(array));
  return query_vector;
}

inline viennamaterials::query make_query(viennamaterials::entry const& l0, viennamaterials::entry const& l1, 
                                         viennamaterials::entry const& l2)
{
  viennamaterials::entry array[] = { l0, l1, l2 };
  std::vector<viennamaterials::entry> query_vector(detail::begin(array), detail::end(array));
  return query_vector;
}

inline viennamaterials::query make_query(viennamaterials::entry const& l0, viennamaterials::entry const& l1, 
                                         viennamaterials::entry const& l2, viennamaterials::entry const& l3)
{
  viennamaterials::entry array[] = { l0, l1, l2, l3 };
  std::vector<viennamaterials::entry> query_vector(detail::begin(array), detail::end(array));
  return query_vector;
}

inline viennamaterials::query make_query(viennamaterials::entry const& l0, viennamaterials::entry const& l1, 
                                         viennamaterials::entry const& l2, viennamaterials::entry const& l3,
                                         viennamaterials::entry const& l4)
{
  viennamaterials::entry array[] = { l0, l1, l2, l3, l4 };
  std::vector<viennamaterials::entry> query_vector(detail::begin(array), detail::end(array));
  return query_vector;
}

inline viennamaterials::query make_query(viennamaterials::entry const& l0, viennamaterials::entry const& l1, 
                                         viennamaterials::entry const& l2, viennamaterials::entry const& l3,
                                         viennamaterials::entry const& l4, viennamaterials::entry const& l5)
{
  viennamaterials::entry array[] = { l0, l1, l2, l3, l4, l5 };
  std::vector<viennamaterials::entry> query_vector(detail::begin(array), detail::end(array));
  return query_vector;
}


} // viennamaterials

#endif

