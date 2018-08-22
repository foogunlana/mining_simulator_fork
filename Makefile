CPP := g++
CPPFLAGS := -std=c++14 -Wall -g
MYFLAGS ?=

LIB := -L./
INC := -I./

LDLIBS := -lgsl -lcblas
# LDLIBS := -lgsl

# MODULES := learning_model,utils
SRCS := $(wildcard src/learning_model/*.cpp)
SRCS += $(wildcard src/mining_game/*.cpp)
SRCS += $(wildcard src/strategy_behaviour/*.cpp)
SRCS += $(wildcard src/utils/*.cpp)

OBJS = $(patsubst %.cpp,%.o,$(SRCS))
MAIN := src/main.cpp

CATCH_TST := tests/test_main
TST_SRCS := $(filter-out $(CATCH_TST).cpp, $(wildcard tests/*.cpp))

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(INC) -c -o $@ $<

$(CATCH_TST).o: $(CATCH_TST).cpp
	$(CPP) $(CPPFLAGS) $(INC) -c -o $@ $<

test: $(CATCH_TST).o $(OBJS) $(TST_SRCS)
	$(CPP) $(CPPFLAGS) $(INC) $(LDLIBS) -o $@ $^

run: $(OBJS) $(MAIN)
	$(CPP) $(CPPFLAGS) $(INC) $(LDLIBS) -o $@ $^

clean:
	rm -f $(OBJS) test run

.PHONY: clean test run
