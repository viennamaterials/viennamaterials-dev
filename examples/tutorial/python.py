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
matlib = pyviennamaterials.generator(matfile)

# now you can use this object to run native XPath queries
print "matlib string query:  ", matlib.query("/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()")
print "matlib numeric query: ", matlib.query_value("/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()")

# simplify the syntax by using a proxy, in this case a viennastar_proxy
proxy = pyviennamaterials.viennastar_proxy(matlib)

# the proxy allows to simplify the query syntax by assuming a specific
# layout, the 'viennastar' layout
print "proxy string query:  ", proxy.query("Si/bandgap")
print "proxy numeric query: ", proxy.query_value("Si/bandgap")
print "proxy unit query: "   , proxy.query_unit("Si/bandgap")
quan = proxy.query_quantity("Si/bandgap")
print "proxy quantity query: ", quan

# you can convert values/quantities to new compatible units
# for instance, convert the bandgab energy in 'eV' to 'J'
units_converter = pyviennamaterials.udunits(unitfile)
units_converter.convert(quan, "J")
print "converted quantity: ", quan
