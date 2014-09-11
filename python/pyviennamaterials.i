
%include <std_string.i>
%include <std_vector.i>


%module pyviennamaterials
%{
  #define SWIG_PYTHON_EXTRA_NATIVE_CONTAINERS

  #include "viennamaterials/forwards.h"
  #include "viennamaterials/attributeentity.hpp"
  #include "viennamaterials/attributeentityfunction.hpp"
  #include "viennamaterials/attributeentityscalar.hpp"
  #include "viennamaterials/backend.hpp"
  #include "viennamaterials/check.hpp"
  #include "viennamaterials/exceptions.hpp"
  #include "viennamaterials/functionbackend.hpp"
  #include "viennamaterials/functionbackendpython.hpp"
  #include "viennamaterials/generator.hpp"
  #include "viennamaterials/library.hpp"
  #include "viennamaterials/proxy.hpp"
  #include "viennamaterials/pugixml.hpp"
  #include "viennamaterials/quantity.hpp"
  #include "viennamaterials/xmldatatypes.h"
  #include "viennamaterials/xmlvalueentity.hpp"
  #include "viennamaterials/xmlvaluescalar.hpp"
  #include "viennamaterials/proxy/viennastar.hpp"
  #include "viennamaterials/utils/convert.hpp"
  #include "viennamaterials/utils/file_extension.hpp"
  #include "viennamaterials/utils/shared_ptr.hpp"
  #include "viennamaterials/utils/string.hpp"
  #include "viennamaterials/utils/udunits.hpp"
%}

#define VIENNAMATERIALS_WITH_UNITS
#define VIENNAMATERIALS_WITH_PYTHON

%include "viennamaterials/forwards.h"
%include "viennamaterials/library.hpp"
%include "viennamaterials/utils/shared_ptr.hpp"
%include "viennamaterials/quantity.hpp"

%include "viennamaterials/xmldatatypes.h"
%include "viennamaterials/xmlvalueentity.hpp"
%include "viennamaterials/xmlvaluescalar.hpp"
%include "viennamaterials/attributeentity.hpp"
%include "viennamaterials/attributeentityscalar.hpp"
%include "viennamaterials/functionbackend.hpp"
%include "viennamaterials/attributeentityfunction.hpp"

%template(attributeHandle) viennamaterials::shared_ptr<viennamaterials::attribute_entity>;

%template(dependencies)  std::vector<  viennamaterials::shared_ptr< viennamaterials::xml_value_entity > >;
%template(xmlValueEntity) viennamaterials::shared_ptr< viennamaterials::xml_value_entity >;

%template(evaluate_value_bool)    viennamaterials::attribute_entity::evaluate_value<bool>;
%template(evaluate_value_integer) viennamaterials::attribute_entity::evaluate_value<long>;
%template(evaluate_value_float)   viennamaterials::attribute_entity::evaluate_value<double>;

%template(evaluate_bool)    viennamaterials::attribute_entity::evaluate<bool>;
%template(evaluate_integer) viennamaterials::attribute_entity::evaluate<long>;
%template(evaluate_float)   viennamaterials::attribute_entity::evaluate<double>;

namespace viennamaterials
{
  %extend quantity<bool>
  {
    const char* __str__()
    {
      return const_cast<char*>(std::string( viennamaterials::convert<std::string>($self->value())+" "+$self->unit() ).c_str());
    }
  }

  %extend quantity<long>
  {
    const char* __str__()
    {
      return const_cast<char*>(std::string( viennamaterials::convert<std::string>($self->value())+" "+$self->unit() ).c_str());
    }
  }

  %extend quantity<double>
  {
    const char* __str__()
    {
      return const_cast<char*>(std::string( viennamaterials::convert<std::string>($self->value())+" "+$self->unit() ).c_str());
    }
  }
}

%template(quantity_bool)    viennamaterials::quantity<bool>;
%template(quantity_integer) viennamaterials::quantity<long>;
%template(quantity_float)   viennamaterials::quantity<double>;
