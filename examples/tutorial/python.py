import sys
sys.path.insert(0, "../../build/python/")
import pyviennamaterials


matfile = "../../examples/data/test.xml"

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

units_converter = pyviennamaterials.udunits("../../units/units.xml")
units_converter.convert(quan, "J")
print "converted quantity: ", quan
