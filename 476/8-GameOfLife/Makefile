########################################################
# Variable definitions
########################################################
# C++ compiler
#CXX := clang++
CXX := g++

# C++ compiler flags
# Use this first configuration for debugging
CXXFLAGS := -ggdb -Wall -std=c++14
# Use the following  configuration for release
#CXXFLAGS := -O3 -Wall -std=c++14

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Linker flags. Usually none.
LDFLAGS := 

# Library paths, prefaced with "-L". Usually none.
LDPATHS := 

# Libraries used, prefaced with "-l".
LDLIBS := -lpthread -lboost_system -lboost_thread

# All source files, separated by spaces. Don't include header files. 
SRCS := Life.cc

# Extension for source files
SOURCESUFFIX := cc

# Determines object files based on SRCS.
OBJS := $(SRCS:.$(SOURCESUFFIX)=.o)

# Executable name. Defaults to basename of first name in SRCS.
EXEC := $(patsubst %.o, %, $(word 1, $(OBJS)))

# Command to generate dependency rules for make. 
MAKEDEPEND := $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MP

#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#       recipe
#############################################################

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(LINK) $(LDFLAGS) $(LDPATHS) $^ -o $@ $(LDLIBS)

-include Makefile.deps

#############################################################

.PHONY : clean
clean :
	$(RM) $(EXEC) $(OBJS) a.out core
	$(RM) Makefile.deps *~

.PHONY :  Makefile.deps
Makefile.deps :
	$(MAKEDEPEND) $(SRCS) > $@

#############################################################
#############################################################
