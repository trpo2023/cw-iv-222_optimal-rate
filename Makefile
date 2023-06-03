CC = gcc
CFLAGS = -Wall -Werror -Isrc/libo_rate -MMD 
TEST_CFLAGS = -Isrc/libo_rate -Ithirdparty -MMD 
LFLAGS = $(shell pkg-config --libs json-c)
TARGET = bin/optimal_rate
TEST_TARGET = bin/test
OBJDIR = obj
SRCDIR = src
TESTDIR = test

SRC_RATE_DIR = $(SRCDIR)/o_rate
SRC_LIBRATE_DIR = $(SRCDIR)/libo_rate
SRC_TEST_DIR = $(TESTDIR)
OBJ_RATE_DIR = $(OBJDIR)/o_rate
OBJ_LIBRATE_DIR = $(OBJDIR)/libo_rate
OBJ_TEST_DIR = $(OBJDIR)/test

SRC_RATE = $(wildcard $(SRC_RATE_DIR)/.c)
OBJ_RATE = $(patsubst $(SRC_RATE_DIR)/%.c, $(OBJ_RATE_DIR)/%.o, $(SRC_RATE))
DEP_RATE = $(OBJ_RATE:.o=.d)

SRC_LIBRATE = $(wildcard $(SRC_LIBRATE_DIR)/.c)
OBJ_LIBRATE = $(patsubst $(SRC_LIBRATE_DIR)/%.c, $(OBJ_LIBRATE_DIR)/%.o, $(SRC_LIBRATE))
DEP_LIBRATE = $(OBJ_LIBRATE:.o=.d)

SRC_TEST = $(wildcard $(SRC_TEST_DIR)/.c)
OBJ_TEST = $(patsubst $(SRC_TEST_DIR)/%.c, $(OBJ_TEST_DIR)/%.o, $(SRC_TEST))
DEP_TEST = $(OBJ_TEST:.o=.d)

RATEOBJS = $(OBJ_RATE_DIR)/main.o
LIBOBJS = $(OBJ_LIBRATE_DIR)/o_rate.o
LIBNAME = $(OBJ_LIBRATE_DIR)/o_rate.a

TESTOBJS = $(OBJ_TEST_DIR)/main.o
TESTLIBOBJS = $(OBJ_TEST_DIR)/test.o

.PHONY: all clean clean_all test

all: $(TARGET)

$(TARGET): $(RATEOBJS) $(LIBNAME)
	$(CC) $^ -o $@ $(LFLAGS)

$(OBJ_RATE_DIR)/%.o: $(SRC_RATE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_LIBRATE_DIR)/%.o: $(SRC_LIBRATE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBNAME): $(LIBOBJS)
	ar rcs $@ $^

-include $(DEP_RATE)
-include $(DEP_LIBRATE)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TESTOBJS) $(TESTLIBOBJS) $(LIBNAME)
	$(CC) $^ -o $@ $(LFLAGS)
	
$(OBJ_TEST_DIR)/%.o: $(SRC_TEST_DIR)/%.c $(LIBNAME) 
	$(CC) $(TEST_CFLAGS) -c $< -o $@


clean:
	$(RM) -r $(OBJ_RATE_DIR)/* $(OBJ_LIBRATE_DIR)/* $(LIBNAME) $(OBJ_TEST_DIR)/* $(TESTLIBNAME)

clean_all:
	$(RM) -r $(OBJ_RATE_DIR)/* $(OBJ_LIBRATE_DIR)/* $(TARGET) $(TEST_TARGET) $(LIBNAME) $(OBJ_TEST_DIR)/*
