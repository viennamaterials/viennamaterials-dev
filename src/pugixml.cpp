/* =============================================================================
   Copyright (c) 2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/pugixml.hpp"

#include "boost/algorithm/string/trim.hpp"

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

viennamaterials::string pugixml::query(viennamaterials::query & query)
{
  viennamaterials::string native_query_string;

  for(viennamaterials::query::iterator iter = query.begin(); 
      iter != query.end(); iter++)
  {
    std::string path = get_accessor(iter->first)();
//    std::cout << "original path: " << path << std::endl;
    boost::algorithm::replace_first(path, placeholder(), iter->second);
//    std::cout << "replaced path: " << path << std::endl;
    native_query_string += path;
  }
//  std::cout << "final path: " << native_query_string << std::endl;
  return this->query_pugixml(native_query_string);
}

viennamaterials::numeric pugixml::query_value(viennamaterials::query & query)
{
  return viennamaterials::convert<viennamaterials::numeric>()(this->query(query));
}

viennamaterials::string pugixml::query_native(viennamaterials::string const& native_query_string)
{
  return this->query_pugixml(native_query_string);
}

// Private members

viennamaterials::string pugixml::query_pugixml(viennamaterials::string const& native_query_string)
{
  std::stringstream result_stream;
  try
  {
     pugi::xpath_node_set query_result = xml_.select_nodes(native_query_string.c_str());
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
  boost::trim(result);  // remove front/trailing whitespaces
  return result;
}

} // viennamaterials

