# Define where to get our sounds
SOUNDDIR=
CC=g++
LFLAGS=-L/usr/X11R6/lib -lm -lglut -lGLU -lGL -lpthread
CFLAGS=-Wall -D_REENTRANT -g -DSOUND_ON # -pg -pedantic 
# -DSOUNDDIR=$(SOUNDDIR)

SOURCES=inputevent.cc console.cc \
	types.cc game.cc keyboard.cc sound.cc display.cc tank.cc map.cc \
	main.cc player.cc render.cc viewpoint.cc framerate.cc server.cc \
	entities.cc projectile.cc explosion.cc textures.cc \
	marker.cc menu.cc menuitem.cc color.cc text.cc vector.cc \
	menu_game.cc client.cc menu_round.cc inventory.cc menu_buy.cc \
	menu_score.cc \
	lup.cc collision.cc object.cc point.cc triangle.cc models.cc

OBS=$(SOURCES:.cc=.o)
TEXTOBS=$(addprefix glTexFont/,tga.o gltga.o text.o utility.o)
all:	thx

clean:	
	rm -f ../core ../thx *~ *.o ../gmon.out *.d &&\
	cd glTexFont && make clean

######################################################################
#
# main target

thx:	$(OBS) $(TEXTOBS)
	$(CC) $(CFLAGS) $(LFLAGS) -o ../thx $(TEXTOBS) $(OBS)

######################################################################
#
# .cc to .o

%.o:	%.cc
	$(CC) $(CFLAGS) -c $<

######################################################################
#
# Automatic dependencies

%.d:	%.cc
	@$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< \
        | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
          [ -s $@ ] || rm -f $@'

######################################################################
#
# Include automatic dependencies

-include $(SOURCES:.cc=.d)
