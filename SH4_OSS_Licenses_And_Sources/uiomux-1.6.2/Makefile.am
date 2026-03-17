## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

DISTCHECK_CONFIGURE_FLAGS = --enable-gcc-werror

SUBDIRS = doc include src

# pkg-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = uiomux.pc

# Extra files to distribute in the source tarball.
EXTRA_DIST = uiomux.pc.in uiomux-uninstalled.pc.in
