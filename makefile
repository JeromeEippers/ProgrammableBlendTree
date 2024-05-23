CC = gcc

PROJECT = pbt
SRCDIR = src    
TESTDIR = test                                                       
OBJDIR = build
BINDIR = bin

CFLAGS = -g -Wall -O3 -std=c99

.PHONY: build clean

#------------------------------------------
# GET
#------------------------------------------
# get the sources
SRCS := $(shell find $(SRCDIR) -name *.c)
TESTS := $(shell find $(TESTDIR) -name *.c)

# get the includes
COMMONINCS = -I$(SRCDIR)

# compiled obj
BINOBJS := $(SRCS:%=$(OBJDIR)/%.o)
TESTBINOBJ := $(filter-out %main.c.o, $(BINOBJS)) $(TESTS:%=$(OBJDIR)/%.o)


#------------------------------------------
# BUILD
#------------------------------------------
build: $(BINDIR)/$(PROJECT) $(BINDIR)/test makefile
	
	@./$(BINDIR)/test
	@./$(BINDIR)/$(PROJECT)

$(BINDIR)/$(PROJECT): $(BINOBJS) makefile
	$(CC) $(CFLAGS) $(BINOBJS) -o $@

$(BINDIR)/test: $(TESTBINOBJ) makefile
	$(CC) $(CFLAGS) $(TESTBINOBJ) -o $@

$(OBJDIR)/%.c.o : %.c makefile
	@mkdir -p $(dir $@)
	$(CC) $(COMMONINCS) $(CFLAGS) -c $< -o $@


#------------------------------------------
# clean
#------------------------------------------
clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*