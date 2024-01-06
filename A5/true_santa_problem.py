import sys

from multiprocessing import Process

from porch import *
from shared import *
from stable import *
from true_elf import *
from true_reindeer import *
from true_santa import *

# Helper function describing how program should be called
def print_usage_and_exit():
    print("Usage: python3 santa_problem X Y Z")
    print(f"  X: Number of reindeer (must be number, at least {MIN_REINDEER})")
    print(f"  Y: Number of elves (must be number, at least {MIN_ELVES} and at least Z many)")
    print(f"  Z: Number of elves to pester santa (must be number)")
    exit()

if __name__ == "__main__":
    # Setup input variables
    try:
        num_reindeer = int(sys.argv[1])
        num_elves = int(sys.argv[2])
        elf_group = int(sys.argv[3])
    except:
        print_usage_and_exit()
    if num_elves < MIN_ELVES or num_reindeer < MIN_REINDEER or num_elves < elf_group:
        print_usage_and_exit() 

    print(f"Starting with {num_reindeer} reindeer and {num_elves} elves")

    # Setup all necessary processes. One santa, one stable, one potch, 
    # num_reindeer many reindeer, and num_elves many elves
    all_processes = [Process(target=santa, args=(DEFAULT_HOST, SANTA_PORT))] \
        + [Process(target=stable, args=(DEFAULT_HOST, STABLE_PORT, DEFAULT_HOST, SANTA_PORT, num_reindeer))] \
        + [Process(target=reindeer, args=(i, DEFAULT_HOST, REINDEER_PORT+i, DEFAULT_HOST, STABLE_PORT)) for i in range(num_reindeer)] \
        + [Process(target=porch, args=(DEFAULT_HOST, PORCH_PORT, DEFAULT_HOST, SANTA_PORT, elf_group))] \
        + [Process(target=elf, args=(i, DEFAULT_HOST, ELF_PORT+i, DEFAULT_HOST, PORCH_PORT)) for i in range(num_elves)]
    
    # Start all processes at the same time (ish)    
    for p in all_processes:
        p.start()

    # Print some feedback to confirm all setup done
    print("All processes started")