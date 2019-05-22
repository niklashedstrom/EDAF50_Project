# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CPPFLAGS =  -std=c++11 -I.
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS =   -g 
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Targets
PROGS = server client

all: libclientserver.a $(PROGS)
	make -C test

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: connection.o server.o
	ar rv libclientserver.a  connection.o server.o
	ranlib libclientserver.a

server: yourserver.o connection.o server.o diskdatabase.o inmemdatabase.o messagehandler.o
	g++ yourserver.cc server.cc connection.cc inmemdatabase.cc diskdatabase.cc messagehandler.cc -l sqlite3 -o server
client: client.o messagehandler.o connection.o

bin:
		mkdir ../bin
cpserver:
		cp server ../bin
cpclient: 
		cp client ../bin

install: bin cpserver cpclient


# Phony targets
.PHONY: all clean distclean

SRC = $(wildcard *.cc)

# Standard clean
clean:
	rm $(SRC:.cc=.o) $(PROGS)

distclean: clean
	-rm libclientserver.a
	-rm $(SRC:.cc=.d) 
	make -C test distclean


# Include the *.d files
-include $(SRC:.cc=.d)
