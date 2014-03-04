SHELL = /bin/bash

########## Set these directories ###############################
BASE=$(HOME)/susykit
#BASE=$(HOME)/school/research

#use this for <softsusy-3.4.0
#SOFTSUSY=$(BASE)/softsusy/current
#use this for >=softsusy-3.4.0
SOFTSUSY=$(BASE)/softsusy/current/src
SOFTSUSY_VERSION_POST_340=1

MICROMEGAS=$(BASE)/micromegas/current
CALCHELP=$(MICROMEGAS)/CalcHEP_src
DARKSUSY=$(BASE)/darksusy/current
FEYNHIGGS=$(DARKSUSY)/contrib/FeynHiggs-2.10.0

SUPERISO=$(BASE)/superiso_relic/current
SUPERISO_NO_RELIC=1

# sometimes boost is in your include path
# but if not, set this to -I<boost include dir>
BOOST_INCL=-I$(BOOST_DIR)/include

################################################################


CXX		= CC
FILES		= priors.cpp nestface.cpp
OBJS		= $(FILES:.cpp=.o)
CXXINCL		= -I../include -I$(MICROMEGAS) -I$(SOFTSUSY) $(BOOST_INCL) -I$(FEYNHIGGS) -I$(FEYNHIGGS)/src/include -I$(SUPERISO)/src
LDDIRS		= -L../lib -L$(SOFTSUSY)/.libs -L$(DARKSUSY)/lib -L$(SUPERISO)/src -L$(SUPERISO)/src/relic
CXXFLAGS	= -std=c++0x -O3 -march=native -funroll-loops
CXXINCL		= -I../include -I$(MICROMEGAS) -I$(SOFTSUSY) $(BOOST_INCL) -I$(FEYNHIGGS) -I$(FEYNHIGGS)/src/include -I$(SUPERISO)/src
LDDIRS		= -L../lib -L$(SOFTSUSY)/.libs -L$(DARKSUSY)/lib -L$(SUPERISO)/src -L$(SUPERISO)/src/relic

ifeq ($(SOFTSUSY_VERSION_POST_340),1)
	CXXFLAGS += -DSOFTSUSY_POST34
endif
ifeq ($(SUPERISO_NO_RELIC),1)
	CXXFLAGS += -DSUPERISO_NO_RELIC 
	LDFLAGS+= -static -lsusykit -lconstrain -lisospin -lsoft -ldarksusy -lisajet -lHB -lFH -lgfortran
else
	LDFLAGS+= -static -lsusykit -lconstrain -lisospin -lrelic -lsoft -ldarksusy -lisajet -lHB -lFH -lgfortran
endif


main ?= susytikNS.cpp

all: $(main) $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXINCL) $(LDDIRS) -o $(main:.cpp=.x) $(main) $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o *.x

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@


