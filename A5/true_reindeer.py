
import random
import socket
import sys
import time

from shared import *

# All reindeer will go on holiday (sleep) as soon as their deliveries are 
# complete. Once they return (wake) they will inform the stable that they have 
# done so 
def go_on_holiday(me, my_host, my_port, stable_host, stable_port):
    # Start their holiday
    print(f"Reindeer {me} has gone on holiday")
    time.sleep(random.randint(MIN_HOLIDAY, MAX_HOLIDAY))
    # Return from holiday
    print(f"Reindeer {me}'s holiday is over")

    # Assemble a message including my own network address, so the stable can 
    # respond
    msg = bytearray(MSG_HOLIDAY_OVER)
    msg.extend(f"-{my_host}:{my_port}".encode())

    # Send the message to the stable
    sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sending_socket.connect((stable_host, stable_port))
    sending_socket.sendall(msg)
    sending_socket.close()

# Once returned from holiday, wait for a reply
def wait_for_reply(me, listening_socket, my_host, my_port):
    # Wait on the listening port to get a connection request
    connection, _ = listening_socket.accept()

    # TODO You must implement how a reindeer will wait for a reply from the 
    # stable. 


# Base reindeer function, to be called as a process
def reindeer(me, my_host, my_port, stable_host, stable_port):
    # Open a listening socket. Doing so early means its definately open before 
    # we might need it
    listening_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listening_socket.bind((my_host, my_port))
    listening_socket.listen()

    # Run forever, its a good life as a reindeer with all those holidays
    while (1):
        go_on_holiday(me, my_host, my_port, stable_host, stable_port)
        wait_for_reply(me, listening_socket, my_host, my_port)
        # Checkin function will 'check in' with a checkin process, if one is 
        # available. This can be removed if you are confident in your answer 
        # and want to avoid the slowdown it adds
        checkin(f"Reindeer {me}")

# As an alternative to using the true_santa_problem.py, you may start a 
# standalone reindeer as described in the handout
if __name__ == "__main__":
    me = int(sys.argv[1])
    my_host = sys.argv[2]
    my_port = int(sys.argv[3])
    santa_host = sys.argv[4]
    santa_port = int(sys.argv[5])
    process = Process(target=reindeer, args=(me, my_host, my_port, santa_host, santa_port))
    process.start()
