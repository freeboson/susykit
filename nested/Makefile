
BASE=$(HOME)/susykit

CONSTRAIN=$(BASE)/constrain/current
SOFTSUSY=$(BASE)/softsusy/current
MICROMEGAS=$(BASE)/micromegas/current
CALCHELP=$(MICROMEGAS)/CalcHEP_src
FEYNHIGGS=$(BASE)/feynhiggs/current
SUPERISO=$(BASE)/superiso_relic/current


MULTINEST=$(BASE)/multinest/current

CXX=mpiCC
CXXFLAGS=-std=c++0x -O3
LDFLAGS= -lmkl_def -lmkl_p4n -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -limf -lpthread -lifcore -lsvml

LMICRO2=$(MICROMEGAS)/sources/micromegas.a $(CALCHELP)/lib/dynamic_me.a \
	$(MICROMEGAS)/MSSM/work/work_aux.a $(MICROMEGAS)/MSSM/lib/aLib.a \
	$(CALCHELP)/lib/sqme_aux.so $(CALCHELP)/lib/libSLHAplus.a \
	$(CALCHELP)/lib/num_c.a  $(CALCHELP)/lib/serv.a \
	-lX11 -lm -rdynamic -ldl


LMICRO3=$(MICROMEGAS)/sources/micromegas.a $(CALCHELP)/lib/dynamic_me.a \
	$(MICROMEGAS)/MSSM/work/work_aux.a $(MICROMEGAS)/MSSM/lib/aLib.a \
	$(CALCHELP)/lib/sqme_aux.so $(CALCHELP)/lib/libSLHAplus.a \
	$(CALCHELP)/lib/num_c.a $(CALCHELP)/lib/ntools.a $(CALCHELP)/lib/serv.a \
	-lX11 -lm -rdynamic -ldl

LMICRO=$(LMICRO2)

LSOFTSUSY=$(SOFTSUSY)/.libs/libsoft.a
LCONSTRAIN=-L$(CONSTRAIN) -lconstrain 
LMULTINEST=-L$(MULTINEST) -lnest3 
LFEYNHIGGS=$(FEYNHIGGS)/x86_64-Linux/lib64/libFH.a -lgfortran

CXXINCL=-I../include -I$(CONSTRAIN) -I$(MICROMEGAS) -I$(SOFTSUSY)  -I$(FEYNHIGGS) -I$(SUPERISO)/src -I../
LDFLAGS+=$(LMICRO) $(LCONSTRAIN) $(LSOFTSUSY) $(LFEYNHIGGS) $(LMULTINEST) -L../lib -lsusykit


OBJS=nestface.o priors.o

main ?= susytikNS.cpp

all: $(main) $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXINCL) -o $(main:.cpp=.x) $(main) $(OBJS) $(LDFLAGS)

clean:
	rm -rf *.o *.x

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@

