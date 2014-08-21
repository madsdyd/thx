dnl ------------------------------------------------------------
dnl Setup initial stuff
AC_INIT([thx], [0.3.6])
AC_CONFIG_SRCDIR([src/client.cc])
AM_INIT_AUTOMAKE
dnl Enable --maintainer-mode
AM_MAINTAINER_MODE
dnl We want a config header
AM_CONFIG_HEADER(src/config.h)

dnl Make sure the flags are blank
CPPFLAGS=""
CFLAGS=""
CXXFLAGS=""

dnl ------------------------------------------------------------
dnl Check system type
dnl AC_CANONICAL_HOST

dnl ------------------------------------------------------------
dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_RANLIB

dnl ------------------------------------------------------------
dnl Checks for libraries.

dnl Check for the math lib
AC_CHECK_LIB(m, main)


dnl ------------------------------------------------------------
dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(sys/time.h unistd.h)

dnl ------------------------------------------------------------
dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME

dnl ------------------------------------------------------------
dnl Checks for library functions.

dnl Check if gcc need -traditional to use ioctl's
dnl We do not need this, AFAIK.
dnl AC_PROG_GCC_TRADITIONAL

dnl Check if the system have gettimeofday
AC_CHECK_FUNCS(gettimeofday)

dnl This checks for opengl. 
dnl MDL_HAVE_OPENGL()
AC_PATH_X
AC_PATH_XTRA
gl_error=no
AC_CHECK_HEADERS(GL/gl.h GL/glu.h GL/glut.h,,gl_error=yes)
AC_CHECK_LIB(GL, glBegin,,gl_error=yes, [$X_CFLAGS $X_LIBS])
AC_CHECK_LIB(GLU, gluBuild2DMipmaps,,gl_error=yes, [$X_CFLAGS $X_LIBS])
AC_CHECK_LIB(glut, glutEntryFunc,,gl_error=yes, [$X_CFLAGS $X_LIBS])
if test "x$gl_error" != "xno" ; then
    AC_MSG_ERROR([
*** One or more of GL, GLU and glut could not be found
*** You need a working OpenGL/Mesa3D installation to
*** compile and use $PACKAGE
])
fi
dnl AC_MSG_RESULT([$PACKAGE is using the following for compiling and linking OpenGL programs: 
dnl   GL_CFLAGS: $GL_CFLAGS
dnl    GL_LIBS: $GL_LIBS])                                                

dnl --------------------------------------------------------------------------
dnl General options
dnl Shamelessly stolen from TuxRacer
dnl --------------------------------------------------------------------------
TR_CPPFLAGS=""
TR_CFLAGS="-Wall -Wtraditional -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual $X_CFLAGS"
TR_CXXFLAGS="-Wall -Wpointer-arith -Wconversion -Woverloaded-virtual $X_CFLAGS"
TR_OP_CFLAGS="-O2 -fomit-frame-pointer -ffast-math -fexpensive-optimizations"
TR_OP_CXXFLAGS="-O2 -fomit-frame-pointer -ffast-math -fexpensive-optimizations"
TR_LIBS="$X_LIBS"

case "$host" in
i*86-*-*) TR_OP_CFLAGS="$TR_OP_CFLAGS -malign-loops=2 -malign-jumps=2 -malign-functions=2";
          TR_OP_CXXFLAGS="$TR_OP_CXXFLAGS -malign-loops=2 -malign-jumps=2 -malign-functions=2";;
alpha*-*-linux-*) TR_CFLAGS="$TR_CFLAGS -mieee";;
esac

AC_ARG_ENABLE(sound,     [  --disable-sound         Produce an executable without simple OSS support],
 			 [disable_sound=yes], [disable_sound=no; AC_DEFINE(SOUND_ON, 1, Includes simple OSS support)])

if test "x$disable_sound" == "xno"; then
  AC_CHECK_HEADER(pthread.h, 
    [TR_LIBS="$TR_LIBS -lpthread"; TR_CFLAGS="$TR_CFLAGS -D_REENTRANT"; TR_CXXFLAGS="$TR_CXXFLAGS -D_REENTRANT"],
    [AC_MSG_ERROR([
*** Sound support currently requires the pthreads library
*** Install the pthreads library, or run configure with the
*** --disable-sound option])]
)
fi

dnl TODO: Ikke oversætte alle sources... 

AC_ARG_ENABLE(debug,     [  --disable-debug         Produce an executable without debugging symbols], 
                         , [AC_DEFINE(DEBUG, 1, High level debug setting) TR_CFLAGS="-g $TR_CFLAGS"; TR_CXXFLAGS="-g $TR_CXXFLAGS"])

AC_ARG_ENABLE(optimize,  [  --enable-optimize       Enable various optimizations. Looooong compile time],
			 [TR_CFLAGS="$TR_CFLAGS $TR_OP_CFLAGS"; TR_CXXFLAGS="$TR_CXXFLAGS $TR_OP_CXXFLAGS"])

AC_ARG_ENABLE(pedantic,  [  --enable-pedantic       Enable -pedantic (and -ansi for C) when compiling], 
                         [TR_CFLAGS="$TR_CFLAGS -ansi -pedantic"; TR_CXXFLAGS="$TR_CXXFLAGS -pedantic"])

AC_ARG_ENABLE(profile,   [  --enable-profile        Produce a profiled executable],
                         [TR_CFLAGS="$TR_CFLAGS -pg"; TR_CXXFLAGS="$TR_CXXFLAGS -pg"])


dnl --------------------------------------------------------------------------
dnl Other options
dnl --------------------------------------------------------------------------

dnl Specify data directory - stolen from tuxkart

dnl MUST FIX: Double expansion, something. This sucks. And is wrong
MY_EXPAND_DIR([thx_datadir], [$prefix/share/games/$PACKAGE])
#AC_DEFINE_UNQUOTED([THX_DATADIR], [$thx_datadir], Defines the location of the data directory)
AC_DEFINE_UNQUOTED([THX_DATADIR], "$thx_datadir", Defines the location of the data directory)

dnl AC_ARG_WITH(data-dir, [  --with-data-dir=PATH    Default thx data directory], 
dnl                      TR_CPPFLAGS="$TR_CPPFLAGS -DDATA_DIR=\\\"$withval\\\"")

dnl Set the final compiler and linker flags

CPPFLAGS="$CPPFLAGS $TR_CPPFLAGS"
CFLAGS="$CFLAGS $TR_CFLAGS"
CXXFLAGS="$CXXFLAGS $TR_CXXFLAGS"
LIBS="$LIBS $TR_LIBS"

AC_OUTPUT(src/Makefile src/glTexFont/Makefile data/Makefile data/graphics/Makefile data/graphics/fonts/Makefile data/graphics/textures/Makefile data/sounds/Makefile Makefile)
