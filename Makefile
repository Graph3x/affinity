CXX      := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c++17

OBJDIR   := build

OBJS     := $(OBJDIR)/example.o $(OBJDIR)/logging.o $(OBJDIR)/comms.o $(OBJDIR)/pyro.o $(OBJDIR)/timer.o $(OBJDIR)/sensors.o $(OBJDIR)/trajectory_controller.o

all: example

example: $(OBJS)
	$(CXX) $(OBJS) -o $@

$(OBJDIR)/example.o: example.cpp misc/logging.h comms/comms.h pyro/pyro.h misc/timer.h sensors/sensors.h sensors/trajectory_controller.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c example.cpp -o $@

$(OBJDIR)/logging.o: misc/logging.cpp misc/logging.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c misc/logging.cpp -o $@

$(OBJDIR)/comms.o: comms/comms.cpp comms/comms.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c comms/comms.cpp -o $@

$(OBJDIR)/pyro.o: pyro/pyro.cpp pyro/pyro.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c pyro/pyro.cpp -o $@

$(OBJDIR)/timer.o: misc/timer.cpp misc/timer.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c misc/timer.cpp -o $@

$(OBJDIR)/trajectory_controller.o: sensors/trajectory_controller.cpp sensors/trajectory_controller.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c sensors/trajectory_controller.cpp -o $@

$(OBJDIR)/sensors.o: sensors/sensors.cpp sensors/sensors.h
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c sensors/sensors.cpp -o $@




clean:
	rm -rf example $(OBJDIR)
