###############################################
# Makefile for compiling the program skeleton
# 'make'           build executable file 'PROJ'
# 'make test'   build test files for project
# 'make all'       build project + test
# 'make clean'  removes all .o, executable 
###############################################
PROJ = s   # the name of the project
CC   = gcc            # name of compiler 
DOXYGEN = doxygen        # name of doxygen binary
# define any compile-time flags
CFLAGS = -std=gnu99 -Wall -O -Wuninitialized -g -pedantic   # there is a space at the end of this
LFLAGS = -lm -pthread                                          
###############################################
# You don't need to edit anything below this line
###############################################
# list of object files 
# The following includes all of them!
C_FILES := $(wildcard *.c)
	OBJS := $(patsubst %.c, %.o, $(C_FILES)) 
# To create the executable file  we need the individual
# object files 
$(PROJ): $(OBJS)
	$(CC) $(LFLAGS) -o $(PROJ) $(OBJS)
# To create each individual object file we need to 
# compile these files using the following general
# purpose macro
.c.o:
	$(CC) $(CFLAGS)  -c $<
# there is a TAB for each identation. 
# To make all (program + test file) "make all"      
all : 
	make
	gcc TestFile.c libs.h msgmodule.h msgmodule.c filemodule.h filemodule.c -Wall -DDEBUG=1 -o TestFile

# To make test file: "make test"      
test :
	gcc TestFile.c libs.h msgmodule.h msgmodule.c filemodule.h filemodule.c -Wall -DDEBUG=1 -o TestFile

# To clean .o files: "make clean"
clean:
	rm -rf *.o 
