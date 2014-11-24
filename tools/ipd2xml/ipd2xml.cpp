/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               matthias

   license:    see file LICENSE in the base directory
============================================================================= */

/*
 * This tool assumes a certain layout of the IPD data:
 *   First level:               IPD sections are mapped to ViennaMaterials groups (containing attributes and/or materials)
 *   |                          IPD variables are mapped to ViennaMaterials attributes (this attributes are not associated with a group or material)
 *   |
 *   -> Second level:           IPD sections are mapped to Viennamaterials materials
 *      |                       IPD variables are mapped to ViennaMaterials attributes (associated with the group of the first level)
 *      |
 *      -> Consecutive levels:  IPD sections are mappe to ViennaMaterials groups
 *                              IPD variables are mapped to ViennaMaterials attributes
 *
 *
 * Furthermore, this tool assumes 2 IPD variables of defined name which are mapped to 'name' and 'category' of ViennaMaterials materials.
 * See ipd_item_name and ipd_item_category in ipd2xml.hpp for the mapping.
 */

#include "tools/ipd2xml/ipd2xml.hpp"

int main(int argc, char** argv)
{

  const char* inputfile_ipd;
  const char* outputfile_xml;

  if(argc == 3)
  {
    inputfile_ipd = argv[1];
    outputfile_xml = argv[2];
  }else
  {
    std::cout << "loading from tools/ipd2xml/tmp/example.ipd" << std::endl;
    std::cout << "exporting to tools/ipd2xml/tmp/example.xml" << std::endl << std::endl;
    inputfile_ipd = "tools/ipd2xml/tmp/example.ipd";
    outputfile_xml = "tools/ipd2xml/tmp/example.xml";
  }

  xmlwriter xmldoc("This database was created by ipd2xml");

  // Initialize the Database.
  // Normally we can use the default values, so we don't need parameters.
  ipdInit(NULL, NULL);

  // Let's create a database
  ipdCreateBase("NameOfTheDatabase", 0);

  // Read a file in the database
  ipdReadInputDeck(inputfile_ipd);

  // Define and initialize a ViennaIPD iterator
  ipdIterator_t  *iNode = NULL;

  // Create a new iterator which operates on the root level
  ipdIteratorNewAtRootSection(&iNode, ipdANY);

  ipdIteratorDoStep(iNode);

  // Create a statistic_data object for the importer
  statistic_data statistics;

  // Create importer and access the IPD layout
  ipd_importer importer(&statistics, &xmldoc);
  importer.traverse_ipd_layout(iNode);

  // Free the iterator
  ipdIteratorFree(iNode);

  // Write the output file
  xmldoc.print(outputfile_xml);

  // Output the gathered statistic data
  std::cout << "Statistics:" << std::endl;
  std::cout << "  " << statistics.get_number_of_attributes_ipd() << " attributes fetched from IPD"    << std::endl;
  std::cout << "  " << statistics.get_number_of_attributes_xml() << " attributes exported in XML"     << std::endl;
  std::cout << "  " << statistics.get_number_of_invalid_nodes()  << " invalid IPD nodes encountered"  << std::endl;

#ifdef VERIFY_XML
  std::cout << std::endl << "Verifiying..." << std::endl;
  std::string filename(outputfile_xml);

  // Create a pugiXML backend and a proxy object to access the previously created XML file
  viennamaterials::backend_handle matlib(new viennamaterials::pugixml(filename));
  viennamaterials::proxy_handle myproxy(new viennamaterials::viennastar_proxy(matlib));

  // Create a new iterator which operates on the root level
  ipdIteratorNewAtRootSection(&iNode, ipdANY);
  ipdIteratorDoStep(iNode);

  // Traverse the ViennaIPD data structure using the iterator
  while (ipdIteratorIsValid(iNode))
    recurise_traverse_and_verify(iNode, myproxy);

  // Free the iterator
  ipdIteratorFree(iNode);

  std::cout << "...done" << std::endl;
#endif

  // Free the ViennaIPD datastructures
  ipdFreeAll();

  return 0;
}

xmlwriter::xmlwriter(const char* note)
{
  root_tag_                       = "database";
  id_tag_                         = "id";
  group_tag_                      = "group";
  attribute_tag_                  = "attribute";
  scalar_tag_                     = "scalar";
  type_attribute_tag_             = "type";
  type_boolean_                   = "bool";
  type_integer_                   = "int";
  type_floating_                  = "float";
  tensor_tag_                     = "tensor";
  tensor_order_attribute_tag_     = "order";
  tensor_dimension_attribute_tag_ = "d";
  tensor_index_attribute_tag_     = "i";
  unit_tag_                       = "unit";
  note_tag_                       = "note";
  category_tag_                   = "category";
  material_tag_                   = "material";
  name_tag_                       = "name";
  string_tag_                     = "string";

  // Initialize the XML document by inserting XML version, root element and optional note element

  TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
  doc_.LinkEndChild( decl );

  TiXmlElement * root = new TiXmlElement( root_tag_ );
  doc_.LinkEndChild( root );

  if(strlen(note) > 0)
  {
    TiXmlElement * root_note = new TiXmlElement( note_tag_ );
    root_note->LinkEndChild( new TiXmlText( note ));
    root->LinkEndChild( root_note );
  }

  currentnode_ = root;
  nodecont_.push_back( root );
}

void xmlwriter::add_element(TiXmlElement* element)
{
  currentnode_->LinkEndChild(element);
}

TiXmlElement* xmlwriter::create_scalar(const char* id, const viennamaterials::xml_bool& value, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag_);

  TiXmlElement* id_element = new TiXmlElement(id_tag_);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* scalar_element = new TiXmlElement(scalar_tag_);
  scalar_element->SetAttribute(type_attribute_tag_, type_boolean_);
  if(value == true)
    scalar_element->LinkEndChild(new TiXmlText("true"));
  else
    scalar_element->LinkEndChild(new TiXmlText("false"));
  attribute_element->LinkEndChild(scalar_element);

  attribute_element->LinkEndChild(this->create_unit(unit));

  return attribute_element;
}

TiXmlElement* xmlwriter::create_scalar(const char* id, const viennamaterials::xml_int& value, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag_);

  TiXmlElement* id_element = new TiXmlElement(id_tag_);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* scalar_element = new TiXmlElement(scalar_tag_);
  scalar_element->SetAttribute(type_attribute_tag_, type_integer_);
  scalar_element->LinkEndChild( new TiXmlText(converter(value).c_str()) );
  attribute_element->LinkEndChild(scalar_element);

  attribute_element->LinkEndChild(this->create_unit(unit));

  return attribute_element;
}

TiXmlElement* xmlwriter::create_scalar(const char* id, const viennamaterials::xml_float& value, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag_);

  TiXmlElement* id_element = new TiXmlElement(id_tag_);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* scalar_element = new TiXmlElement(scalar_tag_);
  scalar_element->SetAttribute(type_attribute_tag_, type_floating_);
  scalar_element->LinkEndChild( new TiXmlText(converter(value).c_str()) );
  attribute_element->LinkEndChild(scalar_element);

  attribute_element->LinkEndChild(this->create_unit(unit));

  return attribute_element;
}

TiXmlElement* xmlwriter::create_tensor(const char* id, const ipdLong& dimensions, const ipdLong* dimension_length, const ipdDouble* values, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag_);

  TiXmlElement* id_element = new TiXmlElement(id_tag_);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);
  TiXmlElement* tensor_element = new TiXmlElement(tensor_tag_);
  tensor_element->SetAttribute(tensor_order_attribute_tag_, dimensions);
  for(long i = 0; i < dimensions; i++) // Add dimension XML attributes to the tensor element
  {
    std::string dimension = tensor_dimension_attribute_tag_;
    dimension.append(converter(i+1));
    tensor_element->SetAttribute(dimension.c_str(), dimension_length[dimensions - 1 - i]);
  }
  tensor_element->SetAttribute(type_attribute_tag_, type_floating_);
  attribute_element->LinkEndChild(tensor_element); // Link the 'empty' tensor element

  attribute_element->LinkEndChild(this->create_unit(unit));

  // Fill the tensor element (add scalar elements)
  long number_of_values = 1;
  for(long i = 0; i < dimensions; i++)
  {
    number_of_values *= dimension_length[i];
  }

  long* tensor_index = new long[dimensions];
  for(long value_index = 0; value_index < number_of_values; value_index++) // Index of IPD value field
  {
    // Only create and add scalar element if value != 0.0
    if(values[value_index] != 0.0)
    {
      TiXmlElement* scalar_element  = new TiXmlElement(scalar_tag_);

      // Calculate indices for current scalar value
      for(long current_index = 0; current_index < dimensions; current_index++) // Indices of scalar element
      {
        long my_index = value_index;
        long index_offset = 1;
        for(long i = current_index; i > 0; i--)
        {
          my_index      -= tensor_index[i-1];
          index_offset  *= dimension_length[dimensions - i];
        }
        tensor_index[current_index] = (my_index / index_offset) % dimension_length[dimensions - 1 - current_index];

        // Compose the index XML attribute tag
        std::string attribute = tensor_index_attribute_tag_;
        attribute.append(converter(current_index));
        // Add the index XML attribute to the scalar element
        scalar_element->SetAttribute(attribute.c_str(), tensor_index[current_index]);
      }

      scalar_element->LinkEndChild( new TiXmlText(converter(values[value_index]).c_str()) );
      tensor_element->LinkEndChild(scalar_element);
    }
  }
  delete[] tensor_index;

  return attribute_element;
}

TiXmlElement* xmlwriter::create_string(const char* id, const viennamaterials::xml_string& value)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag_);

  TiXmlElement* id_element = new TiXmlElement(id_tag_);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* string_element = new TiXmlElement(string_tag_);
  if(value.size() > 0)
    string_element->LinkEndChild( new TiXmlText(value.c_str()) );
  attribute_element->LinkEndChild(string_element);

  attribute_element->LinkEndChild(this->create_unit(""));

  return attribute_element;
}

void xmlwriter::add_note(const char* note)
{
  TiXmlElement* note_element = new TiXmlElement(note_tag_);
  note_element->LinkEndChild(new TiXmlText(note));
  this->add_element(note_element);
}

void xmlwriter::open_material_element(const char* id, const char* name, const char* category)
{
  TiXmlElement* material_element = new TiXmlElement(material_tag_);

  TiXmlElement* id_element = new TiXmlElement(id_tag_);
  id_element->LinkEndChild(new TiXmlText(id));
  material_element->LinkEndChild(id_element);

  if(strlen(name) != 0) // Name element is optional
  {
    TiXmlElement* name_element = new TiXmlElement(name_tag_);
    name_element->LinkEndChild(new TiXmlText(name));
    material_element->LinkEndChild(name_element);
  }

  TiXmlElement* category_element = new TiXmlElement(category_tag_);
  category_element->LinkEndChild(new TiXmlText(category));
  material_element->LinkEndChild(category_element);

  currentnode_->LinkEndChild(material_element);
  currentnode_ = material_element;
  nodecont_.push_back(material_element);
}

void xmlwriter::close_material_element() //wrapper for update(), use with care
{
  this->update();
}

void xmlwriter::open_group_element(const char* id, const char* name, const char* category)
{
  TiXmlElement* group_element = new TiXmlElement(group_tag_);

  TiXmlElement* id_element = new TiXmlElement(id_tag_);
  id_element->LinkEndChild(new TiXmlText(id));
  group_element->LinkEndChild(id_element);

  if(strlen(name) != 0) // Name element is optional
  {
    TiXmlElement* name_element = new TiXmlElement(name_tag_);
    name_element->LinkEndChild(new TiXmlText(name));
    group_element->LinkEndChild(name_element);
  }

  if(strlen(category) != 0) // Category element is optional
  {
    TiXmlElement* category_element = new TiXmlElement(category_tag_);
    category_element->LinkEndChild(new TiXmlText(category));
    group_element->LinkEndChild(category_element);
  }

  currentnode_->LinkEndChild(group_element);
  currentnode_ = group_element;
  nodecont_.push_back(group_element);
}

void xmlwriter::close_group_element() //wrapper for update(), use with care
{
  this->update();
}

void xmlwriter::update()
{
  nodecont_.pop_back();
  currentnode_ = nodecont_[nodecont_.size()-1];
}

void xmlwriter::print(std::string filename)
{
  this->print(filename.c_str());
}

void xmlwriter::print(const char* filename)
{
  doc_.SaveFile( filename );
}

void xmlwriter::print_to_console(void)
{
  TiXmlPrinter printer;
  printer.SetIndent("  ");
  doc_.Accept(&printer);
  std::cout << printer.CStr() << std::endl;
}

TiXmlElement* xmlwriter::create_unit(const char* unit)
{
  TiXmlElement* unit_element = new TiXmlElement(unit_tag_);
  if(strlen(unit) > 0)
    unit_element->LinkEndChild(new TiXmlText(unit));
  return unit_element;
}

ipd_importer::ipd_importer(statistic_data* statistics, xmlwriter* xmldoc)
{
  this->statistics_ = statistics;
  this->xmldoc_     = xmldoc;
}

TiXmlElement* ipd_importer::ipd_value_to_xml(const char* name, ipdTreeNode_t *tn)
{
  TiXmlElement* attribute_element = 0;
  ipdBoolean ipd_success          = ipdFALSE;

  statistics_->increment_attribute_number_ipd();

  // According to the IPD type, the corresponding ViennaMaterials data type is created and given to the XML writer
  switch (tn->type)
  {
    case ipdINTEGER:
    {
      ipdLong value;
      ipd_success = ipdGetIntegerByName(name, &value);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(name);
        throw ipd2xml_error("Error while accessing integer value(" + name_str + ")");
      }
      viennamaterials::xml_int val = value;
      attribute_element = xmldoc_->create_scalar(name, val, "");
      break;
    }

    case ipdREAL:
    case 4098: // rationals .. will be evaluated to floats
    {
      ipdDouble value;
      ipd_success = ipdGetRealByName(name, &value);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(name);
        throw ipd2xml_error("Error while accessing real/rational value(" + name_str + ")");
      }
      viennamaterials::xml_float val = value;
      attribute_element = xmldoc_->create_scalar(name, val, "");
      break;
    }

    case ipdREALQUANTITY:
    case 4106:
    {
      ipdDouble value;
      ipdChar* unit;
      ipd_success = ipdGetRealQuantityByName(name, &value, &unit);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(name);
        throw ipd2xml_error("Error while accessing real quantity value(" + name_str + ")");
      }
      viennamaterials::xml_float val = value;
      attribute_element = xmldoc_->create_scalar(name, val, unit);
      break;
    }

    case ipdBOOLEAN:
    {
      ipdBoolean value;
      ipd_success = ipdGetBooleanByName(name, &value);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(name);
        throw ipd2xml_error("Error while accessing boolean value(" + name_str + ")");
      }
      viennamaterials::xml_bool val;
      if(value == ipdTRUE)
        val = true;
      else
        val = false;
      attribute_element = xmldoc_->create_scalar(name, val, "");
      break;
    }

    case 192: //array
    {
      ipdLong dimension;
      ipdLong* length;
      ipdDouble* matrix;
      const ipdChar* unit;
      ipd_success = ipdGetSloppyRealMatrixByName(name, &dimension, &length, &matrix, &unit);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(name);
        throw ipd2xml_error("Error while accessing array/matrix/tensor value(" + name_str + ")");
      }
      attribute_element = xmldoc_->create_tensor(name, dimension, length, matrix, converter(unit).c_str());

      free(length);
      free(matrix);
      break;
    }

    case ipdSTRING:
    case 2080: // concatenated strings ..
    {
      ipdChar* string = 0;
      ipd_success = ipdGetStringByName(name, &string);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(name);
        throw ipd2xml_error("Error while accessing string value(" + name_str + ")");
      }

      //for an empty string in IPD file, ipdGetStringByName returns a zero pointer
      viennamaterials::xml_string value = "";
      if(string != 0)
        value = string;
      attribute_element = xmldoc_->create_string(name, value);
      break;
    }

    default:
      std::string type_str("");
      type_str = converter(tn->type);
      throw ipd2xml_error("Not implemented IPD structure type (" + type_str + ") encountered (" + name +")");
      break;
  }


  if(attribute_element != 0)
    statistics_->increment_attribute_number_xml();

  return attribute_element;
}

void ipd_importer::recursive_traverse(ipdIterator_t * iNode)
{
  // Traverse the ViennaIPD data structure using the iterator
  while (ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    // If the current element is a _variable_
    if (ipdIteratorGetType(iNode) == ipdVARIABLE) // Corresponds to attribute in ViennaMaterials data layout
    {
      // Skip the current element if it is the 'materials' or 'class' element
      // (those are already handled in the ipdSECTION program path of recursive_traverse() or access_ipd_material() )
      std::string variable_name(ipdIteratorGetItemName(iNode));
      if(variable_name.compare(ipd_item_name) != 0 && variable_name.compare(ipd_item_category) != 0)
      {
        TiXmlElement* attribute = ipd_value_to_xml(iNode->tn->node.sv.name, ipdIteratorEval(iNode));
        if(attribute != 0)
          xmldoc_->add_element(attribute);
      }

    // If the current element is a _section_
    }else if (ipdIteratorGetType(iNode) == ipdSECTION) // Corresponds to group in ViennaMaterials data layout
    {
      // Create a new iterator which should traverse the subsection
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      // Query name and category from IPD
      std::string section_full_ipd_name(ipdIteratorGetItemFullName(iNode));
      std::string name      = query_string_item_by_name_from_section(section_full_ipd_name.c_str(), ipd_item_name.c_str());
      std::string category  = query_string_item_by_name_from_section(section_full_ipd_name.c_str(), ipd_item_category.c_str());

      xmldoc_->open_group_element(itemName, name.c_str(), category.c_str());
      recursive_traverse(iSubNode);
      xmldoc_->close_group_element();

      ipdIteratorFree(iSubNode);
    }

    // Next item
    ipdIteratorDoNext(iNode);
  }

}

void ipd_importer::access_ipd_material(ipdIterator_t * iNode)
{
  // Traverse the ViennaIPD material data structure using the iterator
  if(ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item, the material
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    if (ipdIteratorGetType(iNode) == ipdSECTION) // Corresponds to material in ViennaMaterials data layout
    {
      // Create a new iterator which should traverse the subsection, the attributes of the material
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      // Query name and category from IPD
      std::string material_full_ipd_name(ipdIteratorGetItemFullName(iNode));
      std::string name      = query_string_item_by_name_from_section(material_full_ipd_name.c_str(), ipd_item_name.c_str());
      std::string category  = query_string_item_by_name_from_section(material_full_ipd_name.c_str(), ipd_item_category.c_str());

      xmldoc_->open_material_element(itemName, name.c_str(), category.c_str());
      // Traverse the material data structure
      recursive_traverse(iSubNode);
      xmldoc_->close_material_element();

      ipdIteratorFree(iSubNode);

    }else if (ipdIteratorGetType(iNode) == ipdVARIABLE) // Corresponds to attribute in ViennaMaterials data layout
    {
      TiXmlElement* attribute = ipd_value_to_xml(iNode->tn->node.sv.name, ipdIteratorEval(iNode));
      if(attribute != 0)
        xmldoc_->add_element(attribute);
    }else // Invalid IPD layout
    {
      statistics_->increment_invalid_node_number();

      std::string full_name = ipdIteratorGetItemFullName(iNode);
#ifdef VERBOSE_MODE
      std::cout << "INFO: Invalid IPD layout (" + full_name + ")" << std::endl;
#endif
    }
  }else
  {
    throw ipd2xml_error("Invalid IPD iterator passed to access_ipd_material");
  }
}

void ipd_importer::traverse_ipd_layout(ipdIterator_t * iNode)
{
  // Traverse the ViennaIPD data structure using the iterator
  while (ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    if (ipdIteratorGetType(iNode) == ipdSECTION) // Corresponds to group in ViennaMaterials data layout
    {
      // Create a new iterator which should traverse the subsection, the materials
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      // Section at first level of IPD data layout is assumed as group
      xmldoc_->open_group_element(itemName, "", "");

      // Second level if IPD data layout is assumed as material
      while(ipdIteratorIsValid(iSubNode))
      {
        access_ipd_material(iSubNode);
        ipdIteratorDoNext(iSubNode);
      }

      xmldoc_->close_group_element();

      ipdIteratorFree(iSubNode);

    }else if (ipdIteratorGetType(iNode) == ipdVARIABLE) // Corresponds to attribute in ViennaMaterials data layout
    {
      TiXmlElement* attribute = ipd_value_to_xml(iNode->tn->node.sv.name, ipdIteratorEval(iNode));
      if(attribute != 0)
        xmldoc_->add_element(attribute);
    }else // Invalid IPD layout
    {
      statistics_->increment_invalid_node_number();

      std::string full_name = ipdIteratorGetItemFullName(iNode);
#ifdef VERBOSE_MODE
      std::cout << "INFO: Invalid IPD layout (" + full_name + ")" << std::endl;
#endif
    }

    // Next item
    ipdIteratorDoNext(iNode);
  }
}

std::string ipd_importer::query_string_item_by_name_from_section(const char* section, const char* item_name)
{
  std::string return_val("");

  std::string item_full_ipd_name(section);
  item_full_ipd_name.append(".");
  item_full_ipd_name.append(item_name);

  // Check if item exists
  ipdTreeNode_t* node_type = ipdExistVariableByName(item_full_ipd_name.c_str());
  if(node_type != NULL)
  {
    // Eval item to get type
    ipdTreeNode_s* node = ipdEvalByName(item_full_ipd_name.c_str(), ipdTRUE);
    if(node->type == ipdSTRING || node->type == 2080)
    {
      ipdChar* string = 0;
      ipdBoolean ipd_success = ipdGetStringByName(item_full_ipd_name.c_str(), &string);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(item_full_ipd_name.c_str());
        throw ipd2xml_error("Error while accessing string value(" + name_str + ")");
      }
      // For an empty string in IPD file, ipdGetStringByName returns a zero pointer
      if(string != 0)
        return_val = string;
    }
  }
  return return_val;
}

statistic_data::statistic_data()
{
  this->number_of_attributes_ipd  = 0;
  this->number_of_attributes_xml  = 0;
  this->invalid_ipd_nodes         = 0;
}

void statistic_data::increment_attribute_number_ipd()
{
  this->number_of_attributes_ipd++;
}

long statistic_data::get_number_of_attributes_ipd()
{
  return this->number_of_attributes_ipd;
}

void statistic_data::increment_attribute_number_xml()
{
  this->number_of_attributes_xml++;
}

long statistic_data::get_number_of_attributes_xml()
{
  return this->number_of_attributes_xml;
}

void statistic_data::increment_invalid_node_number()
{
  this->invalid_ipd_nodes++;
}
long statistic_data::get_number_of_invalid_nodes()
{
  return this->invalid_ipd_nodes;
}

#ifdef VERIFY_XML
void recurise_traverse_and_verify(ipdIterator_t * iNode, viennamaterials::proxy_handle my_proxy)
{
  // Traverse the ViennaIPD data structure using the iterator
  while (ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    // If the current element is a _variable_
    if (ipdIteratorGetType(iNode) == ipdVARIABLE)
    {
      // Skip the current element if it is the 'materials' or 'class' element
      std::string variable_name(ipdIteratorGetItemName(iNode));
      if(variable_name.compare(ipd_item_name) != 0 && variable_name.compare(ipd_item_category) != 0)
      {
        const char* name = ipdIteratorGetItemFullName(iNode);
        ipdTreeNode_t* tn = ipdIteratorEval(iNode);

        ipdBoolean ipd_success = ipdFALSE;

        std::cout.precision(STRINGSTREAM_PRECISION);

        // Gather data from IPD and ViennaMaterials and compare results according to the type of data
        switch (tn->type)
        {
          case ipdINTEGER:
          {
            ipdLong value;
            ipd_success = ipdGetIntegerByName(name, &value);
            if(ipd_success == ipdFALSE)
            {
              std::string name_str(name);
              throw ipd2xml_error("Error while accessing integer value(" + name_str + ")");
            }
            viennamaterials::xml_int value_xml = my_proxy->query_value<viennamaterials::xml_int>(ipd_path_to_xml(name));
            if(value != value_xml)
            {
              output_mismatch(name);
              std::cout << "  IPD: " << value     << std::endl;
              std::cout << "  XML: " << value_xml << std::endl;
            }
            if((my_proxy->query_unit(ipd_path_to_xml(name)).length() != 0))
            {
              output_mismatch(name);
              std::cout << "  unit mismatch" << std::endl;
            }
            break;
          }

          case ipdREAL:
          case 4098: // rationals .. will be evaluated to floats
          {
            ipdDouble value;
            ipd_success = ipdGetRealByName(name, &value);
            if(ipd_success == ipdFALSE)
            {
              std::string name_str(name);
              throw ipd2xml_error("Error while accessing real/rational value(" + name_str + ")");
            }
            viennamaterials::xml_float value_xml = my_proxy->query_value<viennamaterials::xml_float>(ipd_path_to_xml(name));
            if(value != value_xml)
            {
              output_mismatch(name);
              std::cout << "  IPD: " << value     << std::endl;
              std::cout << "  XML: " << value_xml << std::endl;
            }
            if((my_proxy->query_unit(ipd_path_to_xml(name)).length() != 0))
            {
              output_mismatch(name);
              std::cout << "  unit mismatch" << std::endl;
            }
            break;
          }

          case ipdREALQUANTITY:
          case 4106:
          {
            ipdDouble value;
            ipdChar* unit;
            ipd_success = ipdGetRealQuantityByName(name, &value, &unit);
            if(ipd_success == ipdFALSE)
            {
              std::string name_str(name);
              throw ipd2xml_error("Error while accessing real quantity value(" + name_str + ")");
            }
            viennamaterials::xml_float value_xml = my_proxy->query_value<viennamaterials::xml_float>(ipd_path_to_xml(name));
            std::string unit_xml = my_proxy->query_unit(ipd_path_to_xml(name));
            if(almost_equal(value, value_xml) == false || unit_xml.compare(unit) != 0)
            {
              output_mismatch(name);
              std::cout << "  IPD: " << value     << " " << unit << std::endl;
              std::cout << "  XML: " << value_xml << " " << unit_xml << std::endl;
              if(unit_xml.compare(unit) != 0)
                std::cout << "  unit mismatch" << std::endl;
            }
            break;
          }

          case ipdBOOLEAN:
          {
            ipdBoolean value;
            ipd_success = ipdGetBooleanByName(name, &value);
            if(ipd_success == ipdFALSE)
            {
              std::string name_str(name);
              throw ipd2xml_error("Error while accessing boolean value(" + name_str + ")");
            }
            viennamaterials::xml_bool value_xml = my_proxy->query_value<viennamaterials::xml_bool>(ipd_path_to_xml(name));
            if( (value == ipdTRUE && value_xml == false) || (value == ipdFALSE && value_xml == true))
            {
              output_mismatch(name);
              std::cout << "  IPD: " << value     << std::endl;
              std::cout << "  XML: " << value_xml << std::endl;
            }
            if((my_proxy->query_unit(ipd_path_to_xml(name)).length() != 0))
            {
              output_mismatch(name);
              std::cout << "  unit mismatch" << std::endl;
            }
            break;
          }

          case 192: //array
          {
            //TODO array (matrix) not supported yet by ViennaMaterials
            break;
          }

          case ipdSTRING:
          case 2080: // concatenated strings ..
          {
            ipdChar* string = 0;
            ipd_success = ipdGetStringByName(name, &string);
            if(ipd_success == ipdFALSE)
            {
              std::string name_str(name);
              throw ipd2xml_error("Error while accessing string value(" + name_str + ")");
            }

            //for an empty string in IPD file, ipdGetStringByName returns a zero pointer
            viennamaterials::xml_string value = "";
            if(string != 0)
              value = string;

            viennamaterials::xml_string value_xml = my_proxy->query_value<viennamaterials::xml_string>(ipd_path_to_xml(name));
            if(value_xml.compare(value) != 0)
            {
              output_mismatch(name);
              std::cout << "  IPD: " << value     << std::endl;
              std::cout << "  XML: " << value_xml << std::endl;
            }
            if((my_proxy->query_unit(ipd_path_to_xml(name)).length() != 0))
            {
              output_mismatch(name);
              std::cout << "  unit mismatch" << std::endl;
            }
            break;
          }

          default:
            std::string type_str("");
            type_str = converter(tn->type);
            throw ipd2xml_error("Not implemented IPD structure type (" + type_str + ") encountered (" + name +")");
            break;
        }
      }

    // If the current element is a _section_
    }else if (ipdIteratorGetType(iNode) == ipdSECTION)
    {
      // Create a new iterator which should traverse the subsection
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      recurise_traverse_and_verify(iSubNode, my_proxy);

      ipdIteratorFree(iSubNode);
    }

    // Next item
    ipdIteratorDoNext(iNode);
  }
}

std::string ipd_path_to_xml(const char* ipd_path)
{
  std::string path_conversion(ipd_path);
  path_conversion = path_conversion.erase(0, 1); // Remove facing '~'
  std::replace(path_conversion.begin(), path_conversion.end(), '.', '/'); // Replace '.' with '/'
  return path_conversion;
}

void output_mismatch(const char* ipd_path)
{
  std::cout << "MISMATCH: "   << std::endl;
  std::cout << "  IPD path: " << ipd_path                   << std::endl;
  std::cout << "  XML path: " << ipd_path_to_xml(ipd_path)  << std::endl;
}

bool almost_equal(double a, double b)
{
  const double max_relative_error = 1e-6;
  if(a != 0.0)
  {
    if(fabs(a-b)/fabs(a) <= max_relative_error)
      return true;
  }else
    if(fabs(a-b) <= max_relative_error)
      return true;

  return false;
}

#endif
