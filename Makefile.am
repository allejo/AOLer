lib_LTLIBRARIES = AOLer.la

AOLer_la_SOURCES = AOLer.cpp
AOLer_la_CPPFLAGS= -I$(top_srcdir)/include -I$(top_srcdir)/plugins/plugin_utils
AOLer_la_LDFLAGS = -module -avoid-version -shared
AOLer_la_LIBADD = $(top_builddir)/plugins/plugin_utils/libplugin_utils.la

AM_CPPFLAGS = $(CONF_CPPFLAGS)
AM_CFLAGS = $(CONF_CFLAGS)
AM_CXXFLAGS = $(CONF_CXXFLAGS)

EXTRA_DIST = \
	README.AOLer.txt \
	AOLer.def \
	AOLer.sln \
	AOLer.vcxproj

MAINTAINERCLEANFILES =	\
	Makefile.in
