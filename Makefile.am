SUBDIRS = src data

# Eventually, the data will probably be handled as a seperate package.
# For now, it is not worth it.
# The makefile here is the old makefile, which should probably go
# into the distribution, untill I am sure the configure stuff works.
EXTRA_DIST = bootstrap config CHANGELOG thx.xpm DOCS src/Makefile.linux.mdk80


# Hook to install our data. This is a hack, I think.
#install-data-hook:
#	echo cp -a $(srcdir)/data/* $(DATA_DIR)

# Hook for making sure that we do not distribute CVS stuff
dist-hook:
	find $(distdir) -type d -name "*CVS*" | xargs rm -Rf


