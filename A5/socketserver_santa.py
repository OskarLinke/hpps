
import socket
import sys
import socketserver

from shared import *
from threading import Lock

# A handler function, called on each incoming message to the server
class SantaHandler(socketserver.StreamRequestHandler):
    def handle(self):
        # Read the message
        msg = self.request.recv(MAX_MSG_LEN)

        # TODO You must implement a handler function that can handle mutiple 
        # different messages at the same time. This might be heavily 'inspired'
        # by naive_santa.py, but must also account for any concurrency issues 
        # (races and deadlock) that arise from this

        # Checkin function will 'check in' with a checkin process, if one is 
        # available. This can be removed if you are confident in your answer 
        # and want to avoid the slowdown it adds
        checkin(f"Santa")

# A socketserver class to run santa as a constant server
class SantaServer(socketserver.ThreadingTCPServer):
    # Constructor for our custom class. If we wish to add more variables or
    # arguments this is where we could do it
    def __init__(self, server_address, num_reindeer, elf_group, request_handler_class):
        # Call the parent classes constructor
        super().__init__(server_address, request_handler_class)
        # Record the expected numbers of reindeer and elves
        self.num_reindeer = num_reindeer
        self.elf_group = elf_group
        # Setup the lists for collecting reindeer and elf addresses
        self.reindeer_counter = []
        self.elf_counter = []

# Base santa function, to be called as a process
def santa(host, port, num_reindeer, elf_group):
    with SantaServer((host, port), num_reindeer, elf_group, SantaHandler) as santa_server:
        try:
            # Always be able to handle incoming messages
            santa_server.serve_forever()
        finally:
            # If we keyboard interupt this will wrap up all the backend stuff
            santa_server.server_close()

# As an alternative to using the socketserver_santa_problem.py, you may start a 
# standalone santa as described in the handout
if __name__ == "__main__":
    my_host = sys.argv[1]
    my_port = int(sys.argv[2])
    num_reindeer = int(sys.argv[3])
    elf_group = int(sys.argv[4])
    process = Process(target=santa, args=(my_host, my_port, num_reindeer, elf_group))
    process.start()