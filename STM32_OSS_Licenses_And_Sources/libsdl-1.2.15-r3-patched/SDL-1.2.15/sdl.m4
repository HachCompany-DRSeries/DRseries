# Configure paths for SDL
# Sam Lantinga 9/21/99
# stolen from Manish Singh
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

# serial 1

dnl AM_PATH_SDL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for SDL, and define SDL_CFLAGS and SDL_LIBS
dnl
AC_DEFUN([AM_PATH_SDL],
[dnl 
  min_sdl_version=ifelse([$1], ,0.11.0,$1)
  PKG_CHECK_MODULES([SDL], [sdl >= $min_sdl_version])
  AC_SUBST(SDL_CFLAGS)
  AC_SUBST(SDL_LIBS)
])
