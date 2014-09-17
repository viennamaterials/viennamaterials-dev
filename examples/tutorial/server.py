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
port=2730
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
    connection.sendall( matlib.query_to_string(query) )
  except socket.error, msg:
    print "[Server][Error] querying client request"
  finally:
    # Clean up the connection
    connection.close()
