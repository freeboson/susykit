
BASE=$(HOME)/school/research

CONSTRAIN=$(BASE)/constrain/current
SOFTSUSY=$(BASE)/softsusy/current
MICROMEGAS=$(BASE)/micromegas/current
CALCHELP=$(MICROMEGAS)/CalcHEP_src

CXX=g++-4.7.2
CXXFLAGS=-std=c++0x -O3 -march=native
LDFLAGS=-lgfortran

LMICRO=$(MICROMEGAS)/sources/micromegas.a $(CALCHELP)/lib/dynamic_me.a \
	$(MICROMEGAS)/MSSM/work/work_aux.a $(MICROMEGAS)/MSSM/lib/aLib.a \
	$(CALCHELP)/lib/sqme_aux.so $(CALCHELP)/lib/libSLHAplus.a \
	$(CALCHELP)/lib/num_c.a $(CALCHELP)/lib/serv.a \
	-lX11 -lm -rdynamic -ldl

LCONSTRAIN=-L$(CONSTRAIN) -lconstrain

#LSOFTSUSY=-L$(SOFTSUSY)/.libs -lsoft
LSOFTSUSY=$(SOFTSUSY)/.libs/libsoft.a

CXXINCL=-I$(CONSTRAIN) -I$(MICROMEGAS) -I$(SOFTSUSY) 

LDFLAGS+=$(LMICRO) $(LCONSTRAIN) $(LSOFTSUSY)


OUTPATH=$(CURDIR)/..
SUSYTIK_INIT=init_susytik 	# bin filename
SUSYTIK=susytik 		# bin filename
GEN_SEEDS=gen_seeds		# bin filename
QPOINT=qpoint

TARGETS=$(SUSYTIK) $(SUSYTIK_INIT) $(GEN_SEEDS) $(QPOINT)

OBJS=sujmicro.o suj_slha_out.o
SCANOBJS=susytik_scan.o
INITOBJS=init_susytik.o
SEEDOBJS=gen_seeds.o
QPOINTOBJS=qpoint.o

all: $(TARGETS)

clean:
	rm -rf *.o
	pushd $(OUTPATH) && rm -rf $(TARGETS) && popd

$(SUSYTIK_INIT): $(OBJS) $(INITOBJS)
	$(CXX) $(CXXFLAGS) -o $(OUTPATH)/$(SUSYTIK_INIT) $(OBJS) $(INITOBJS) $(LDFLAGS)

$(QPOINT): $(OBJS) $(QPOINTOBJS)
	$(CXX) $(CXXFLAGS) -o $(OUTPATH)/$(QPOINT) $(OBJS) $(QPOINTOBJS) $(LDFLAGS)

$(SUSYTIK): $(OBJS) $(SCANOBJS)
	$(CXX) $(CXXFLAGS) -o $(OUTPATH)/$(SUSYTIK) $(OBJS) $(SCANOBJS) $(LDFLAGS)

$(GEN_SEEDS): $(SEEDOBJS)
	$(CXX) $(CXXFLAGS) -o $(OUTPATH)/$(GEN_SEEDS) $(SEEDOBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@

