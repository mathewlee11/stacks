I wrote this a couple of years ago, most likely out of boredom. It's a mini programming language based entirely on the stack data structure.

#Basic Idea
The memory architecture is an infinite (minus any memory constraints) row of stacks. You have the ability to shift between these stacks, and push or pop values to them. You also have a "current" number, which you can print out or transfer between stacks.

#Operators
v : pushes the current value onto the current stack, and sets current to 0

` : same as v except it doesn't reset current

^ : pop the top value from the current stack in current

~ : same as ^ except doesn't remove the top value from the stack

< : shift to the stack to the left, creating it if it doesn't exist

\> : shift to the stack to the right, creating it if it doesn't exist

\# : print the current value as a number

@ : print the current value as a character

+-/* : simple arithmetic, explained below
