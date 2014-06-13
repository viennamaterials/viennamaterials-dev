#ifndef VIENNAMATERIALS_PROXY_VIENNASTAR_HPP
#define VIENNAMATERIALS_PROXY_VIENNASTAR_HPP

/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/proxy.hpp"

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/replace.hpp"

namespace viennamaterials
{

struct viennastar_proxy : public viennamaterials::proxy
{
private:
  typedef std::vector<std::string>    QueryPartsType;

public:
  viennastar_proxy(viennamaterials::library_handle& matlib) :
    viennamaterials::proxy(matlib),
    placeholder_    ("%"),
    token_          ("/"),
    sub_path_       ("/*[id=\""+placeholder_+"\"]"),
    value_path_     ("/value/text()"),
    unit_path_      ("/unit/text()")
  {
  }

  std::string query          (std::string const& q)
  {
    std::string base_path;
    generate_base_path(q, base_path);
    return matlib()->query(base_path);
  }

  std::string query_unit     (std::string const& q)
  {
    std::string base_path;
    generate_base_path(q, base_path);
    base_path += unit_path_;
    return matlib()->query(base_path);
  }

  numeric     query_value    (std::string const& q)
  {
    std::string base_path;
    generate_base_path(q, base_path);
    base_path += value_path_;
    return matlib()->query_value(base_path);
  }

  quantity    query_quantity (std::string const& q)
  {
    return quantity(query_value(q), query_unit(q));
  }

private:

  void generate_base_path(std::string const& q, std::string & base_path)
  {
    query_parts_.clear();
    boost::algorithm::split(query_parts_, q, boost::algorithm::is_any_of(token_));
    for(QueryPartsType::const_iterator iter = query_parts_.begin();
        iter != query_parts_.end(); iter++)
    {
      std::string temp_path = sub_path_;
      boost::algorithm::replace_first(temp_path, placeholder_, *iter);
      base_path += temp_path;
    }
  }

  const std::string placeholder_;
  const std::string token_;
  const std::string sub_path_;
  const std::string value_path_;
  const std::string unit_path_;

  QueryPartsType query_parts_;
};


} // viennamaterials

#endif
