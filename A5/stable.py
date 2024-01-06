import socket
import sys
import socketserver

from shared import *
from threading import Lock

# A handler function, called on each incoming message to the server
class StableHandler(socketserver.StreamRequestHandler):
    def handle(self):

        # TODO You must decide how to implement the stable handler, so that it 
        # can recieve all reindeer messages that their holiday is done, and 
        # then inform the last reindeer that it must inform the others and 
        # santa that it is time to deliver presents       
        pass

# A socketserver class to run the stable as a constant server
class StableServer(socketserver.ThreadingTCPServer):
    # Constructor for our custom class. If we wish to add more variables or
    # arguments this is where we could do it
    def __init__(self, server_address, num_reindeer, santa_host, santa_port, request_handler_class):
        # Call the parent classes constructor
        super().__init__(server_address, request_handler_class)
        # Record the expected number of reindeer, and santas address
        self.num_reindeer = num_reindeer
        self.santa_host = santa_host
        self.santa_port = santa_port

        # TODO you must decide on any additional variables to set up here
 
# Base stable function, to be called as a process
def stable(my_host, my_port, santa_host, santa_port, num_reindeer):
    # Start a socketserver to always be listening
    with StableServer((my_host, my_port), num_reindeer, santa_host, santa_port, StableHandler) as stable_server:
        try:
            # Always be able to handle incoming messages
            stable_server.serve_forever()
        finally:
            # If we keyboard interupt this will wrap up all the backend stuff
            stable_server.server_close()

# As an alternative to using the socketserver_santa_problem.py, you may start a 
# standalone elf as described in the handout
if __name__ == "__main__":
    process = Process(target=stable, args=(sys.argv[1:]))
    process.start()