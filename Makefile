#
# Part of program XRDML_2_CSV
# Makefile
# Author: mgr inż. Aleksander Szpakiewicz-Szatan
# (c) 2021-2022
# Licensed under GPL-3.0 License
# 

SRCDIR=./src/
INCLUDEDIR=./include/
OBJDIR=./obj/
BINDIR=./bin/

CXX=gcc

CFLAGS=-pedantic -Wall -std=gnu99 -O3 -I$(INCLUDEDIR) 
LIBS= -lm

DEPS = messages.h data.h setup.h
SRCS = main.c messages.c data.c setup.c

OBJS := $(addprefix $(OBJDIR),$(SRCS:.c=.o))
SRCS := $(addprefix $(SRCDIR),$(SRCS))
DEPS := $(addprefix $(INCLUDEDIR),$(DEP))
	
xrdml2csv: $(OBJS) 
	$(CXX) $(CFLAGS) $(LIBS) -o $(BINDIR)$@ $^  

$(OBJDIR)%.o: $(SRCDIR)%.c $(DEPS) dirs
	$(CXX) $(CFLAGS) -c -o $@ $< 

dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

.PHONY: clean all ver


all: xrdml2csv
clean:
	rm -f $(OBJDIR)*
	rm -f $(BINDIR)xrdml2csv
ver:
	$(CXX) --version

