########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : CHRISTOPHER ROBERT SALTER
# Student Number   : s3334875
# Course Code      : COSC1076
# Program Code     : BP162
# Start up code provided by Paul Miller
########################################################################

SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h
README=ppd_readme
MAKEFILE=Makefile
OBJECTS=ppd_main.o ppd_stock.o ppd_coin.o ppd_menu.o ppd_options.o \
ppd_utility.o
# The compiler
CC=gcc
# The compiler flags
CFLAGS=-ansi -Wall -pedantic -ggdb
# The linker files
LFLAGS=

#default target call the 'ppd' target
all:ppd

# Calls the linker
ppd: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o ppd
	
#compiles each .c file into a .o file
%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# debug target	
debug:CFLAGS=-ansi -Wall -pedantic -g
debug:clean ppd

.PHONY:clean
clean:
	rm -f *.o ppd
########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)
