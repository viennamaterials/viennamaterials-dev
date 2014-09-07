#ifdef VIENNAMATERIALS_WITH_UNITS

/* =============================================================================
   Copyright (c) 2013-2014, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at/
                             -----------------
            ViennaMaterials - The Vienna Materials Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include "viennamaterials/utils/udunits.hpp"

namespace viennamaterials {

  udunits::udunits(std::string const& filename)
  {
    unit_system_ = ut_read_xml(filename.c_str());
    ut_status stat = ut_get_status();
    if(stat == UT_OPEN_ARG)   throw udunits_exception("Could not open unit database!");
    else if(stat == UT_OS)    throw udunits_exception("Unit database encountered operating system error!");
    else if(stat == UT_PARSE) throw udunits_exception("Unit database could not process unit XML file!");

    ut_set_error_message_handler(&ut_ignore);
  }

  udunits::~udunits()
  {
    ut_free_system(unit_system_);
  }

  void udunits::convert(viennamaterials::numeric& value, std::string const& source_unit, std::string const& target_unit)
  {
    cv_converter* converter = this->get_converter(source_unit, target_unit);
    value = cv_convert_double(converter, value);
    cv_free(converter);
  }

  template<>
  void udunits::convert(viennamaterials::quantity<xml_float>& quan, std::string const& target_unit)
  {
    this->convert(quan.value(), quan.unit(), target_unit);
    quan.unit() = target_unit;
  }

  cv_converter* udunits::get_converter(std::string const& source_unit, std::string const& target_unit)
  {
    ut_unit* udunit_source = ut_parse(unit_system_, source_unit.c_str(), UT_UTF8);
    if(!udunit_source) investigate_parse_error(source_unit);

    ut_unit* udunit_target = ut_parse(unit_system_, target_unit.c_str(), UT_UTF8);
    if(!udunit_target) investigate_parse_error(target_unit);

    cv_converter* converter = ut_get_converter(udunit_source, udunit_target);
    if(!converter) investigate_conversion_error(source_unit, target_unit);
    return converter;
  }

  void udunits::investigate_parse_error(std::string const& unit)
  {
    std::string err_msg = "Error parsing unit \""+unit+"\": ";
    ut_status status = ut_get_status();
    if(status == UT_BAD_ARG) throw udunits_exception(err_msg+"Unit or system is NULL!");
    else
    if(status == UT_SYNTAX)  throw udunits_exception(err_msg+"Unit string contains a syntax error!");
    else
    if(status == UT_UNKNOWN) throw udunits_exception(err_msg+"Unit string contains an unknown identifier!");
    else                     throw udunits_exception(err_msg+"An unknown error occured!");
  }

  void udunits::investigate_conversion_error(std::string const& source_unit, std::string const& target_unit)
  {
    std::string err_msg = "Error converting from \""+source_unit+"\" to \""+target_unit+"\": ";
    ut_status status = ut_get_status();
    if(status == UT_BAD_ARG) throw udunits_exception(err_msg+"One of the units is NULL!");
    else
    if(status == UT_NOT_SAME_SYSTEM)  throw udunits_exception(err_msg+"The units are from different unit systems!");
    else
    if(status == UT_SUCCESS) throw udunits_exception(err_msg+"Units cannot be converted!");
    else
    if(status == UT_MEANINGLESS) throw udunits_exception(err_msg+"Units cannot be converted!");
    else                     throw udunits_exception(err_msg+"An unknown error occured!");
  }

} // viennamaterials

#endif
