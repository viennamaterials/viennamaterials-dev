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

template < typename T >
std::string converter(T t)
{
   std::stringstream ss;
   ss << t;
   return ss.str();
}

//
// ----------------------------------------------------------
//

//TODO doxygen
struct xmlwriter
{
  xmlwriter(const char* note);
  void add_element(TiXmlElement* element);
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_bool& value, const char* unit);
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_int& value, const char* unit);
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_float& value, const char* unit);
  TiXmlElement* create_tensor(const char* id, const double& tensor_rows, const double& tensor_columns, const double& tensor_order, const double* values, const char* unit);
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


  TiXmlDocument doc;

  TiXmlElement * currentnode;

  const char* root_tag;
  const char* id_tag;
  const char* group_tag;
  const char* attribute_tag;
  const char* scalar_tag;
  const char* type_attribute_tag;
  const char* type_boolean;
  const char* type_integer;
  const char* type_floating;
  const char* tensor_tag;
  const char* tensor_row_attribute_tag;
  const char* tensor_column_attribute_tag;
  const char* tensor_order_attribute_tag;
  const char* unit_tag;
  const char* note_tag;
  const char* category_tag;
  const char* material_tag;
  const char* name_tag;
  const char* string_tag;

  std::vector< TiXmlElement * > nodecont;
};

/*
 * @brief This method traverses the IPD layout to locate a materials
 * @param iNode IPD iterator pointing to the root of the IPD input file
 * @param xmldoc xmlwriter object used for XML output
 */
void traverse_ipd_layout(ipdIterator_t * iNode, xmlwriter& xmldoc);

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


#endif /* IPD2XML_HPP_ */
