# Define where to get our sounds
SOUNDDIR=
CC=g++
LFLAGS=-lm -lglut -lGLU -lGL -pthread
CFLAGS=-Wall -DREENTRANT -D_REENTRANT -g # -pg -pedantic # -DSOUNDDIR=$(SOUNDDIR)
SOURCES=tga.cpp gltga.cpp text.cpp
OBS=$(SOURCES:.cpp=.o)
all:	$(OBS)

clean:	
	rm -f ../core ../3dse *~ *.o ../gmon.out *.d

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
