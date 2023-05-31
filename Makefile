CC = gcc
CFLAGS = -Wall -Werror -Isrc/libo_rate -MMD
TARGET = bin/optimal_rate
OBJDIR = obj
SRCDIR = src

SRC_RATE_DIR = $(SRCDIR)/o_rate
SRC_LIBRATE_DIR = $(SRCDIR)/libo_rate
OBJ_RATE_DIR = $(OBJDIR)/o_rate
OBJ_LIBRATE_DIR = $(OBJDIR)/libo_rate

SRC_RATE = $(wildcard $(SRC_RATE_DIR)/.c)
OBJ_RATE = $(patsubst $(SRC_RATE_DIR)/%.c, $(OBJ_RATE_DIR)/%.o, $(SRC_RATE))
DEP_RATE = $(OBJ_RATE:.o=.d)

SRC_LIBRATE = $(wildcard $(SRC_LIBRATE_DIR)/.c)
OBJ_LIBRATE = $(patsubst $(SRC_LIBRATE_DIR)/%.c, $(OBJ_LIBRATE_DIR)/%.o, $(SRC_LIBRATE))
DEP_LIBRATE = $(OBJ_LIBRATE:.o=.d)

RATEOBJS = $(OBJ_RATE_DIR)/main.o
LIBOBJS = $(OBJ_LIBRATE_DIR)/o_rate.o
LIBNAME = $(OBJ_LIBRATE_DIR)/o_rate.a

.PHONY: all clean clean_all

all: $(TARGET)

$(TARGET): $(RATEOBJS) $(LIBNAME)
    $(CC) $^ -o $@

$(OBJ_RATE_DIR)/%.o: $(SRC_RATE_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_LIBRATE_DIR)/%.o: $(SRC_LIBRATE_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@

$(LIBNAME): $(LIBOBJS)
    ar rcs $@ $^

-include $(DEP_RATE)
-include $(DEP_LIBRATE)

clean:
    $(RM) -r $(OBJ_RATE_DIR)/* $(OBJ_LIBRATE_DIR)/* $(LIBNAME)

clean_all:
    $(RM) -r $(OBJ_RATE_DIR)/* $(OBJ_LIBRATE_DIR)/* $(TARGET) $(LIBNAME)