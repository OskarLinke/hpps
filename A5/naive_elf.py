
import random
import socket
import sys
import time

from shared import *

# All elves will build presents (sleep) for a random amount of time. When they 
# encounter a problem (wake) they will inform santa that they have done so
def build_presents(me, my_host, my_port, santa_host, santa_port):
    # Start work
    print(f"Elf {me} has started building presents")
    time.sleep(random.randint(MIN_BUILD, MAX_BUILD))
    # Encountered a problem
    print(f"Elf {me} has encountered a problem and needs help")

    # Assemble a message including my own network address, so santa can respond
    msg = bytearray(MSG_PROBLEM)
    msg.extend(f"-{my_host}:{my_port}".encode())

    # Send the message to santa
    sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sending_socket.connect((santa_host, santa_port))
    sending_socket.sendall(msg)
    sending_socket.close()

# Once a problem has been encounted, wait for a reply
def wait_for_reply(me, listening_socket):
    # Wait on the listening port to get a connection request
    connection, _ = listening_socket.accept()

    msg = b''
    while msg != MSG_SORT_PROBLEM: 
        # Read from the connection
        msg = connection.recv(MAX_MSG_LEN)
        # If we get something we didn't expect then abort
        if msg != MSG_SORT_PROBLEM:
            print(f"Elf {me} recieved an unknown instruction")
            exit()

# Base elf function, to be called as a process
def elf(me, my_host, my_port, santa_host, santa_port):
    print(f"starting elf with {me}, {my_host}, {my_port}, {santa_host}, {santa_port}")
    # Open a listening socket. Doing so early means its definately open before 
    # we might need it
    listening_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listening_socket.bind((my_host, my_port))
    listening_socket.listen()

    # Run forever, its a tough life as an elf with all that work
    while (1):
        build_presents(me, my_host, my_port, santa_host, santa_port)
        wait_for_reply(me, listening_socket)
        # Checkin function will 'check in' with a checkin process, if one is 
        # available. This can be removed if you are confident in your answer 
        # and want to avoid the slowdown it adds
        checkin(f"Elf {me}")

# As an alternative to using the naive_santa_problem.py, you may start a 
# standalone elf as described in the handout
if __name__ == "__main__":
    me = int(sys.argv[1])
    my_host = sys.argv[2]
    my_port = int(sys.argv[3])
    santa_host = sys.argv[4]
    santa_port = int(sys.argv[5])
    process = Process(target=elf, args=(me, my_host, my_port, santa_host, santa_port))
    process.start()
