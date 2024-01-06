
# A variety of configuation options. Generally these should not need to be 
# altered but depending on your testing, you may wish to do so

MIN_REINDEER = 3
MIN_ELVES = 5

MIN_HOLIDAY = 20
MAX_HOLIDAY = 30

MIN_BUILD = 5
MAX_BUILD = 45

DEFAULT_HOST = "127.0.0.1"
SANTA_PORT = 5000
STABLE_PORT = 5001
PORCH_PORT = 5002
REINDEER_PORT = 6000
ELF_PORT = 7000

MSG_HOLIDAY_OVER = b"over"
MSG_DELIVER_PRESENTS = b"deliver"
MSG_PROBLEM = b"problem"
MSG_SORT_PROBLEM = b"solve"
MSG_NOTIFY = b"notify"

MAX_MSG_LEN = 1024

# The below is a crude utility that allows for each process in the system to 
# 'check in' with an external processes, so that we can see when we last heard 
# from each process. THIS IS NOT A TOOL FOR PROVING DEADLOCK FREEDOM, but it 
# can be used as a crude check as it to how your system is behaving

import socket
import time
from datetime import datetime
from multiprocessing import Process

CHECK_HOST = "127.0.0.1"
CHECK_PORT = 8000

def checkin(name):
    try:
        sending_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sending_socket.connect((CHECK_HOST, CHECK_PORT))
        sending_socket.sendall(bytearray(name.encode()))
        sending_socket.close()
    except:
        pass

def checkin_process(my_host, my_port):
    print("Started checkin process")

    listening_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listening_socket.bind((my_host, my_port))
    listening_socket.listen()

    checkins = {}

    while (1):
        connection, _ = listening_socket.accept()
        msg = connection.recv(MAX_MSG_LEN)
        checkins[msg.decode()] = time.time()

        print(f"Latest checkins at {datetime.now().strftime('%H:%M.%S')}")
        for k, v in checkins.items():
            print(f"\t{k} last seen {round(time.time() - v, 0)} seconds ago")

if __name__ == "__main__":
    process = Process(target=checkin_process, args=(CHECK_HOST, CHECK_PORT))
    process.start()