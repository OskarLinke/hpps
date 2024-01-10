import socket
import sys
import socketserver

from shared import *
from threading import Lock

# A handler function, called on each incoming message to the server
class StableHandler(socketserver.StreamRequestHandler):
    def handle(self):    

        msg = self.request.recv(MAX_MSG_LEN)
        if b'-' in msg:
            body = msg[msg.index(b'-')+1:]
            msg = msg[:msg.index(b'-')]
        


        if msg == MSG_HOLIDAY_OVER:
            # Part of the message will be the address of the reindeer, separate
            # the two parts
            reindeer_host = body[:body.index(b':')].decode()
            reindeer_port = int(body[body.index(b':')+1:].decode())

            # Append them to a list of collected reindeer addresses
            self.server.lock.acquire()
            self.server.reindeer_counter.append((reindeer_host, reindeer_port))

            if len(self.server.reindeer_counter) == self.server.num_reindeer:
                host, port = self.server.reindeer_counter[self.server.num_reindeer -1]
                sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sending_socket.connect((host, port))
                msg = bytearray(MSG_HOLIDAY_OVER)
                msg.extend(f"-{self.server.santa_host}:{self.server.santa_port}".encode())
                sending_socket.sendall(msg)
                sending_socket.close()

                for host, port in self.server.reindeer_counter:
                    sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    sending_socket.connect((host, port))
                    sending_socket.sendall(MSG_DELIVER_PRESENTS)
                    sending_socket.close()
                    # Reset the reindeer address collection
                self.server.reindeer_counter = []
                self.server.lock.release()
            else: 
                self.server.lock.release()  


# A socketserver class to run the stable as a constant server
class StableServer(socketserver.ThreadingTCPServer):
    # Constructor for our custom class. If we wish to add more variables or
    # arguments this is where we could do it
    def __init__(self, server_address, num_reindeer, santa_host, santa_port, request_handler_class):
        # Call the parent classes constructor
        super().__init__(server_address, request_handler_class)
        # Record the expected number of reindeer, and santas address
        self.num_reindeer = num_reindeer
        self.reindeer_counter = []
        self.santa_host = santa_host
        self.santa_port = santa_port

        self.lock = Lock()

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