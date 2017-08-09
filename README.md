# ATMEGA32-scheduler-class
A memory-friendly class for controlling program flow at timed intervals for atmega32 and Arduino 

It is useful to break a program down into individual blocks that get run at different intervals. A keypad only needs to be checked every five milliseconds, an LCD (and the human eye) are slow devices, so an update every 200 milliseconds or so if required is more than adequate. This frees up resources and time for the other parts of your program to do the critical stuff.

The files are heavily-documented, and full documentation is included if you open index.html in the./docs folder.

Note that the software uses some C++11 constructs, so include -std=c++11 to the flags for the compiler. On Atmel Studio, open your project properties, Toolchain, C++ Compiler, Miscellaneous, and add -std=c++11 (all lowercase).

I would love to hear your comments and suggestions.
