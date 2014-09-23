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

#include "tools/ipd2xml/ipd2xml.hpp"
#include <iostream>


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

  traverse_ipd_layout(iNode, xmldoc);

  // Free the iterator
  ipdIteratorFree(iNode);

  // Free the ViennaIPD datastructures
  ipdFreeAll();

  xmldoc.print(outputfile_xml);
//  xmldoc.print_to_console(); //XXX

  return 0;
}
//
// ----------------------------------------------------------
//

xmlwriter::xmlwriter(const char* note)
{
  root_tag                    = "database";
  id_tag                      = "id";
  group_tag                   = "group";
  attribute_tag               = "attribute";
  scalar_tag                  = "scalar";
  type_attribute_tag          = "type";
  type_boolean                = "bool";
  type_integer                = "int";
  type_floating               = "float";
  tensor_tag                  = "tensor";
  tensor_row_attribute_tag    = "row";
  tensor_column_attribute_tag = "col";
  tensor_order_attribute_tag  = "order";
  unit_tag                    = "unit";
  note_tag                    = "note";
  category_tag                = "category";
  material_tag                = "material";
  name_tag                    = "name";

  TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
  doc.LinkEndChild( decl );

  TiXmlElement * root = new TiXmlElement( root_tag );
  doc.LinkEndChild( root );

  TiXmlElement * rootid = new TiXmlElement( note_tag );
  rootid->LinkEndChild( new TiXmlText( note ));
  root->LinkEndChild( rootid );

  currentnode = root;
  nodecont.push_back( root );
}

void xmlwriter::add_element(TiXmlElement* element)
{
  currentnode->LinkEndChild(element);
}

TiXmlElement* xmlwriter::create_scalar(const char* id, const viennamaterials::xml_bool& value, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag);

  TiXmlElement* id_element = new TiXmlElement(id_tag);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* scalar_element = new TiXmlElement(scalar_tag);
  scalar_element->SetAttribute(type_attribute_tag, type_boolean);
  if(value == true)
    scalar_element->LinkEndChild(new TiXmlText("true"));
  else
    scalar_element->LinkEndChild(new TiXmlText("false"));
  attribute_element->LinkEndChild(scalar_element);

  TiXmlElement* unit_element = new TiXmlElement(unit_tag);
  unit_element->LinkEndChild(new TiXmlText(unit));
  attribute_element->LinkEndChild(unit_element);

  return attribute_element;
}

TiXmlElement* xmlwriter::create_scalar(const char* id, const viennamaterials::xml_int& value, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag);

  TiXmlElement* id_element = new TiXmlElement(id_tag);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* scalar_element = new TiXmlElement(scalar_tag);
  scalar_element->SetAttribute(type_attribute_tag, type_integer);
  scalar_element->LinkEndChild( new TiXmlText(converter(value).c_str()) );
  attribute_element->LinkEndChild(scalar_element);

  TiXmlElement* unit_element = new TiXmlElement(unit_tag);
  unit_element->LinkEndChild(new TiXmlText(unit));
  attribute_element->LinkEndChild(unit_element);

  return attribute_element;
}

TiXmlElement* xmlwriter::create_scalar(const char* id, const viennamaterials::xml_float& value, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag);

  TiXmlElement* id_element = new TiXmlElement(id_tag);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* scalar_element = new TiXmlElement(scalar_tag);
  scalar_element->SetAttribute(type_attribute_tag, type_floating);
  scalar_element->LinkEndChild( new TiXmlText(converter(value).c_str()) );
  attribute_element->LinkEndChild(scalar_element);

  TiXmlElement* unit_element = new TiXmlElement(unit_tag);
  unit_element->LinkEndChild(new TiXmlText(unit));
  attribute_element->LinkEndChild(unit_element);

  return attribute_element;
}

TiXmlElement* xmlwriter::create_tensor(const char* id, const double& tensor_rows, const double& tensor_columns, const double& tensor_order, const double* values, const char* unit)
{
  TiXmlElement* attribute_element = new TiXmlElement(attribute_tag);

  TiXmlElement* id_element = new TiXmlElement(id_tag);
  id_element->LinkEndChild(new TiXmlText(id));
  attribute_element->LinkEndChild(id_element);

  TiXmlElement* tensor_element = new TiXmlElement(tensor_tag);
  tensor_element->SetAttribute(tensor_row_attribute_tag, tensor_rows);
  tensor_element->SetAttribute(tensor_column_attribute_tag, tensor_columns);
  tensor_element->SetAttribute(tensor_order_attribute_tag, tensor_order);
  tensor_element->SetAttribute(type_attribute_tag, type_floating);
  attribute_element->LinkEndChild(tensor_element);

  TiXmlElement* unit_element = new TiXmlElement(unit_tag);
  unit_element->LinkEndChild(new TiXmlText(unit));
  attribute_element->LinkEndChild(unit_element);

  long row, column, order;
  long value_item = 0;
  for(order = 1; order <= tensor_order; order++)
  {
    for(row = 1; row <= tensor_rows; row++)
    {
      for(column = 1; column <= tensor_columns; column++)
      {
        TiXmlElement* scalar_element = new TiXmlElement(scalar_tag);
        scalar_element->SetAttribute(tensor_row_attribute_tag, row);
        scalar_element->SetAttribute(tensor_column_attribute_tag, column);
        scalar_element->SetAttribute(tensor_order_attribute_tag, order);
        scalar_element->LinkEndChild( new TiXmlText(converter(values[value_item]).c_str()) );
        tensor_element->LinkEndChild(scalar_element);

        value_item++;
      }
    }
  }

  return attribute_element;
}

void xmlwriter::add_note(const char* note)
{
  TiXmlElement* note_element = new TiXmlElement(note_tag);
  note_element->LinkEndChild(new TiXmlText(note));
  this->add_element(note_element);
}

void xmlwriter::open_material_element(const char* id, const char* name, const char* category)
{
  TiXmlElement* material_element = new TiXmlElement(material_tag);

  TiXmlElement* id_element = new TiXmlElement(id_tag);
  id_element->LinkEndChild(new TiXmlText(id));
  material_element->LinkEndChild(id_element);

  if(strlen(name) != 0)
  {
    TiXmlElement* name_element = new TiXmlElement(name_tag);
    name_element->LinkEndChild(new TiXmlText(name));
    material_element->LinkEndChild(name_element);
  }

  TiXmlElement* category_element = new TiXmlElement(category_tag);
  category_element->LinkEndChild(new TiXmlText(category));
  material_element->LinkEndChild(category_element);

  currentnode->LinkEndChild(material_element);
  currentnode = material_element;
  nodecont.push_back(material_element);
}

void xmlwriter::close_material_element() //wrapper for update(), use with care
{
  this->update();
}

void xmlwriter::open_group_element(const char* id, const char* name, const char* category)
{
  TiXmlElement* group_element = new TiXmlElement(group_tag);

  TiXmlElement* id_element = new TiXmlElement(id_tag);
  id_element->LinkEndChild(new TiXmlText(id));
  group_element->LinkEndChild(id_element);

  if(strlen(name) != 0)
  {
    TiXmlElement* name_element = new TiXmlElement(name_tag);
    name_element->LinkEndChild(new TiXmlText(name));
    group_element->LinkEndChild(name_element);
  }

  if(strlen(category) != 0)
  {
    TiXmlElement* category_element = new TiXmlElement(category_tag);
    category_element->LinkEndChild(new TiXmlText(category));
    group_element->LinkEndChild(category_element);
  }

  currentnode->LinkEndChild(group_element);
  currentnode = group_element;
  nodecont.push_back(group_element);
}

void xmlwriter::close_group_element() //wrapper for update(), use with care
{
  this->update();
}

void xmlwriter::update()
{
  nodecont.pop_back();
  currentnode = nodecont[nodecont.size()-1];
}

void xmlwriter::print(std::string filename)
{
  this->print(filename.c_str());
}

void xmlwriter::print(const char* filename)
{
  doc.SaveFile( filename );
}

void xmlwriter::print_to_console(void)
{
  TiXmlPrinter printer;
  printer.SetIndent("  ");
  doc.Accept(&printer);
  std::cout << printer.CStr() << std::endl;
}
//
// ----------------------------------------------------------
//

TiXmlElement* ipd_value_to_xml(const char* name, ipdTreeNode_t *tn, xmlwriter& xmldoc)
{
  TiXmlElement* attribute_element = 0;

  ipdBoolean ipd_success;

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
      attribute_element = xmldoc.create_scalar(name, val, "");
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
      attribute_element = xmldoc.create_scalar(name, value, "");
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
      attribute_element = xmldoc.create_scalar(name, value, unit);
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
      attribute_element = xmldoc.create_scalar(name, val, "");
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

      if(dimension > 3)
      {
        /*
         * Tensors with dimension greater than 3 are not supported in the ViennaMaterials XML layout.
         * Thus, this tensor is ignored!
         */
  #ifdef VERBOSE_MODE
        std::cout << "INFO: IPD tensor variable ignored (" << name << ")" << std::endl;
  #endif
        attribute_element = 0;
        break;
      }

      double tensor_rows    = 1;
      double tensor_columns = 1;
      double tensor_order   = 1;

      switch(dimension)
      {
        case 1:
          tensor_columns  = length[0];
          break;
        case 2:
          tensor_rows     = length[0];
          tensor_columns  = length[1];
          break;
        case 3:
          tensor_order    = length[0];
          tensor_rows     = length[1];
          tensor_columns  = length[2];
          break;
        default:
          std::string name_str(name);
          throw ipd2xml_error("Invalid dimension value of array/matrix/tensor encountered (" + name_str + ")");
      }

      std::stringstream ss;
      ss << unit;
      attribute_element = xmldoc.create_tensor(name, tensor_rows, tensor_columns, tensor_order, matrix, ss.str().c_str());

      free(length);
      free(matrix);
      break;
    }

    case ipdSTRING:
    case 2080: // concatenated strings ..
    {
      /*
       * Strings are not supported as attributes in the ViennaMaterials XML layout.
       * Thus, string variables in IPD input files are ignored!
       */
#ifdef VERBOSE_MODE
      std::cout << "INFO: IPD string variable ignored (" << name << ")" << std::endl;
#endif
      attribute_element = 0;
      break;
    }

    default:
      std::string type_str("");
      type_str = converter(tn->type);
      throw ipd2xml_error("Not implemented IPD structure type (" + type_str + ") encountered (" + name +")");
      break;
  }




  return attribute_element;
}
//
// ----------------------------------------------------------
//

void recursive_traverse(ipdIterator_t * iNode, xmlwriter& xmldoc)
{
  // Traverse the ViennaIPD datastructure using the iterator
  while (ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    // If the current element is a _variable_
    if (ipdIteratorGetType(iNode) == ipdVARIABLE)
    {
      TiXmlElement* attribute = ipd_value_to_xml(iNode->tn->node.sv.name, ipdIteratorEval(iNode), xmldoc);
      if(attribute != 0)
        xmldoc.add_element(attribute);

    // If the current element is a _section_
    }else if (ipdIteratorGetType(iNode) == ipdSECTION)
    {
      // Create a new iterator which should traverse the subsection
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      xmldoc.open_group_element(itemName, "", "");
      recursive_traverse(iSubNode, xmldoc);
      xmldoc.close_group_element();

      ipdIteratorFree(iSubNode);
    }

    // Next item
    ipdIteratorDoNext(iNode);
  }

}
//
// ----------------------------------------------------------
//

void access_ipd_material(ipdIterator_t * iNode, xmlwriter& xmldoc)
{
  // Traverse the ViennaIPD datastructure using the iterator
  if(ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item, the material
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    if (ipdIteratorGetType(iNode) == ipdSECTION) //IPD material node must be a section
    {
      // Create a new iterator which should traverse the subsection, the attributes of the material
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      //TODO extract 'class' and 'materials' but dont extract them in recursive_traverse
      xmldoc.open_material_element(itemName, "TODO", "TODO"); //FIXME
      recursive_traverse(iSubNode, xmldoc);
      xmldoc.close_material_element();

      ipdIteratorFree(iSubNode);

    }else //TODO: return value, abort iteration for this iterator
    {
      std::string full_name = ipdIteratorGetItemFullName(iNode);
//      throw ipd2xml_error("Invalid IPD layout (" + full_name + ")");
#ifdef VERBOSE_MODE
      std::cout << "INFO: Invalid IPD layout (" + full_name + ")" << std::endl;
#endif
    }
  }else
  {
    throw ipd2xml_error("Invalid IPD iterator passed to access_ipd_material");
  }
}
//
// ----------------------------------------------------------
//

void traverse_ipd_layout(ipdIterator_t * iNode, xmlwriter& xmldoc)
{
  // Traverse the ViennaIPD datastructure using the iterator
  while (ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    if (ipdIteratorGetType(iNode) == ipdSECTION) //must be a section
    {
      // Create a new iterator which should traverse the subsection, the materials
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      while(ipdIteratorIsValid(iSubNode))
      {
        access_ipd_material(iSubNode, xmldoc);
        ipdIteratorDoNext(iSubNode);
      }

      ipdIteratorFree(iSubNode);
    }else
    {
      std::string full_name = ipdIteratorGetItemFullName(iNode);
      throw ipd2xml_error("Invalid IPD layout (" + full_name + ")");
    }

    // Next item
    ipdIteratorDoNext(iNode);
  }
}
