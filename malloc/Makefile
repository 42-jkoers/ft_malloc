NAME			= bin/libmalloc.so

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

FLAGS			= -Wall -Wextra -Werror -fpic

ifdef PRODUCTION
FLAGS			+= -O3 -D PRODUCTION
else
FLAGS 			+= -g3
endif

LIBS			= $(LIBDIR)/libft/bin/libft.a

UNAME_S			= $(shell uname -s)
ifeq ($(UNAME_S), Linux)
NCPU 			= $(shell nproc --all)
LIBFLAGS		= -fPIC -shared
else ifeq ($(UNAME_S), Darwin)
NCPU 			= $(shell sysctl -n hw.ncpu)
LIBFLAGS		= -fPIC -dynamiclib
else
$(error Windows is, and should never be supported)
endif

MAKEFLAGS		+="j $(NCPU) $(NAME)"

ifeq ($(HOSTTYPE),)
HOSTTYPE 		= $(shell uname -m)_$(shell uname -s)
endif

VPATH = $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')

.SUFFIXES:

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: $(NAME)

print_vars:
	$(foreach v, $(.VARIABLES), $(if $(filter file,$(origin $(v))), $(info $(v)=$($(v)))))


$(NAME): $(BINDIR) $(OBJS) $(LIBS)
	$(CC) $(LIBFLAGS) $(BUILDDIR)/*.$(OBJEXT) -o $(NAME)

$(LIBDIR)/libft/bin/libft.a:
	$(MAKE) -C $(LIBDIR)/libft

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) $(BUILDDIR)
	$(CC) $(FLAGS) -I$(HEADERDIR) -c $< -o $(BUILDDIR)/$(notdir $@)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	$(MAKE) -C $(LIBDIR)/libft fclean
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif
	rm -rf $(BINDIR)

re:
	$(MAKE) clean
	rm -f $(NAME)
	$(MAKE)
