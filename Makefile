CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
DEBUGFLAGS = -DDEBUG

LIBS = libthread.o -ldl -pthread

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
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $(EXECUTABLE).exe

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp $*.h -o $*.o


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

ServicerT.o: ServicerT.h ServicerT.cpp
RequesterT.o: RequesterT.h RequesterT.cpp


clean: 
	rm -f $(OBJECTS) $(EXECUTABLE) $(TESTS)

.PHONY: all clean alltests debug release
.SUFFIXES: