SHELL = /bin/bash

# sometimes boost is in your include path
# but if not, set this to -I<boost include dir>
BOOST_INCL=-I$(BOOST_DIR)/include

CXX		= CC
FILES		= seedgen.cpp
OBJS		= $(FILES:.cpp=.o)
CXXFLAGS	= -std=c++0x 
CXXINCL		= $(BOOST_INCL)
LDDIRS		= 

main ?= main.cpp

all: $(main) $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXINCL) $(LDDIRS) -o $(main:.cpp=.x) $(main) $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o *.x

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@


