/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

// ViennaMaterials includes
//
#include "viennamaterials/utils/string.hpp"

#ifdef NDEBUG
  #undef NDEBUG
#endif
#include <assert.h>

int main()
{
  /** String 'trim' tests **/
  std::string test1 = " test ";
  assert(viennamaterials::ltrim(test1) == "test ");

  std::string test2 = " test ";
  assert(viennamaterials::rtrim(test2) == " test");

  std::string test3 = " test ";
  assert(viennamaterials::trim(test3) == "test");

  /** String 'split' tests **/
  std::string test4 = "some/test/for/splitting";
  std::vector<std::string> result;
  viennamaterials::split(result, test4, '/');
  assert(result[0] == "some");
  assert(result[1] == "test");
  assert(result[2] == "for");
  assert(result[3] == "splitting");
  assert(result.size() == 4);

  /** String 'replace' tests **/
  viennamaterials::replace_first(test4, "/", "?");
  assert(test4 == "some?test/for/splitting");
}
