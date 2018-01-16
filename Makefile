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

HEADERS = 

all: $(EXECUTABLE)

release: CXXFLAGS += -DNDEBUG
release: all

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean all

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(HEADERS) $(LIBS) -o $(EXECUTABLE).exe

define make_tests
    SRCS = $$(filter-out $$(PROJECTFILE), $$(SOURCES))
    HDRS = $$(wildcard *.h)
    $(1): CXXFLAGS += $$(DEBUGFLAGS)
    $(1): $$(HDRS) $(1).cpp
	$$(CXX) $$(CXXFLAGS) $$(LIBS) $(1).cpp -o $(1).exe
endef
$(foreach test, $(TESTS), $(eval $(call make_tests, $(test))))

alltests: clean $(TESTS)


clean: 
	rm -f $(EXECUTABLE) $(TESTS)


.PHONY: all clean alltests debug release