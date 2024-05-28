CC = gcc

PROJECT = pbt
SRCDIR = src    
TESTDIR = test                                                       
OBJDIR = build
BINDIR = bin

# using gnu99 instead of c99 to remove python timespec error
CFLAGS := -g -Wall -O0 -std=gnu99

.PHONY: build clean

#------------------------------------------
# GET
#------------------------------------------
# get the sources
SRCS := $(shell find $(SRCDIR) -name *.c)
TESTS := $(shell find $(TESTDIR) -name *.c)

# get the includes
COMMONINCS := -I$(SRCDIR) -I/usr/include/python3.10
COMMONLIBS := -lpython3.10

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
	$(CC)   $(BINOBJS) -o $@ $(COMMONLIBS)

$(BINDIR)/test: $(TESTBINOBJ) makefile
	$(CC)   $(TESTBINOBJ) -o $@ $(COMMONLIBS)

$(OBJDIR)/%.c.o : %.c makefile
	@mkdir -p $(dir $@)
	$(CC) $(COMMONINCS) $(CFLAGS) -c $< -o $@


#------------------------------------------
# clean
#------------------------------------------
clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*