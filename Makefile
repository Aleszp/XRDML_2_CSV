SRCDIR=./src/
INCLUDEDIR=./include/
OBJDIR=./obj/
BINDIR=./bin/

CXX=gcc

CFLAGS=-pedantic -Wall -std=gnu99 -O3 -I$(INCLUDEDIR) 
LIBS= -lm

DEPS = messages.h
SRCS = main.c messages.c

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

