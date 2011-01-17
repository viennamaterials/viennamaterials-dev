/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               Markus Bina                        bina@iue.tuwien.ac.at
               Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    see file LICENSE in the ViennaMaterials base directory
============================================================================= */


#ifndef VIENNAMATERIALS_HPP
#define VIENNAMATERIALS_HPP


//
// *** system
//
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
//
// *** external/boost
//
#include "boost/shared_ptr.hpp"
//
// *** external/pugixml
//
#include "pugixml/src/pugixml.hpp"
//
// *** local
//
//#include "configuration.hpp"

#define XML_FILE_DEFAULT_INDENT "  "

namespace viennamaterials
{

class XmlQueryException : public std::runtime_error 
{
   public:
   XmlQueryException(std::string const & str) : std::runtime_error(str) {}
};

class InvalidXmlFileException : public std::runtime_error 
{
   public:
   InvalidXmlFileException(std::string const & str) : std::runtime_error(str) {}
};

class Library
{
protected:
// pugi::xml_document is not copyable,
// therefore, it is wrapped in a pointer.
typedef boost::shared_ptr<pugi::xml_document>   xml_doc_type;

xml_doc_type xml;
std::string filename;
std::string indent_string;

public:

Library() : indent_string(XML_FILE_DEFAULT_INDENT) {
  xml = xml_doc_type( new pugi::xml_document() );
}

Library(const std::string & filename, const std::string indent = XML_FILE_DEFAULT_INDENT) : indent_string(indent)
{
    xml = xml_doc_type( new pugi::xml_document() );
    this->filename = filename;
    this->read();
}

~Library() {  }

void setFilename(const std::string & filename)
{
  this->filename = filename;
}

void read()
{
  pugi::xml_parse_result result = this->xml->load_file(this->filename.c_str());
//  if (result != pugi::status_ok)
//  {
//    throw InvalidXmlFileException(result.description());
//  }
}

void read(const std::string & filename)
{
  this->setFilename(filename);
  this->read();
}

void write()
{
  std::ofstream stream(this->filename.c_str());
  xml->save(stream, this->indent_string.c_str());
}

void dump(std::ostream & stream = std::cout)
{
    xml->save(stream, this->indent_string.c_str());
}


std::string query(std::string xpath_query_str)
{
    std::stringstream result_stream;
    try
    {
      pugi::xpath_node_set query_result = xml->select_nodes(xpath_query_str.c_str());
      for(size_t i = 0; i < query_result.size(); i++)
      {
        query_result[i].node().print(result_stream, this->indent_string.c_str());
      }
    }
    catch (const pugi::xpath_exception& e)
    {
      std::cerr << "Exception caught in XmlReader::query ->  " << e.what() << std::endl;
      throw XmlQueryException("Exception in XmlReader::query ->  " + std::string(e.what()));
    }
    return result_stream.str();
}

};
} //namespace viennamaterials  

#endif
