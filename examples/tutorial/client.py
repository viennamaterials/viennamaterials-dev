import sys
import socket

mySocket = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )

host='localhost'
port=2730
buffersize=4096

try:
  mySocket.connect ( ( host, port ) )
except socket.error, msg:
  print "[Client][Error] server is not reachable:",host,":",port
  sys.exit(-1)

query='/*/*[id=\'test-material\']/*[id=\'bool-scalar\']'

mySocket.sendall(query)

result = mySocket.recv(buffersize)
print "[Client] received answer:", result

#attribute = generate_attribute(result)
