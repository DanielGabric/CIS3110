Name: Daniel Gabric
Student ID: ******
Assignment #2

TO COMPILE TYPE IN 'make'
TO RUN type in ./runMe with optional flags (-d -v (-r NUMBER)) < TEXTFILE

Assumptions:
- !!!!!Descriptive flag does not print in order but accumulates as events occur
- !!!!!Verbose flag does not print in order but prints as events occur
- When taking in input from textfile I expect space delimiters and nothing other than the numbers involved
- There is no switch time between the same thread in the same process
- There is no switch time for the first thread to be popped
- switch time comes after io time
- Will prioritize same thread if same arrival time for round robin
- MIGHT SEGFAULT ON MAC
- When in round robin you push the thread back int he event queue if the quantum is smaller than the remaining cpu time and keep it as ready
- MAX LENGTH OF HEAP 300
- MAX LENGTH OF DESCRIPTION 300*300

i. Does your simulator include switch overhead for the first ready state to running state transition? Explain.
   It does not because I assume it starts at a state with no process/thread before it.
ii. Does your simulator include switch overhead if a thread moves from ready state to running state and the CPU is idle? Explain.
    Yes it does because the only way a thread can go from ready to running is if a previous thread has stopped so you switch.
iii. Does your simulator include switch overhead if a thread moves from running state to blocked state and the ready queue is empty? Explain.
    No it does not because I am not using a ready queue.
iv. Does your simulation include switch overhead if a thread is interrupted (due to a time slice) and either the ready queue is empty or the thread has the highest priority? Explain. 
    No.

REFERENCES:
- I used the Heap code provided in the lab