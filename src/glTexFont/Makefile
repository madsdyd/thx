# Compiles these files into a bunch of .o files
CC=g++
CFLAGS=-Wall -D_REENTRANT -g # -pg -pedantic 
SOURCES=utility.cpp tga.cpp gltga.cpp text.cpp
OBS=$(SOURCES:.cpp=.o)
all:	$(OBS)

clean:	
	rm -f *~ *.o *.d

%.o:	%.cpp
	$(CC) $(CFLAGS) -c $<

######################################################################
#
# Automatic dependencies

%.d:	%.cpp
	@$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< \
        | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
          [ -s $@ ] || rm -f $@'

######################################################################
#
# Include automatic dependencies

include $(SOURCES:.cpp=.d)
