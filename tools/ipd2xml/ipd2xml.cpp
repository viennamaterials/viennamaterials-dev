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
#include <vector>
#include "tools/ipd2xml/ViennaIPD/src/include/ipd.h"
#include "tools/ipd2xml/tinyxml/tinyxml.h"

#include "viennamaterials/xmldatatypes.h"


//#define CONVOUTPUT
//
// ----------------------------------------------------------
//
struct xmlwriter
{
  xmlwriter(const char* docid)
  {
    root_tag             = "database";
//    identifier_tag       = "identifier";
//    element_tag          = "element";
//    data_tag             = "data";
//    representation_tag   = "representation";
//    double_tag           = "double";
    
    id_tag                      = "id";
    group_tag                   = "group";
    attribute_tag               = "attribute";
    value_tag                   = "value";
    scalar_tag                  = "scalar";
    type_attribute_tag   = "type";
    type_boolean         = "bool";
    type_integer         = "int";
    type_floating        = "float";
    tensor_tag                  = "tensor";
    tensor_row_attribute_tag    = "row";
    tensor_column_attribute_tag = "col";
    tensor_order_attribute_tag  = "order";
    unit_tag                    = "unit";

    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * root = new TiXmlElement( root_tag );
    doc.LinkEndChild( root );

    TiXmlElement * rootid = new TiXmlElement( id_tag );
    rootid->LinkEndChild( new TiXmlText( docid )); //XXX
    root->LinkEndChild( rootid );

    currentnode = root;
    nodecont.push_back( root );
  }

  void add_section(const char* tag)
  {
    //std::cout << "# add_section: " << tag << std::endl;

    TiXmlElement * element = new TiXmlElement( group_tag );
    currentnode->LinkEndChild( element );

    TiXmlElement * elementid = new TiXmlElement( id_tag );
    elementid->LinkEndChild( new TiXmlText( tag ));
    element->LinkEndChild( elementid );

    currentnode = element;
    nodecont.push_back( element );
  }

  void add_variable(const char* tag, const char* value)
  {
    //std::cout << "# add_variable: " << tag << " : " << value << std::endl;

    TiXmlElement * data = new TiXmlElement( attribute_tag );
    currentnode->LinkEndChild( data );

    TiXmlElement * dataid = new TiXmlElement( id_tag );
    dataid->LinkEndChild( new TiXmlText( tag ));
    data->LinkEndChild( dataid );

    TiXmlElement * repres = new TiXmlElement( value_tag );
    data->LinkEndChild( repres );

    TiXmlElement * double_ = new TiXmlElement( double_tag );
    double_->LinkEndChild( new TiXmlText( value ));
    repres->LinkEndChild( double_ );
  }

  void add_element(TiXmlElement* element)
  {
    currentnode->LinkEndChild(element);
  }

  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_bool& value, const char* unit)
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

  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_int& value, const char* unit)
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

  TiXmlElement* create_scalar(const char* id, const viennamaterials::xml_float& value, const char* unit)
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

  TiXmlElement* create_tensor(const char* id, const double& tensor_rows, const double& tensor_columns, const double& tensor_order, const double* values, const char* unit)
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

  void update()
  {
    nodecont.pop_back();
    currentnode = nodecont[nodecont.size()-1];
  }

  void print(std::string filename)
  {
    this->print(filename.c_str());
  }

  void print(const char* filename)
  {
    doc.SaveFile( filename );
  }

  void print_to_console(void)
  {
    TiXmlPrinter printer;
    printer.SetIndent("  ");
    doc.Accept(&printer);
    std::cout << printer.CStr() << std::endl;
  }

  TiXmlDocument doc;

  TiXmlElement * currentnode;

  const char* root_tag;
  const char* id_tag;
  const char* group_tag;
  const char* attribute_tag;
  const char* value_tag; //XXX
  const char* double_tag; //XXX
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

  std::vector< TiXmlElement * > nodecont;
};
//
// ----------------------------------------------------------
//
const char* access_ipd_value(const char* name, ipdTreeNode_t *tn)
{
  int l, b, v, len, dim;
  int *rlen;
  double re, im, *matrix;
  char *unit, *str;
  const char* munit;

  str = unit = NULL;

  std::string retval("");

  //std::cout << name
  //          << " - valtype: " << ipdGetValueType(tn)
  //          << " - type: " << ipdGetType(tn) << std::endl;

  switch (tn->type)
  {
    case ipdINTEGER:
      v = ipdGetIntegerByName(name, &l);
#ifdef CONVOUTPUT
      std::cout << "int: " << l << "  vs.  " << converter(l) << std::endl;
#endif
      if(v) retval = converter(l);
      else std::cout << "error @ ipdINTEGER - name: " << name << std::endl;
      break;
    case ipdREAL:
      v = ipdGetRealByName(name, &re);
#ifdef CONVOUTPUT
      std::cout << "real: " << re << "  vs.  " << converter(re) << std::endl;
#endif
      if(v) retval = converter(re);
      else std::cout << "error @ ipdREAL - name: " << name << std::endl;
      break;
    case ipdCOMPLEX:
      v = ipdGetComplexByName(name, &re, &im);
#ifdef CONVOUTPUT
      std::cout << "complex: " << re << " " << im << "  vs.  " << converter(re) << " " << converter(im) << std::endl;
#endif
      if(v) retval = converter(re) + " i" + converter(im);
      else std::cout << "error @ ipdCOMPLEX - name: " << name << std::endl;
      break;
    case ipdREALQUANTITY:
      v = ipdGetRealQuantityByName(name, &re, &unit);
#ifdef CONVOUTPUT
      std::cout << "real quan: " << re << " " << unit << "  vs.  " << converter(re) << " " << converter(unit) << std::endl;
#endif
      if(v && (unit)) retval = converter(re) + " " + converter(unit);
      else std::cout << "error @ ipdREALQUANTITY - name: " << name << std::endl;
      break;
    case ipdCOMPLEXQUANTITY:
      v = ipdGetComplexQuantityByName(name, &re, &im, &unit);
#ifdef CONVOUTPUT
      std::cout << "real comp quan: " << re << " " << im << " " << unit << "  vs.  " << converter(re) << " " << converter(im) << " " << converter(unit) << std::endl;
#endif
      if(v) retval = converter(re) + " i" + converter(im) + " " + converter(unit);
      else std::cout << "error @ ipdCOMPLEXQUANTITY - name: " << name << std::endl;
      break;
    case ipdSTRING:
      v = ipdGetStringByName(name, &str);
#ifdef CONVOUTPUT
      std::cout << "string: " << str << "  vs.  " << converter(str) << std::endl;
#endif
      if(v)
      {
        if(str)
          retval = converter(str);
      }
      else std::cout << "error @ ipdSTRING - name: " << name << std::endl;
      break;
    case ipdBOOLEAN:
      v = ipdGetBooleanByName(name, &b);
#ifdef CONVOUTPUT
      std::cout << "bool: " << b << "  vs.  " << converter(b) << std::endl;
#endif
      if(v) retval = converter(b);
      else std::cout << "error @ ipdBOOLEAN - name: " << name << std::endl;
      break;

    case 192: // array
      v = ipdGetSloppyRealMatrixByName(name, &dim, &rlen, &matrix, &munit);
      if(v)
      {
        len = 1;
        int i;
        for (i = 0; i < dim; i++)
          len = len * rlen[i];
        retval = "[ ";
        for (i = 0; i < len; i++)
          retval += converter(matrix[i]) + " ";
        std::stringstream ss;
        ss << munit;
        retval += "] " + ss.str();

        //free(rlen);
        free(matrix);
      }
      else std::cout << "error @ 192 - name: " << name << std::endl;
      break;

    case 2080: // concatenated strings ..
      //std::cout << "name-2080: " << name << " type: " << tn->type << std::endl;
      v = ipdGetStringByName(name, &str);
      if(v && (str)) retval = converter(str);
      else std::cout << "error @ 2080 - name: " << name << std::endl;
      break;

    case 4098: // rationals .. will be evaluated to floats
      //std::cout << "name-4098: " << name << " type: " << tn->type << std::endl;
      v = ipdGetRealByName(name, &re);
      if(v) retval = converter(re);
      else std::cout << "error @ 4098 - name: " << name << std::endl;
      break;

    case 4106: //
      //std::cout << "name-4106: " << name << " type: " << tn->type << std::endl;
      v = ipdGetRealQuantityByName(name, &re, &unit);
      if(v && (unit)) retval = converter(re) + " " + converter(unit);
      else std::cout << "error @ 4106 - name: " << name << std::endl;
      break;

    default:

      std::cout << "error @ ipd db access - node type not recognized: "<< name
      << " - type: " << ipdGetType(tn) << std::endl;
      retval = "not-accessible";
      break;
  }

  return retval.c_str();
}
//
// ----------------------------------------------------------
//
void recursive_traverse(ipdIterator_t * iNode, xmlwriter & xmldoc)
{
  // Traverse the ViennaIPD datastructure using the iterator
  while (ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    // If the current element is a _variable_
    if (ipdIteratorGetType(iNode) == ipdVARIABLE)
    {
      // Print the name of the variable
      //printf("variable: %s\n", itemName);

      std::string valuestring("");

      valuestring = access_ipd_value(iNode->tn->node.sv.name, ipdIteratorEval(iNode));

      xmldoc.add_variable( itemName, valuestring.c_str() );
    }

    // If the current element is a _section_
    else if (ipdIteratorGetType(iNode) == ipdSECTION)
    {
      xmldoc.add_section( itemName );

      // Print the name of the section
      //printf("section: %s\n", itemName);

      // Create a new iterator which should traverse the subsection
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      recursive_traverse(iSubNode, xmldoc);

      xmldoc.update();

    }

    // Next item
    ipdIteratorDoNext(iNode);

  }
}

/*
 * ############################################################################
 * ############################################################################
 * ############################################################################
 */

//
// ----------------------------------------------------------
//
/*
 * @brief This method performs the transformation from IPD data structure to ViennaMaterials XML data layout
 * @param name The name of the value to be accessed
 * @param tn IPD tree node structure containing the evaluated value
 * @param xmldoc A reference to an object of type xmlwriter
 * @return XML element object containing the value in the ViennaMaterials XML layout
 */
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
    {
      ipdDouble value;
      ipd_success = ipdGetRealByName(name, &value);
      if(ipd_success == ipdFALSE)
      {
        std::string name_str(name);
        throw ipd2xml_error("Error while accessing real value(" + name_str + ")");
      }
      attribute_element = xmldoc.create_scalar(name, value, "");
      break;
    }

    case ipdREALQUANTITY:
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

      std::cout << "dimension: " << dimension << std::endl; //XXX
      for(long i = 0; i < dimension; i++)
        std::cout << "length: " << length[i] << std::endl; //XXX

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

      std::cout << "row: " << tensor_rows << " cols: " << tensor_columns << " order: " << tensor_order << std::endl; //XXX

      std::stringstream ss;
      ss << unit;
      attribute_element = xmldoc.create_tensor(name, tensor_rows, tensor_columns, tensor_order, matrix, ss.str().c_str());

      free(length);
      free(matrix);
      break;
    }

    case ipdSTRING:
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
      throw ipd2xml_error("Not implemented IPD structure type encountered (" + type_str + ")");
      break;
  }




  return attribute_element;
}

//
// ----------------------------------------------------------
//

static const long intend_spacing = 2;

void print_ipd_tree_recursive_traverse(ipdIterator_t * iNode, long indent, xmlwriter & xmldoc)
{
  std::string indention("");
  for(long i = 0; i < indent; i++)
    indention.append(" ");

  // Traverse the ViennaIPD datastructure using the iterator
  while (ipdIteratorIsValid(iNode))
  {
    // Get the name of the current item
    ipdConstString itemName = ipdIteratorGetItemName(iNode);

    // If the current element is a _variable_
    if (ipdIteratorGetType(iNode) == ipdVARIABLE)
    {
      std::cout << "(" << indent/intend_spacing << ")" << indention << "V  " << itemName << ": " <<
        access_ipd_value(iNode->tn->node.sv.name, ipdIteratorEval(iNode)) << std::endl;

      TiXmlElement* attribute = ipd_value_to_xml(iNode->tn->node.sv.name, ipdIteratorEval(iNode), xmldoc);
      if(attribute != 0)
        xmldoc.add_element(attribute);
    // If the current element is a _section_
    }else if (ipdIteratorGetType(iNode) == ipdSECTION)
    {
      std::cout << "(" << indent/intend_spacing << ")" << indention << "S  " << itemName << std::endl;

      // Create a new iterator which should traverse the subsection
      ipdIterator_t  *iSubNode = NULL;

      // Set the iterator to origin at this particular section
      ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);

      // Step into the subsection
      ipdIteratorDoStep(iSubNode);

      print_ipd_tree_recursive_traverse(iSubNode, indent+intend_spacing, xmldoc);
    }

    // Next item
    ipdIteratorDoNext(iNode);
  }
}
//
// ----------------------------------------------------------
//
void print_ipd_tree(void)
{
  // Define and initialize a ViennaIPD iterator
  ipdIterator_t  *iNode = NULL;

  // Create a new iterator which operates on the root level
  ipdIteratorNewAtRootSection(&iNode, ipdANY);

  ipdIteratorDoStep(iNode);

  xmlwriter xmldoc("modelipd");
  print_ipd_tree_recursive_traverse(iNode, 0, xmldoc);

  // Free the iterator
  ipdIteratorFree(iNode);


  xmldoc.print_to_console();
}
//
// ----------------------------------------------------------
//
void do_stuff(void)
{
  print_ipd_tree();
}
//
// ----------------------------------------------------------
//

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

  xmlwriter xmldoc("modelipd");

  // Initialize the Database.
  // Normally we can use the default values, so we don't need parameters.
  ipdInit(NULL, NULL);

  // Let's create a database
  ipdCreateBase("NameOfTheDatabase", 0);

  // Read a file in the database
  ipdReadInputDeck(inputfile_ipd);

#if 0

  // Define and initialize a ViennaIPD iterator
  ipdIterator_t  *iNode = NULL;

  // Create a new iterator which operates on the root level
  ipdIteratorNewAtRootSection(&iNode, ipdANY);

  ipdIteratorDoStep(iNode);

  recursive_traverse(iNode, xmldoc);

  // Free the iterator
  ipdIteratorFree(iNode);
#endif

  do_stuff();

  // Free the ViennaIPD datastructures
  ipdFreeAll();

//  xmldoc.print(outputfile_xml); //FIXME

  return 0;
}


