
#BASE=$(HOME)/susytik
BASE=$(HOME)/school/research

CONSTRAIN=$(BASE)/constrain/current
SOFTSUSY=$(BASE)/softsusy/current
MICROMEGAS=$(BASE)/micromegas/current
CALCHELP=$(MICROMEGAS)/CalcHEP_src
FEYNHIGGS=$(BASE)/feynhiggs/current

BOOST=$(BOOST_DIR)/include

CXX=g++
CXXFLAGS=-std=c++0x -O3 -ffast-math -funroll-loops
LDFLAGS=-lgfortran

LMICRO=$(MICROMEGAS)/sources/micromegas.a $(CALCHELP)/lib/dynamic_me.a \
	$(MICROMEGAS)/MSSM/work/work_aux.a $(MICROMEGAS)/MSSM/lib/aLib.a \
	$(CALCHELP)/lib/sqme_aux.so $(CALCHELP)/lib/libSLHAplus.a \
	$(CALCHELP)/lib/num_c.a $(CALCHELP)/lib/serv.a \
	-lX11 -lm -rdynamic -ldl

LCONSTRAIN=-L$(CONSTRAIN) -lconstrain

#LSOFTSUSY=-L$(SOFTSUSY)/.libs -lsoft
LSOFTSUSY=$(SOFTSUSY)/.libs/libsoft.a

LFEYNHIGGS=$(FEYNHIGGS)/feynbuild/lib64/libFH.a

CXXINCL=-I$(CONSTRAIN) -I$(MICROMEGAS) -I$(SOFTSUSY) -I$(BOOST) -I$(FEYNHIGGS) -I$(FEYNHIGGS)/src/include

LDFLAGS+=$(LMICRO) $(LCONSTRAIN) $(LSOFTSUSY) $(LFEYNHIGGS)


OUTPATH=$(CURDIR)/..
SUSYTIK_INIT=init_susytik 	# bin filename
SUSYTIK=susytik 		# bin filename
GEN_SEEDS=gen_seeds		# bin filename
FEYNTEST=feyntest

TARGETS=$(SUSYTIK) $(SUSYTIK_INIT) $(GEN_SEEDS) $(FEYNTEST)

OBJS=sujmicro.o suj_slha_out.o
SCANOBJS=susytik_scan.o
INITOBJS=init_susytik.o
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
	$(CXX) $(CXXFLAGS) -o $(OUTPATH)/$(GEN_SEEDS) $(SEEDOBJS)

$(FEYNTEST): $(FEYNOBJS) suj_slha_out.o
	$(CXX) $(CXXFLAGS) -o $(OUTPATH)/$(FEYNTEST) $(FEYNOBJS) suj_slha_out.o $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@

