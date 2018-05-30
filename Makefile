CPP := g++
CPPFLAGS := -std=c++14 -Wall -g
MYFLAGS ?=

LIB := -L./
INC := -I./

LDLIBS := -lgsl -lcblas
# LDLIBS := -lgsl

# MODULES := learning_model,utils
SRCS := $(wildcard src/learning_model/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

CATCH_TST := tests/tests-main
TST_SRCS := $(filter-out $(CATCH_TST).cpp, $(wildcard tests/*.cpp))
TST_OBJS := $(patsubst %.cpp,%.o,$(TST_SRCS))

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(INC) -c -o $@ $<

$(CATCH_TST).o: $(CATCH_TST).cpp
	$(CPP) $(CPPFLAGS) $(INC) -c -o $@ $<

test: $(CATCH_TST).o $(OBJS) $(TST_SRCS)
	$(CPP) $(CPPFLAGS) $(INC) $(LDLIBS) -o $@ $^ && ./test

clean:
	rm *.o

.PHONY: clean test
