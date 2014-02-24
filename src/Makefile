
BASE=$(HOME)/susykit
#BASE=$(HOME)/school/research

CONSTRAIN=$(BASE)/constrain/current
#SOFTSUSY=$(BASE)/softsusy/current
SOFTSUSY=$(BASE)/softsusy/current/src
MICROMEGAS=$(BASE)/micromegas/current
CALCHELP=$(MICROMEGAS)/CalcHEP_src
FEYNHIGGS=$(BASE)/feynhiggs/current
SUPERISO=$(BASE)/superiso_relic/current

BOOST=$(BOOST_DIR)/include

CXX=g++
CXXFLAGS=-std=c++0x -O3 -ffast-math -funroll-loops 
LDFLAGS=-lgfortran #-lifcore
LDFLAGS+=-lifcore -limf -lsvml -lc -lintlc -lifport

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

LCONSTRAIN=-L$(CONSTRAIN) -lconstrain

#LSOFTSUSY=-L$(SOFTSUSY)/.libs -lsoft
LSOFTSUSY=$(SOFTSUSY)/.libs/libsoft.a
CXXFLAGS+=-DSOFTSUSY_POST34

LFEYNHIGGS=-L$(FEYNHIGGS)/x86_64-Linux/lib64/ -lFH

CXXINCL=-I$(CONSTRAIN) -I$(MICROMEGAS) -I$(SOFTSUSY) -I$(BOOST) -I$(FEYNHIGGS) -I$(FEYNHIGGS)/src/include -I$(SUPERISO)/src

LDFLAGS+=$(LMICRO) $(LCONSTRAIN) $(LSOFTSUSY) $(LFEYNHIGGS)

OUTPATH=$(CURDIR)/..
SUSYTIK_INIT=init_susytik 	# bin filename
SUSYTIK=susytik 		# bin filename
GEN_SEEDS=gen_seeds		# bin filename
FEYNTEST=feyntest
QPOINT=qpoint

TARGETS=$(QPOINT) $(SUSYTIK) $(SUSYTIK_INIT) $(GEN_SEEDS) $(FEYNTEST)

OBJS=sujmicro.o sujsoft.o sujfeyn.o softsusy_opts.o # sujiso.o #sujiso not ready
SCANOBJS=susytik_scan.o suj_slha_out.o
INITOBJS=init_susytik.o suj_slha_out.o
SEEDOBJS=gen_seeds.o
FEYNOBJS=sujfeyn.o feyntest.o

all: $(TARGETS)

clean:
	rm -rf *.o
	pushd $(OUTPATH) && rm -rf $(TARGETS) && popd

$(SUSYTIK_INIT): $(OBJS) $(INITOBJS)
	$(CXX) -dynamic $(CXXFLAGS) -o $(OUTPATH)/$(SUSYTIK_INIT) $(OBJS) $(INITOBJS) $(LDFLAGS)

$(SUSYTIK): $(OBJS) $(SCANOBJS)
	$(CXX) -dynamic $(CXXFLAGS) -o $(OUTPATH)/$(SUSYTIK) $(OBJS) $(SCANOBJS) $(LDFLAGS)

$(GEN_SEEDS): $(SEEDOBJS)
	$(CXX) -dynamic $(CXXFLAGS) -o $(OUTPATH)/$(GEN_SEEDS) $(SEEDOBJS)

$(FEYNTEST): $(FEYNOBJS) suj_slha_out.o
	$(CXX)  -dynamic $(CXXFLAGS) -o $(OUTPATH)/$(FEYNTEST) $(FEYNOBJS) suj_slha_out.o $(LDFLAGS)

$(QPOINT): $(OBJS) qpoint_softsusy_opts.o sujpoint.o 
	$(CXX)  -dynamic $(CXXFLAGS) -o $(OUTPATH)/$(QPOINT) $(OBJS) qpoint_softsusy_opts.o sujpoint.o $(LDFLAGS)


.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@

