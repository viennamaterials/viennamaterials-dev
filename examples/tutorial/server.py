import sys
import socket
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





# http://pymotw.com/2/socket/tcp.html

host='localhost'
port=2729
buffersize=4096

# Create a TCP/IP socket
mySocket = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )

# Bind the socket to the port
server_address = (host, port)
print "[Server] starting up on %s port %s" % server_address
mySocket.bind(server_address)

# Listen for incoming connections
mySocket.listen(1)

while True:
  # Wait for a connection
  print "[Server] waiting for a connection"
  connection, client_address = mySocket.accept()

  print "[Server] connection from", client_address
  try:
    query = connection.recv(buffersize)
  except socket.error, msg:
    print "[Server][Error] receiving client data"
    connection.close()
    continue

  print '[Server] received query:', query
  try:
    query_result = matlib.query(query)

    # here we would need a string representation to be able to transfer it over the network

    # conceptual problem? the entire 'is attribute' approach, should it be re-implemented for the network?
    # maybe use a native backend here instead of the library?
    # maybe provide a string return overload for the query?
    # -> return the xml return string and evaluate it on the client side? no,
    # or ..
    # define a small client server language package: value, unit, number of dependencies


#    print query_result.evaluate
#    if query_result.is_scalar_bool():
#      connection.sendall( query_result.evaluate_value_bool() )
  except socket.error, msg:
    print "[Server][Error] querying client request"
  finally:
    # Clean up the connection
    connection.close()
