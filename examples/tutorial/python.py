import sys
import pyviennamaterials

if len(sys.argv) != 3:
  print "Error - Usage:", sys.argv[0], " input-material-file.xml unit-database.xml"
  print "Aborting .."
  exit(-1)

matfile  = str(sys.argv[1])
unitfile = str(sys.argv[2])

pugi = pyviennamaterials.pugixml(matfile)
print "pugi string query:  ", pugi.query("/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()")
print "pugi numeric query: ", pugi.query_value("/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()")

matlib = pyviennamaterials.generator(matfile)
print "matlib string query:  ", pugi.query("/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()")
print "matlib numeric query: ", pugi.query_value("/material[id=\"Si\"]/parameter[id=\"bandgap\"]/value/text()")

proxy = pyviennamaterials.viennastar_proxy(matlib)
print "proxy string query:  ", proxy.query("Si/bandgap")
print "proxy numeric query: ", proxy.query_value("Si/bandgap")
print "proxy unit query: "   , proxy.query_unit("Si/bandgap")
quan = proxy.query_quantity("Si/bandgap")
print "proxy quantity query: ", quan

units_converter = pyviennamaterials.udunits(unitfile)
units_converter.convert(quan, "J")
print "converted quantity: ", quan
