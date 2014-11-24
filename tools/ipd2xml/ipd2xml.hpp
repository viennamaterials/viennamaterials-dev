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

#ifdef VERIFY_XML
#include "viennamaterials/forwards.h"
#include "viennamaterials/pugixml.hpp"
#include "viennamaterials/proxy/viennastar.hpp"
#include <math.h>

/**
 * @brief This method recursively traverses the given IPD structure, evaluates the IPD data and verifies it with the corresponding data queried by a ViennaMaterials proxy.
 * @param iNode IPD iterator of the current level of the IPD data structure
 * @param my_proxy ViennaMaterials proxy_handle used to access data by ViennaMaterials
 */
void recurise_traverse_and_verify(ipdIterator_t * iNode, viennamaterials::proxy_handle my_proxy);

/**
 * @brief This method returns the corresponding ViennaMaterials proxy path from a given IPD path
 * @param ipd_path A C-string containing the full/absolute IPD path
 * @return A string containing the ViennaMaterials proxy path
 */
std::string ipd_path_to_xml(const char* ipd_path);

/**
 * @brief This method writes IPD and ViennaMaterials path to the console.
 * @param ipd_path A C-string containing the full/absolute IPD path
 */
void output_mismatch(const char* ipd_path);

/**
 * @brief This method compares two given double values if they are (almost) equal.
 * @param a Examinee
 * @param b Examinee
 * @return true if 'a' can be considered equal as 'b', false otherwise
 */
bool almost_equal(double a, double b);
#endif


/// String holding the IPD item name of the 'name' XML element of a ViennaMaterials material
const std::string ipd_item_name("materials"); //'materials' IPD item is mapped to 'name' XML element of a ViennaMaterials material
/// String holding the IPD item name of the 'category' XML element of a ViennaMaterials material
const std::string ipd_item_category("class"); //'class' IPD item is mapped to 'category' XML element of a ViennaMaterials material

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


/// @brief Precision for stringstream used in converter method
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

/**
 * @brief Class holding counters which are used for statistics
 */
class statistic_data
{
public:
  statistic_data();
  /// @brief Increment the IPD attributes counter by 1
  void increment_attribute_number_ipd();
  /// @brief Get the value of IPD attributes counter
  /// @return long Value of the counter
  long get_number_of_attributes_ipd();
  /// @brief Increment the XML attributes counter by 1
  void increment_attribute_number_xml();
  /// @brief Get the value of XML attributes counter
  /// @return long Value of the counter
  long get_number_of_attributes_xml();
  /// @brief Increment the invalid attributes counter by 1
  void increment_invalid_node_number();
  /// @brief Get the value of invalid attributes counter
  /// @return long Value of the counter
  long get_number_of_invalid_nodes();

private:
  long number_of_attributes_ipd;
  long number_of_attributes_xml;
  long invalid_ipd_nodes;
};

/**
 * @brief This class provides methods to create an writes attributes to XML files according to the ViennaMaterials XML layout.
 */
class xmlwriter
{
public:
  xmlwriter(const char* note);

  /**
   * @brief This method adds the given XML element to the current node in the XML tree
   * @param element The element which should be added
   */
  void add_element(TiXmlElement* element);

  /**
   * @brief This method creates a new XML representation of a scalar attribute according to the values given and returns a pointer to the new XML element object.
   * @param id The ID of this attribute
   * @param value The boolean value of this attribute
   * @param unit The unit of this attribute
   * @return A pointer to the new XML element object
   */
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_bool& value, const char* unit);

  /**
   * @brief This method creates a new XML representation of a scalar attribute according to the values given and returns a pointer to the new XML element object.
   * @param id The ID of this attribute
   * @param value The integer value of this attribute
   * @param unit The unit of this attribute
   * @return A pointer to the new XML element object
   */
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_int& value, const char* unit);

  /**
   * @brief This method creates a new XML representation of a scalar attribute according to the values given and returns a pointer to the new XML element object.
   * @param id The ID of this attribute
   * @param value The floating point value of this attribute
   * @param unit The unit of this attribute
   * @return A pointer to the new XML element object
   */
  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_float& value, const char* unit);

  /**
   * @brief This method creates a new XML representation of a tensor attribute according to the values given and returns a pointer to the new XML element object.
   * @param id The ID of this attribute
   * @param dimensions The order (number of dimensions) of the tensor
   * @param dimension_length An array containing the length of each dimension of the tensor
   * @param values An array containing the values of this attribute
   * @param unit The unit of this attribute
   * @return A pointer to the new XML element object
   */
  TiXmlElement* create_tensor(const char* id, const ipdLong& dimensions, const ipdLong* dimension_length, const ipdDouble* values, const char* unit);

  /**
   * @brief This method creates a new XML representation of a string attribute according to the values given and returns a pointer to the new XML element object.
   * @param id The ID of this attribute
   * @param value The string value of this attribute
   * @return A pointer to the new XML element object
   */
  TiXmlElement* create_string(const char* id, const viennamaterials::xml_string& value);

  /**
   * @brief This method adds a note-XML element to the current node in the XML tree
   * @param note The note text which
   */
  void add_note(const char* note);

  /**
   * @brief This method adds a new material element to the XML tree and sets the current node to this material element
   * @param id The ID of this material
   * @param name The name of this material
   * @param category The category of this material
   */
  void open_material_element(const char* id, const char* name, const char* category);

  /**
   * @brief Wrapper function for update()
   */
  void close_material_element(); //wrapper for update(), use with care

  /**
   * @brief This method adds a new group element to the XML tree and sets the current node to this group element
   * @param id The ID of this group
   * @param name The name of this group
   * @param category The category of this group
   */
  void open_group_element(const char* id, const char* name, const char* category);

  /**
   * @brief Wrapper function for update()
   */
  void close_group_element(); //wrapper for update(), use with care

  /**
   * @brief This method finishes/closes the current node and sets the parent node as current node
   */
  void update();

  /**
   * @brief This methods writes the XML tree to the specified output file
   * @param filename The filename of the output file
   */
  void print(std::string filename);

  /**
   * @brief This methods writes the XML tree to the specified output file
   * @param filename The filename of the output file
   */
  void print(const char* filename);

  /**
   * @brief This methods prints the XML tree to the standard output stream
   */
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


/**
 * @brief This class provides methods to traverse a defined IPD structure to extract material attributes.
 *        The material attributes are passed to an XML writer object.
 */
class ipd_importer
{

public:
  ipd_importer(statistic_data* statistics, xmlwriter* xmldoc);

  /**
   * @brief This method traverses the IPD layout to locate a materials
   * @param iNode IPD iterator pointing to the root of the IPD input file
   */
  void traverse_ipd_layout(ipdIterator_t * iNode);

private:
  /**
   * @brief This method adds a new material to XML and subsequently traverses the IPD structure for attributes.
   * @param iNode An IPD iterator pointing to a material
   */
  void access_ipd_material(ipdIterator_t * iNode);

  /**
   * @brief This method traverses the IPD structure.
   * If a variable is encountered it is passed to the xmlwriter.
   * If a section is encountered the method calls itself.
   * @param iNode IPD iterator which should be traversed
   */
  void recursive_traverse(ipdIterator_t * iNode);

  /**
   * @brief This method performs the transformation from IPD data structure to ViennaMaterials XML data layout
   * @param name The name of the value to be accessed
   * @param tn IPD tree node structure containing the evaluated value
   * @return XML element object containing the value in the ViennaMaterials XML layout
   */
  TiXmlElement* ipd_value_to_xml(const char* name, ipdTreeNode_t *tn);

  /**
   * @brief This method queries a string item by its name within an given section in the IPD structure.
   * @param section An IPD path (full/absolute path) to a section
   * @param item_name The name of the item within the given section holding a string value
   * @return A string holding the value of the identified item
   */
  std::string query_string_item_by_name_from_section(const char* section, const char* item_name);

private:
  statistic_data* statistics_;
  xmlwriter* xmldoc_;
};


#endif /* IPD2XML_HPP_ */
