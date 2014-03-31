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
#FEYNHIGGS=$(DARKSUSY)/contrib/FeynHiggs-2.10.0
FEYNHIGGS=$(BASE)/feynhiggs/current

SUPERISO=$(BASE)/superiso/current
SUPERISO_NO_RELIC=1

# sometimes boost is in your include path
# but if not, set this to -I<boost include dir>
#BOOST_INCL=-I$(HOME)/boost/boost_1_55_0

################################################################


CXX		= mpiCC
FILES		= seedgen.cpp mpiscan.cpp
OBJS		= $(FILES:.cpp=.o)
CXXINCL		= -I../include -I$(MICROMEGAS) -I$(SOFTSUSY) $(BOOST_INCL) -I$(FEYNHIGGS) -I$(FEYNHIGGS)/src/include -I$(SUPERISO)/src
CXXFLAGS	= -std=c++0x -fpermissive -Wno-deprecated
LDDIRS		= -L../lib -L$(SOFTSUSY)/.libs -L$(DARKSUSY)/lib -L$(SUPERISO)/src -L$(FEYNHIGGS)/build 
LDFLAGS=-lgfortran #-lifcore
LDFLAGS+=-lifcore -limf -lsvml -lc -lifport 

ifeq ($(SOFTSUSY_VERSION_POST_340),1)
	CXXFLAGS += -DSOFTSUSY_POST34
endif

LMICRO2=$(MICROMEGAS)/sources/micromegas.a $(CALCHELP)/lib/dynamic_me.a \
	$(MICROMEGAS)/MSSM/work/work_aux.a $(MICROMEGAS)/MSSM/lib/aLib.a \
	$(CALCHELP)/lib/libSLHAplus.a $(CALCHELP)/lib/num_c.a \
	$(CALCHELP)/lib/serv.a -lm \
	-Wl,-Bdynamic -rdynamic -ldl -lX11 $(CALCHELP)/lib/sqme_aux.so

# sorry, I give up with micromegas-3.x 
LMICRO3=$(MICROMEGAS)/sources/micromegas.a $(CALCHELP)/lib/dynamic_me.a \
	$(MICROMEGAS)/MSSM/work/work_aux.a $(MICROMEGAS)/MSSM/lib/aLib.a \
	$(CALCHELP)/lib/sqme_aux.so $(CALCHELP)/lib/libSLHAplus.a \
	$(CALCHELP)/lib/num_c.a $(CALCHELP)/lib/ntools.a $(CALCHELP)/lib/serv.a \
	-lX11 -lm -rdynamic -ldl

LMICRO=$(LMICRO2)


LDFLAGS+= -lsusykit -lconstrain -lisospin -lsoft -lFH $(LMICRO)

main ?= main.cpp

all: $(main) $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXINCL) $(LDDIRS) -o $(main:.cpp=.x) $(main) $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o *.x

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@


