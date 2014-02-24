
BASE=$(HOME)/susytik

CONSTRAIN=$(BASE)/constrain/current
SOFTSUSY=$(BASE)/softsusy/current
MICROMEGAS=$(BASE)/micromegas/current
CALCHELP=$(MICROMEGAS)/CalcHEP_src


MULTINEST=$(BASE)/multinest/current

CXX=CC
CXXFLAGS=-std=c++0x -O3
LDFLAGS=-dynamic

LMICRO=$(MICROMEGAS)/sources/micromegas.a $(CALCHELP)/lib/dynamic_me.a \
	$(MICROMEGAS)/MSSM/work/work_aux.a $(MICROMEGAS)/MSSM/lib/aLib.a \
	$(CALCHELP)/lib/sqme_aux.so $(CALCHELP)/lib/libSLHAplus.a \
	$(CALCHELP)/lib/num_c.a $(CALCHELP)/lib/serv.a \
	-lX11 -lm -rdynamic -ldl
LSOFTSUSY=$(SOFTSUSY)/.libs/libsoft.a
LCONSTRAIN=-L$(CONSTRAIN) -lconstrain
LMULTINEST=-L$(MULTINEST) -lnest3 -lgfortran

CXXINCL=-I$(CONSTRAIN) -I../ -I../hepstats -I$(MICROMEGAS) -I$(SOFTSUSY) 
LDFLAGS+=$(LCONSTRAIN) $(LMULTINEST) $(LMICRO) $(LSOFTSUSY)



OBJS=nestface.o ../sujmicro.o ../suj_slha_out.o ../hepstats/likeconfig.o ../hepstats/likedatum.o ../hepstats/loglike.o

all: nestface

clean:
	rm -rf *.o

nestface: $(OBJS)
	$(CXX) $(CXXFLAGS) -o susytikNS $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@

