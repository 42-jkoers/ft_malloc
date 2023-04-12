# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jkoers <jkoers@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2020/11/05 15:36:08 by jkoers        #+#    #+#                  #
#    Updated: 2021/01/17 13:35:42 by jkoers        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME			= malloc

SRCEXT      	= c
SRCDIR      	= src
OBJEXT      	= o
HEADEREXT		= h
HEADERDIR		= include
BUILDDIR    	= obj
LIBDIR			= lib
SRC				= $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJS			= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))
HEADERS 		= $(shell find $(HEADERDIR) -name '*.$(HEADEREXT)')

FLAGS			= -Wall -Wextra -Werror

ifdef PRODUCTION
FLAGS			+= -O3 -D PRODUCTION
else
FLAGS 			+= -g3 -fsanitize=address
endif

LIBS			=

UNAME_S			= $(shell uname -s)
ifeq ($(UNAME_S), Linux)
NCPU 			= $(shell nproc --all)
else ifeq ($(UNAME_S), Darwin)
NCPU 			= $(shell sysctl -n hw.ncpu)
else
NCPU 			= 4
endif
MAKEFLAGS		+="j $(NCPU) $(NAME)"

VPATH = $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: $(NAME)
	@echo Build complete

$(NAME): $(BUILDDIR) $(OBJS) $(HEADERS) $(LIBDIR)/libft/bin/libft.a
	$(CC) -o $@ $(OBJS) $(FLAGS) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) $(FLAGS) -c -o $(BUILDDIR)/$(notdir $@) $<

$(LIBDIR)/libft/bin/libft.a:
	$(MAKE) -C $(LIBDIR)/libft

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	$(MAKE) -C $(LIBDIR)/libft fclean
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif
	rm -f $(NAME)

re:
	$(MAKE) clean
	rm -f $(NAME)
	$(MAKE)
