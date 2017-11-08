AM_CPPFLAGS = \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS = \
	-Wall\
	-g

lib_LTLIBRARIES = libgmr.la

libgmr_la_SOURCES = \
	gmr.h

nobase_pkginclude_HEADERS = \
	gmr.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gmr.pc

EXTRA_DIST = \
	gmr.pc.in