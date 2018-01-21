CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
DEBUGFLAGS = -DDEBUG

LIBS = DiskSchedulerShared.h libthread.o -ldl -pthread

EXECUTABLE = scheduler

PROJECTFILE = scheduler.cpp

TESTSOURCES = $(wildcard test*.cpp)
TESTS = $(TESTSOURCES:%.cpp=%)

SOURCES = $(wildcard *.cpp)
SOURCES := $(filter-out $(TESTSOURCES), $(SOURCES))
OBJECTS = $(SOURCES:%.cpp=%.o)

all: $(EXECUTABLE)

release: CXXFLAGS += -DNDEBUG
release: all

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean all

$(EXECUTABLE): $(OBJECTS) #$(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

define make_tests
    SRCS = $$(filter-out $$(PROJECTFILE), $$(SOURCES))
    OBJS = $$(SRCS:%.cpp=%.o)
    HDRS = $$(wildcard *.h)
    $(1): CXXFLAGS += $$(DEBUGFLAGS)
    $(1): $$(HDRS) $$(OBJS) $(1).cpp
	$$(CXX) $$(CXXFLAGS) $$(LIBS) $$(OBJS) $(1).cpp -o $(1).exe
endef
$(foreach test, $(TESTS), $(eval $(call make_tests, $(test))))

alltests: clean $(TESTS)

DiskSchedulerShared.o: DiskSchedulerShared.h DiskSchedulerShared.cpp
ServicerT.o: ServicerT.h ServicerT.cpp DiskSchedulerShared.o
RequesterT.o: RequesterT.h RequesterT.cpp DiskSchedulerShared.o
SchedulerT.o: SchedulerT.h SchedulerT.cpp ServicerT.o RequesterT.o DiskSchedulerShared.o
scheduler.o: DiskSchedulerShared.h scheduler.cpp SchedulerT.o 


clean: 
	rm -f $(OBJECTS) $(EXECUTABLE) $(TESTS)

.PHONY: all clean alltests debug release
.SUFFIXES: