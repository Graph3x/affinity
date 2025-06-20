CXX      := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c++17

OBJDIR   := build

OBJS     := $(OBJDIR)/example.o $(OBJDIR)/logging.o $(OBJDIR)/comms.o

all: example

example: $(OBJS)
	$(CXX) $(OBJS) -o $@

$(OBJDIR)/example.o: example.cpp infrastructure/logging.h comms/comms.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c example.cpp -o $@

$(OBJDIR)/logging.o: infrastructure/logging.cpp infrastructure/logging.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c infrastructure/logging.cpp -o $@

$(OBJDIR)/comms.o: comms/comms.cpp comms/comms.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c comms/comms.cpp -o $@

$(OBJDIR)/pyro.o: pyro/pyro.cpp pyro/pyro.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c pyro/pyro.cpp -o $@

clean:
	rm -rf example $(OBJDIR)
