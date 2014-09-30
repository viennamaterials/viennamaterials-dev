import sys
import socket
import pyviennamaterials

mySocket = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )

host='localhost'
port=2730
buffersize=4096

try:
  mySocket.connect ( ( host, port ) )
except socket.error, msg:
  print "[Client][Error] server is not reachable:",host,":",port
  sys.exit(-1)

query='/*/*[id=\'Si\']/*[id=\'mu_L_n\']'

mySocket.sendall(query)

result = mySocket.recv(buffersize)
print "[Client] received string-based answer:"
print result

print "Converting to attribute .."
attribute = pyviennamaterials.generate_attribute(result)
print "float function value: " , attribute.evaluate_value_float()
func_args = attribute.get_dependencies()
func_args[0].set_value(400.)
attribute.set_dependencies(func_args)
print "float function value new: " , attribute.evaluate_value_float()
