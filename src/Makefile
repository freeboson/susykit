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
#BOOST_INCL=-I$(BOOST_DIR)/include
BOOST_INCL=

################################################################


CXX		= CC
LCONSFILES	= parseutils.cpp parse.cpp model.cpp model_lookup.cpp special_lookups.cpp get_slha.cpp likeconfig.cpp likedatum.cpp loglike.cpp 
CONFILES	= constraint.cpp constrain.cpp constrain_opts.cpp
QPARSEFILES	= qparse.cpp
SLHAFILES	= sdb2slha.cpp
LSKITFILES	= superiso_interface.cpp darksusy_interface.cpp micromegas_interface.cpp softsusy_interface.cpp feynhiggs_interface.cpp softsusy_opts.cpp
LCONSOBJS	= $(LCONSFILES:.cpp=.o)
CONOBJS		= $(CONFILES:.cpp=.o)
PLOTOBJS	= $(PLOTFILES:.cpp=.o)
QPARSEOBJS	= $(QPARSEFILES:.cpp=.o)
SLHAOBJS	= $(SLHAFILES:.cpp=.o)
LSKITOBJS	= $(LSKITFILES:.cpp=.o)
BINS   		= constrain qparse sdb2slha qpoint susykit_init susykit_scan gen_seeds
LIBS   		= libconstrain.a libsusykit.a 
CXXFLAGS	= -std=c++0x -O3 -march=native -funroll-loops
CXXINCL		= -I../include -I$(MICROMEGAS) -I$(SOFTSUSY) $(BOOST_INCL) -I$(FEYNHIGGS) -I$(FEYNHIGGS)/src/include -I$(SUPERISO)/src
LDDIRS		= -L../lib -L$(SOFTSUSY)/.libs -L$(DARKSUSY)/lib -L$(SUPERISO)/src -L$(SUPERISO)/src/relic



all: $(LIBS) $(BINS) hepstats_test darktest

#debug: clean
debug: CXXFLAGS= -O0 -ggdb -std=c++0x -DLCONSTRAIN_DEBUG -DSOFTSUSY_POST34
debug: all

libconstrain.a: $(LCONSOBJS) 
	ar csr ../lib/$@ $^

qparse: libconstrain.a $(QPARSEOBJS)
	$(CXX) $(CXXFLAGS) $(QPARSEOBJS) $(LDDIRS) -lconstrain -o ../bin/qparse

constrain: libconstrain.a $(CONOBJS)
	$(CXX) $(CXXFLAGS) $(CONOBJS) $(LDDIRS) -lconstrain -o ../bin/constrain

sdb2slha: libconstrain.a $(SLHAOBJS)
	$(CXX) $(CXXFLAGS) $(SLHAOBJS) $(LDDIRS) -lconstrain -o ../bin/sdb2slha

hepstats_test: libconstrain.a hepstats_test.o
	$(CXX) $(CXXFLAGS) $(CXXINCL) hepstats_test.o $(LDDIRS) -lconstrain -o ../test/hepstats_test

libsusykit.a: $(LSKITOBJS)
	ar csr ../lib/$@ $^

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


ifeq ($(SOFTSUSY_VERSION_POST_340),1)
	CXXFLAGS += -DSOFTSUSY_POST34
endif
ifeq ($(SUPERISO_NO_RELIC),1)
	CXXFLAGS += -DSUPERISO_NO_RELIC 
	LDFLAGS+= -Wl,-Bstatic -lsusykit -lconstrain -lisospin -lsoft -ldarksusy -lisajet -lHB -lFH -lgfortran $(LMICRO)
else
	LDFLAGS+= -Wl,-Bstatic -lsusykit -lconstrain -lisospin -lrelic -lsoft -ldarksusy -lisajet -lHB -lFH -lgfortran $(LMICRO)
endif


susykit_init: $(LIBS) susykit_init.o
	$(CXX) $(CXXFLAGS) -o ../bin/susykit_init susykit_init.o $(LDDIRS) $(LDFLAGS)

susykit_scan: $(LIBS) susykit_scan.o
	$(CXX) $(CXXFLAGS) -o ../bin/susykit_scan susykit_scan.o $(LDDIRS) $(LDFLAGS)

qpoint: $(LIBS) qpoint_softsusy_opts.o qpoint.o 
	$(CXX) $(CXXFLAGS) -o ../bin/qpoint qpoint_softsusy_opts.o qpoint.o $(LDDIRS) $(LDFLAGS)

gen_seeds: gen_seeds.o
	$(CXX) $(CXXFLAGS) gen_seeds.o -o ../bin/gen_seeds

darktest: $(LIBS) darktest.o
	$(CXX) $(CXXFLAGS) $(CXXINCL) darktest.o qpoint_softsusy_opts.o $(LDDIRS) -o ../test/darktest $(LDFLAGS)

clean:
	rm -rf *.o
	pushd ../bin/ && (rm -rf $(BINS); popd)
	pushd ../lib/ && (rm -rf $(LIBS); popd)
	pushd ../test/ && (rm -rf hepstats_test; popd)
	make -C ../nested clean

#NOCPP11:=$(filter-out -std=c++0x,$(CXXFLAGS))
#superiso_interface_pimpl.o:
#	$(CXX) $(NOCPP11) $(CXXINCL) -c superiso_interface_pimpl.cpp -o superiso_interface_pimpl.o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@


