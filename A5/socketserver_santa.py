
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

                # If the message has an additional payload, then separate the variables
        if b'-' in msg:
            body = msg[msg.index(b'-')+1:]
            msg = msg[:msg.index(b'-')]

        # This message will be sent by each reindeer in turn, as they finish 
        # their holiday
        if msg == MSG_HOLIDAY_OVER:
            # Part of the message will be the address of the reindeer, separate
            # the two parts
            reindeer_host = body[:body.index(b':')].decode()
            reindeer_port = int(body[body.index(b':')+1:].decode())

            # Append them to a list of collected reindeer addresses
            self.server.reindeer_counter.append((reindeer_host, reindeer_port))


            # If we've collected all reindeer addresses, then tell them all 
            # that we can deliver
            if len(self.server.reindeer_counter) == self.server.num_reindeer:
                # Deliver presents                
                print(f"Santa is delivering presents with all {self.server.num_reindeer} the reindeer")
                # Tell each reindeer to deliver
                for host, port in self.server.reindeer_counter:
                    sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    sending_socket.connect((host, port))
                    sending_socket.sendall(MSG_DELIVER_PRESENTS)
                    sending_socket.close()
                # Reset the reindeer address collection
                self.server.reindeer_counter = []

        # This message will be sent by any elves that encounter a problem
        elif msg == MSG_PROBLEM:
            
            elf_host = body[:body.index(b':')].decode()
            elf_port = int(body[body.index(b':')+1:].decode())

            # Append to elf list
            self.server.elf_counter.append((elf_host, elf_port))

            if len(self.server.elf_counter) == self.server.elf_group:
                # fucking elverproblemer
                print(f"Santa is dealing with all {len(self.server.elf_counter)} elf problems")

                # Fix every problem
                for host, port in self.server.elf_counter:
                    sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    sending_socket.connect((host, port))
                    sending_socket.sendall(MSG_SORT_PROBLEM)
                    sending_socket.close()
                
                # Reset elf counter
                self.server.elf_counter = []                

        # If we get something we didn't expect then abort
        else:
            print(f"Santa recieved an unknown instruction")
            exit()

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