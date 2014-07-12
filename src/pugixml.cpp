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
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/exceptions.hpp"
#include "viennamaterials/utils/convert.hpp"
#include "viennamaterials/utils/file_extension.hpp"
#include "viennamaterials/utils/string.hpp"

namespace viennamaterials {

// Public members

pugixml::pugixml() : library()
{
  this->init();
}

pugixml::pugixml(std::string const& filename)
{
  this->init();
  this->read(filename);
}

pugixml::~pugixml()
{
}

void pugixml::init()
{
  indent_string_ = "  ";
}

bool pugixml::read(std::string const& filename)
{
  if(viennamaterials::file_extension(filename) != "xml")
  {
    throw no_xml_file_error();
    return false;
  }

  xml_.reset();
#ifdef VIENNAMATERIALS_HAS_LIBXML2
  if(!viennamaterials::check(filename))
  {
    return false;
  }
#endif

  if(!viennamaterials::file_exists(filename))
  {
    throw xml_file_load_error();
    return false;
  }

  pugi::xml_parse_result result = xml_.load_file(filename.c_str());
  if(!result)
    throw viennamaterials::xml_parse_error(std::string(result.description()));

  return true;
}

bool pugixml::read(std::stringstream & stream)
{
  xml_.reset();

  pugi::xml_parse_result result = xml_.load(stream);
  if(!result)
    throw viennamaterials::xml_parse_error(std::string(result.description()));

  return true;
}

bool pugixml::write(std::stringstream& stream)
{
  dump(stream);
  return true;
}

bool pugixml::write(std::ofstream& ostream)
{
  dump(ostream);
  return true;
}

bool pugixml::write(std::string const& filename)
{
  std::ofstream stream(filename.c_str());
  dump(stream);
  stream.close();
  return true;
}

void pugixml::dump(std::ostream& stream)
{
  xml_.save(stream, indent_string_.c_str());
}

bool pugixml::has_entry(std::string const& native_query)
{
  return ( this->query(native_query) != "" );
}

std::string pugixml::query(std::string const& native_query)
{
  std::stringstream result_stream;
  try
  {
     pugi::xpath_node_set query_result = xml_.select_nodes(native_query.c_str());
     for(size_t i = 0; i < query_result.size(); i++)
     {
       query_result[i].node().print(result_stream, indent_string_.c_str());
     }
  }
  catch (const pugi::xpath_exception& e)
  {
     std::cerr << "PugiXML query exception: " << e.what() << std::endl;
  }
  std::string result = result_stream.str();
  viennamaterials::trim(result);  // remove front/trailing whitespaces
  return result;
}

viennamaterials::numeric pugixml::query_value(std::string const& native_query)
{
  return viennamaterials::convert<viennamaterials::numeric>(this->query(native_query));
}

double pugixml::query_xpath_number(std::string const& native_query)
{
  pugi::xpath_query query(native_query.c_str());
  return query.evaluate_number(xml_);
}

std::string pugixml::query_xpath_string(std::string const& native_query)
{
  pugi::xpath_query query(native_query.c_str());
  return query.evaluate_string(xml_); //TODO: check pugi::string_t
}

} // viennamaterials
