
BASE=$(HOME)/susytik

CONSTRAIN=$(BASE)/constrain/current

CXX=CC
CXXFLAGS=-std=c++0x -O3
LDFLAGS=

LCONSTRAIN=-L$(CONSTRAIN) -lconstrain
CXXINCL=-I$(CONSTRAIN)
LDFLAGS+=$(LCONSTRAIN)



OBJS=likeconfig.o likedatum.o loglike.o test.o

all: test

clean:
	rm -rf *.o

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o test $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINCL) -c $< -o $@

