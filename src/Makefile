# Define where to get our sounds
CC=g++
LFLAGS=-L/usr/X11R6/lib -lm -lglut -lGLU -lGL -lpthread

#WFLAGS=-Wtraditional -Wshadow -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Woverloaded-virtual 
WFLAGS=-Wtraditional -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual 
CFLAGS=-Wall $(WFLAGS) -I/usr/X11R6/include -pedantic -D_REENTRANT -DSOUND_ON -DDEBUG# -pg -pedantic 

SOURCES=menu_controls.cc \
	inputmouse.cc debug.cc gamemode.cc command.cc commandconsumer.cc \
	inputconsumer.cc inputkeyboard.cc inputevent.cc console.cc \
	types.cc game.cc sound.cc display.cc tank.cc map.cc \
	main.cc player.cc render.cc viewpoint.cc framerate.cc server.cc \
	entities.cc projectile.cc explosion.cc textures.cc \
	marker.cc menu.cc menuitem.cc color.cc text.cc vector.cc \
	menu_game.cc client.cc menu_round.cc inventory.cc menu_buy.cc \
	menu_score.cc \
	lup.cc collision.cc object.cc point.cc triangle.cc models.cc

OBS=$(SOURCES:.cc=.o)
TEXTOBS=$(addprefix glTexFont/,tga.o gltga.o text.o utility.o)
#TEXTOBS=glTexFont/gltexfont.o
all:	thx



clean:	
	rm -f ../core ../thx *~ *.o ../gmon.out *.d &&\
	cd glTexFont && make clean

######################################################################
#
# main target

thx:	../thx

../thx:	$(OBS) $(TEXTOBS)
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
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
        | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
          [ -s $@ ] || rm -f $@'

######################################################################
#
# Include automatic dependencies

-include $(SOURCES:.cc=.d)
