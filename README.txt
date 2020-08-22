**********Virtual Memory - Page Replacement Simulation**********
*******************Author:Jeff Godfrey**************************

This program simulates various page replacement policies including
OPT (optimum), FIFO (first-in-first-out), LRU (least-recently-used)
and CLOCK (otherwise known as the second-chance algorithm).

The program includes two header files and a main file. In order to
compile, make sure all files are included in the same directory, and
simply type 'make' in the command line (LINUX).

After compiling, the following line will be entered into the command
line to run the program:

./Project2 <your input text file> <min frames> <max frames> <pages in process>

Your input file should be a text file with values no larger than the
number of pages in process you wish to simulate minus one. It has been
coded so you may enter as many page requests as you wish. If you enter
a number larger than pages in process minus one in your inputfile, you
will be given an error message and the program will exit. If you enter
the incorrect number of arguments when trying to run the program, you 
will also receive an error message and the program will exit. If your
input file does not exist... you will also receive an error message and
the program will exit.
