SUBDIRS = glTexFont

bin_PROGRAMS = thx
thx_LDADD = glTexFont/libglTexFont.a

# Note, the order of source files is important!
# This is because there are dependencies on the initilization 
# of the global c++ objects, which sucks!

thx_SOURCES = aicasper.cc aibaltazar.cc aifactory.cc aiplayer.cc \
	sound.cc menuitem_list.cc \
	menu_players.cc menuitem_color.cc menu_controls.cc \
	inputmouse.cc debug.cc gamemode.cc command.cc commandconsumer.cc \
	inputconsumer.cc inputkeyboard.cc inputevent.cc console.cc \
	types.cc game.cc display.cc tank.cc map.cc \
	main.cc player.cc render.cc viewpoint.cc framerate.cc server.cc \
	entities.cc projectile.cc explosion.cc textures.cc \
	marker.cc menu.cc menuitem.cc color.cc text.cc vector.cc \
	menu_game.cc client.cc menu_round.cc inventory.cc menu_buy.cc \
	menu_score.cc \
	lup.cc collision.cc object.cc point.cc triangle.cc models.cc 

# The headers order does not matter. None of these are installed

noinst_HEADERS = aibaltazar.hh aicasper.hh aifactory.hh aiplayer.hh client.hh collision.hh color.hh command.hh commandconsumer.hh console.hh debug.hh display.hh entities.hh explosion.hh framerate.hh game.hh gamemode.hh inputconsumer.hh inputevent.hh inputkeyboard.hh inputmouse.hh inventory.hh lup.hh map.hh marker.hh menu.hh menu_buy.hh menu_controls.hh menu_game.hh menu_players.hh menu_round.hh menu_score.hh menuitem.hh menuitem_color.hh menuitem_list.hh models.hh object.hh player.hh point.hh projectile.hh render.hh server.hh sound.hh tank.hh text.hh textures.hh triangle.hh types.hh vector.hh viewpoint.hh 