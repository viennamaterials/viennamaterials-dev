/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    matthias

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef IPD2XML_HPP_
#define IPD2XML_HPP_

#include <iostream>
#include <sstream>
#include <exception>
#include <vector>
#include "tools/ipd2xml/tinyxml/tinyxml.h"
#include "tools/ipd2xml/ViennaIPD/src/include/ipd.h"
#include "viennamaterials/xmldatatypes.h"

#define VERBOSE_MODE

#ifdef VERIFY_XML
#include "viennamaterials/forwards.h"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/proxy/viennastar.hpp"
#include <math.h>

void recurise_traverse_and_verify(ipdIterator_t * iNode, viennamaterials::proxy_handle myproxy);
std::string ipd_path_to_xml(const char* ipd_path);
void output_mismatch(const char* name);
bool almost_equal(double a, double b);
#endif


const std::string ipd_item_name("materials"); //using 'materials' IPD item as 'name' for XML material
const std::string ipd_item_category("class"); //using 'class' IPD item as 'category' for XML material

struct ipd2xml_error : public std::exception
{
  ipd2xml_error(std::string const& info) : info(info) {}
  ~ipd2xml_error() throw() {}

  virtual const char* what() const throw()
  {
    return info.c_str();
  }

  std::string info;
};


/// @brief precision for stringstream used in converter method
#define STRINGSTREAM_PRECISION 15

template < typename T >
std::string converter(T t)
{
   std::stringstream ss;
   ss.precision(STRINGSTREAM_PRECISION);
   ss << t;
   return ss.str();
}

//
// ----------------------------------------------------------
//

//TODO doxygen
class statistic_data
{
public:
  statistic_data();
  void increment_attribute_number_ipd();
  long get_number_of_attributes_ipd();
  void increment_attribute_number_xml();
  long get_number_of_attributes_xml();
  void increment_invalid_node_number();
  long get_number_of_invalid_nodes();

private:
  long number_of_attributes_ipd;
  long number_of_attributes_xml;
  long invalid_ipd_nodes;
};

//TODO doxygen
class xmlwriter
{
public:
  xmlwriter(const char* note);
  void add_element(TiXmlElement* element);
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_bool& value, const char* unit);
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_int& value, const char* unit);
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_float& value, const char* unit);
  TiXmlElement* create_tensor(const char* id, const ipdLong& dimensions, const ipdLong* dimension_length, const ipdDouble* values, const char* unit);
  TiXmlElement* create_string(const char* id, const viennamaterials::xml_string& value);
  void add_note(const char* note);
  void open_material_element(const char* id, const char* name, const char* category);
  void close_material_element(); //wrapper for update(), use with care
  void open_group_element(const char* id, const char* name, const char* category);
  void close_group_element(); //wrapper for update(), use with care
  void update();
  void print(std::string filename);
  void print(const char* filename);
  void print_to_console(void);

private:
  TiXmlElement* create_unit(const char* unit);

private:
  TiXmlDocument doc_;

  TiXmlElement * currentnode_;

  const char* root_tag_;
  const char* id_tag_;
  const char* group_tag_;
  const char* attribute_tag_;
  const char* scalar_tag_;
  const char* type_attribute_tag_;
  const char* type_boolean_;
  const char* type_integer_;
  const char* type_floating_;
  const char* tensor_tag_;
  const char* tensor_order_attribute_tag_;
  const char* tensor_dimension_attribute_tag_;
  const char* tensor_index_attribute_tag_;
  const char* unit_tag_;
  const char* note_tag_;
  const char* category_tag_;
  const char* material_tag_;
  const char* name_tag_;
  const char* string_tag_;

  std::vector< TiXmlElement * > nodecont_;
};


//TODO doxygen
class ipd_importer
{

public:
  ipd_importer(statistic_data* statistics);

  /*
   * @brief This method traverses the IPD layout to locate a materials
   * @param iNode IPD iterator pointing to the root of the IPD input file
   * @param xmldoc xmlwriter object used for XML output
   */
  void traverse_ipd_layout(ipdIterator_t * iNode, xmlwriter& xmldoc);

private:
  /*
   * @brief This method adds a new material to XML and subsequently traverses the IPD structure for attributes.
   * @param iNode An IPD iterator pointing to a material
   * @param xmldoc xmlwriter object used for XML output
   */
  void access_ipd_material(ipdIterator_t * iNode, xmlwriter& xmldoc);

  /*
   * @brief This method traverses the IPD structure.
   * If a variable is encountered it is passed to the xmlwriter.
   * If a section is encountered the method calls itself.
   * @param iNode IPD iterator which should be traversed
   * @param xmldoc xmlwriter object used for XML output
   */
  void recursive_traverse(ipdIterator_t * iNode, xmlwriter& xmldoc);

  /*
   * @brief This method performs the transformation from IPD data structure to ViennaMaterials XML data layout
   * @param name The name of the value to be accessed
   * @param tn IPD tree node structure containing the evaluated value
   * @param xmldoc xmlwriter object used for XML output
   * @return XML element object containing the value in the ViennaMaterials XML layout
   */
  TiXmlElement* ipd_value_to_xml(const char* name, ipdTreeNode_t *tn, xmlwriter& xmldoc);

  //TODO doxygen
  std::string query_string_item_by_name_from_section(const char* section, const char* item_name);

private:
  statistic_data* statistics_;

};


#endif /* IPD2XML_HPP_ */
