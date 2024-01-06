
import sys
import socketserver

from shared import *

# A handler function, called on each incoming message to the server
class SantaHandler(socketserver.StreamRequestHandler):
    def handle(self):
        # Read the message
        msg = self.request.recv(MAX_MSG_LEN)

        # If santa is told to deliver presents, then deliver he shall
        if msg == MSG_DELIVER_PRESENTS:
            print(f"Santa is delivering presents")
        # If he's told to sort problems with elves, then he'll do that 
        elif msg == MSG_SORT_PROBLEM:
            print(f"Santa is sorting problems")

        # Checkin function will 'check in' with a checkin process, if one is 
        # available. This can be removed if you are confident in your answer 
        # and want to avoid the slowdown it adds
        checkin(f"Santa")

# A socketserver class to run santa as a constant server
class SantaServer(socketserver.ThreadingTCPServer):
    # Constructor for our custom class. If we wish to add more variables or
    # arguments this is where we could do it
    def __init__(self, server_address, request_handler_class):
        # Call the parent classes constructor
        super().__init__(server_address, request_handler_class)

# Base santa function, to be called as a process
def santa(host, port):
    # Start a socketserver to always be listening
    with SantaServer((host, port), SantaHandler) as santa_server:
        try:
            # Always be able to handle incoming messages
            santa_server.serve_forever()
        finally:
            # If we keyboard interupt this will wrap up all the backend stuff
            santa_server.server_close()

# As an alternative to using the true_santa_problem.py, you may start a 
# standalone santa as described in the handout
if __name__ == "__main__":
    my_host = sys.argv[1]
    my_port = int(sys.argv[2])
    num_reindeer = int(sys.argv[3])
    elf_group = int(sys.argv[4])
    process = Process(target=santa, args=(my_host, my_port, num_reindeer, elf_group))
    process.start()
