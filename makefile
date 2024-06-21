CC = gcc

PROJECT = pbt
SRCDIR = src    
TESTDIR = test                                                       
OBJDIR = build
BINDIR = bin

# using gnu99 instead of c99 to remove python timespec error
CFLAGS := -g -Wall -O0 -std=gnu99

.PHONY: build clean asm

#------------------------------------------
# GET
#------------------------------------------
# get the sources
SRCS := $(shell find $(SRCDIR) -name *.c)
TESTS := $(shell find $(TESTDIR) -name *.c)

# get the includes
COMMONINCS := -I$(SRCDIR) -I/usr/include/python3.10
COMMONLIBS := -lpython3.10 -lraylib -lGL -lm -ldl

# compiled obj
BINOBJS := $(SRCS:%=$(OBJDIR)/%.o)
BINASM := $(SRCS:%=$(OBJDIR)/%.s)
TESTBINOBJ := $(filter-out %main.c.o, $(BINOBJS)) $(TESTS:%=$(OBJDIR)/%.o)
TESTBINASM := $(filter-out %main.c.s, $(BINOBJS)) $(TESTS:%=$(OBJDIR)/%.s)

#------------------------------------------
# BUILD
#------------------------------------------
build: $(BINDIR)/$(PROJECT) $(BINDIR)/test makefile
	@./$(BINDIR)/test

asm: $(BINASM) $(TESTBINASM)

$(BINDIR)/$(PROJECT): $(BINOBJS) makefile
	@mkdir -p $(BINDIR)
	$(CC)   $(BINOBJS) -o $@ $(COMMONLIBS)

$(BINDIR)/test: $(TESTBINOBJ) makefile
	@mkdir -p $(BINDIR)
	$(CC)   $(TESTBINOBJ) -o $@ $(COMMONLIBS)

$(OBJDIR)/%.c.o : %.c makefile
	@mkdir -p $(dir $@)
	$(CC) $(COMMONINCS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.c.s : %.c makefile
	@mkdir -p $(dir $@)
	$(CC) $(COMMONINCS) $(CFLAGS) -S $< -o $@

#------------------------------------------
# clean
#------------------------------------------
clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*