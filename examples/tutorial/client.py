import sys
import socket
import pyviennamaterials
import csv

mySocket = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )

host='localhost'
port=2730
buffersize=4096

try:
  mySocket.connect ( ( host, port ) )
except socket.error, msg:
  print "[Client][Error] server is not reachable:",host,":",port
  sys.exit(-1)

query='/*/*[id=\'Ge\']/*[id=\'mu_L_n\']'

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

# Now, use our standalone model retrieved from the server to generate a
# figure by sweeping through a range of values and evaluate the model for each
# single one of them

data_table = list()
for T in range(100,510,10):
  func_args[0].set_value(float(T))
  attribute.set_dependencies(func_args)
  data_table.append([T, attribute.evaluate_value_float()])
#print data_table

#write the data table to a CSV file
with open('output.csv', 'wb') as csvfile:
  writer = csv.writer(csvfile, delimiter=' ')
  for row in data_table:
    writer.writerow(row)
