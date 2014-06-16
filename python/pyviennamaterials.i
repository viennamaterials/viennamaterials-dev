
%include <std_string.i>
%include <std_vector.i>

%module pyviennamaterials
%{
  #include "viennamaterials/forwards.h"
  #include "viennamaterials/utils/shared_ptr.hpp"
  #include "viennamaterials/library.hpp"
  #include "viennamaterials/pugixml.hpp"
  #include "viennamaterials/proxy.hpp"
  #include "viennamaterials/proxy/viennastar.hpp"
  #include "viennamaterials/generator.hpp"
  #include "viennamaterials/utils/convert.hpp"
  #include "viennamaterials/utils/udunits.hpp"
%}

%include "viennamaterials/forwards.h"
%include "viennamaterials/utils/shared_ptr.hpp"
%include "viennamaterials/library.hpp"
%include "viennamaterials/pugixml.hpp"
%include "viennamaterials/proxy.hpp"
%include "viennamaterials/proxy/viennastar.hpp"
%include "viennamaterials/generator.hpp"
%include "viennamaterials/utils/convert.hpp"

#define VIENNAMATERIALS_WITH_UNITS
%include "viennamaterials/utils/udunits.hpp"

%template(libraryHandle)  viennamaterials::shared_ptr<viennamaterials::library>;

namespace viennamaterials
{
  struct quantity
  {
    // enable the quantity class to be 'printable' within python ..
    %extend {
      char * __str__()
      {
        return const_cast<char*>(std::string( viennamaterials::convert<std::string>($self->value())+" "+$self->unit() ).c_str());
      }
    }
  };
}
