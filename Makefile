#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the Cpp compiler to use
CXX = g++

# define any compile-time flags
CXXFLAGS	:= -std=c++11 -Wall -Wextra -g

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTPUT	:= build

# define source directory
SRCDIR := src
SRC	   += $(SRCDIR)/charger
SRC	   += $(SRCDIR)/global_clk
SRC	   += $(SRCDIR)/evtol_sim
SRC	   += $(SRCDIR)/flight_sim

#define lib subdirectories

# define include directory
INCLUDE	:= include

#define main directory
MAINPATH := main

#define test directory
TEST := tests

ifeq ($(OS),Windows_NT)
MAIN	:= main.exe
SOURCEDIRS	:= $(SRC)
TESTDIRS 		:= $(TEST)
MAINDIRS 		:= $(MAINPATH)
INCLUDEDIRS	:= $(INCLUDE)
FIXPATH = $(subst /,\,$1)
RM	:= rm -f
MD	:= mkdir
else
MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
TESTDIRS 	  := $(shell find $(TEST) -type d)
MAINDIRS 		:= $(shell find $(MAINPATH) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
INCLUDES  += $(patsubst %,-I%, $(SOURCEDIRS:%/=%))

#define C main file
SRCMAIN := $(wildcard $(patsubst %,%/*.cpp, $(MAINDIRS)))

# define the C test files
TESTS := $(wildcard $(patsubst %,%/*.cpp, $(TESTDIRS)))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))

# define the C object files
OBJECTS		:= $(SOURCES:.cpp=.o)
TESTOBJ 	:= $(TESTS:.cpp=.o)
MAINOBJ   := $(SRCMAIN:.cpp=.o)

# Switch between main and test here
# TODO: make more elegant, perhaps with a runtime switch
OBJECTS += $(MAINOBJ)
# OBJECTS += $(TESTOBJ)

# define the dependency output files
DEPS		:= $(OBJECTS:.o=.d)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's and .d's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# -MMD generates dependency output files same name as the .o file
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!
