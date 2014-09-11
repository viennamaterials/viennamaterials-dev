import sys
import pyviennamaterials

# check whether the correct number of parameters have been provided
if len(sys.argv) != 3:
  print "Error - Usage:", sys.argv[0], " input-material-file.xml unit-database.xml"
  print "Example:      ", sys.argv[0], " data/material_test.xml ../units/units.xml"
  print "Aborting .."
  exit(-1)

# get the input files from the input parameters
matfile  = str(sys.argv[1])
unitfile = str(sys.argv[2])

# create a material library by providing the input material file
matlib = pyviennamaterials.library(matfile)

# ------------------------------------------------------------------------------
# query scalar values
#
value = matlib.query("/*/*[id='test-material']/*[id='bool-scalar']")
if value.is_scalar_bool() :
  print "boolean scalar value: " , value.evaluate_value_bool()

value = matlib.query("/*/*[id='test-material']/*[id='int-scalar']")
if value.is_scalar_int() :
  print "int scalar value: " , value.evaluate_value_integer()

value = matlib.query("/*/*[id='test-material']/*[id='float-scalar']")
if value.is_scalar_float() :
  print "float scalar value: " , value.evaluate_value_float()

# ------------------------------------------------------------------------------
# query functions returning a scalar value
#
bool_function_attribute = matlib.query("/*/*[id='test-material']/*[id='bool-function-without-args']")
if bool_function_attribute.is_function_bool() :
  print "bool function value: " , bool_function_attribute.evaluate_value_bool()

int_function_attribute = matlib.query("/*/*[id='test-material']/*[id='int-function-add']")
if int_function_attribute.is_function_int() :
  print "int function value: " , int_function_attribute.evaluate_value_integer()
  func_args = int_function_attribute.get_dependencies()
  func_args[0].set_value(8)
  int_function_attribute.set_dependencies(func_args)
  print "int function value new: " , int_function_attribute.evaluate_value_integer()


float_function_attribute = matlib.query("/*/*[id='test-material']/*[id='float-function-multiply']")
if float_function_attribute.is_function_float() :
  print "float function value: " , float_function_attribute.evaluate_value_float()
  func_args = float_function_attribute.get_dependencies()
  func_args[1].set_value(0.125)
  float_function_attribute.set_dependencies(func_args)
  print "float function value new: " , float_function_attribute.evaluate_value_float()

float_function_ref_attribute = matlib.query("/*/*[id='test-material']/*[id='float-function-references']")
if float_function_ref_attribute.is_function_float() :
  print "float function value reference: " , float_function_ref_attribute.evaluate_value_float()



# ------------------------------------------------------------------------------
# query quantities holding a scalar value
#
quantity = matlib.query("/*/*[id='test-material']/*[id='int-scalar']")
if quantity.is_scalar_int() :
  print "int scalar value: ", quantity.evaluate_integer()
  quan = quantity.evaluate_integer()
  print "  quantity-value: ", quan.value()
  print "  quantity-unit:  ", quan.unit()
