NAME			= tester
SO				= ../malloc/bin/libmalloc.so

SRCEXT      	= c
SRCDIR      	= src
OBJEXT      	= o
HEADEREXT		= h
HEADERDIR		= include
BUILDDIR    	= obj
LIBDIR			= lib
BINDIR			= bin
SRC				= $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJS			= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))
HEADERS 		= $(shell find $(HEADERDIR) -name '*.$(HEADEREXT)')

FLAGS			= -Wall -Wextra -Werror

ifdef PRODUCTION
FLAGS			+= -O3 -D PRODUCTION
else
FLAGS 			+= -g3
endif

LIBS			= -L../malloc/bin -lmalloc

UNAME_S			= $(shell uname -s)
ifeq ($(UNAME_S), Linux)
NCPU 			= $(shell nproc --all)
else ifeq ($(UNAME_S), Darwin)
NCPU 			= $(shell sysctl -n hw.ncpu)
else
$(error Windows is, and should never be supported)
endif
MAKEFLAGS		+="j $(NCPU) $(NAME)"

VPATH = $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')

.SUFFIXES:

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: $(BINDIR) $(NAME)
	$(MAKE) -C ../malloc
	@cp ../malloc/bin/libmalloc.so bin/libmalloc.so
	@echo Build complete
	@echo

$(NAME): $(SO) $(BUILDDIR) $(OBJS) $(HEADERS)
	$(CC) -o $@ $(OBJS) $(FLAGS) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) -I$(HEADERDIR) -c -o $(BUILDDIR)/$(notdir $@) $< $(LIBS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

test:
	@$(MAKE) all 2>&1 | grep -v "forced in submake: disabling jobserver mode."
	@LD_LIBRARY_PATH=/home/joppe/git/malloc/malloc/bin ./tester

clean:
	make -C ../malloc clean
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif
	rm -f $(NAME)
	rm -rf $(BINDIR)

re:
	$(MAKE) clean
	rm -f $(NAME)
	$(MAKE)

$(BINDIR):
	mkdir -p $(BINDIR)
$(SO):
	$(MAKE) -C ../malloc
# Dirty hack below
	cp ../malloc/bin/libmalloc.so $(BINDIR)
