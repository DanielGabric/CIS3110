Name: Daniel Gabric
ID: ******
Assignment #1

Algorithm explanation:
-pretty self explanatory, just create new processes and end them when they are done 
 executing

READ:
-to compile type in "make"
-to run after compiling type ./runMe

Notes:
- since I allocate memory for every command and never free the memory,
  there might be segfaults after continued use
- no multiple piping, and also, <, >, and | only work for two parameters/commands ex. (ls -1 > test.txt)(cat < test.txt)(ls -1 | grep .c)
- #include <wait.h> for linux, and comment that out if it is marked on mac osx
- sigset sometimes makes the code crap out
- piping sometimes causes the shell prompt to print out more than one ex. (Daniel's Shell>Daniel's Shell>)

EXTRA COMMAND - #3 on part 2

-if is the same as add as in it takes in integer arguments, but to invoke the command
 use keyword "muls", and it multiplies all integer arguments following it


References:

I used snippets of code from 
-http://people.cs.pitt.edu/~khalifa/cs1550/sum06/Assignments/Assign1/proj1/myshell.c
and from
-http://beej.us/guide/bgipc/output/html/multipage/pipes.html