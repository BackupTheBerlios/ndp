# Configure path for the Partition Of Unity
# Arcila Thomas 
# based on Christopher R. Gabriel <cgabriel@linux.it>, April 2000


AC_DEFUN(AM_PATH_POU,
[
AC_ARG_WITH(pou-prefix,[  --with-pou-prefix=PFX   Prefix where POU is installed (optional)],
            pou_prefix="$withval", pou_prefix="")
AC_ARG_WITH(pou-exec-prefix,[  --with-pou-exec-prefix=PFX Exec prefix where POU is installed (optional)],
            pou_exec_prefix="$withval", pou_exec_prefix="")
AC_ARG_ENABLE(poutest, [  --disable-poutest       Do not try to compile and run a test POU program],
		    , enable_poutest=yes)

  if test "x${POU_CONFIG+set}" != xset ; then
     if test "x$pou_prefix" != x ; then
         POU_CONFIG="$pou_prefix/bin/pou-config"
     fi
     if test "x$pou_exec_prefix" != x ; then
        POU_CONFIG="$pou_exec_prefix/bin/pou-config"
     fi
  fi

  AC_PATH_PROG(POU_CONFIG, pou-config, no)
  min_pou_version=ifelse([$1], ,0.1,$1)
  AC_MSG_CHECKING(for POU - version >= $min_pou_version)
  no_pou=""
  if test "$POU_CONFIG" = "no" ; then
    no_POU=yes
  else
    POU_CFLAGS=`$POU_CONFIG --cflags`
    POU_LIBS=`$POU_CONFIG --libs`

    pou_major_version=`$POU_CONFIG --version | \
           sed 's/^\([[0-9]]*\).*/\1/'`
    if test "x${pou_major_version}" = "x" ; then
       pou_major_version=0
    fi

    pou_minor_version=`$POU_CONFIG --version | \
           sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`
    if test "x${pou_minor_version}" = "x" ; then
       pou_minor_version=0
    fi

    pou_micro_version=`$POU_CONFIG --version | \
           sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`
    if test "x${pou_micro_version}" = "x" ; then
       pou_micro_version=0
    fi

    if test "x$enable_poutest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $POU_CFLAGS"
      LIBS="$LIBS $POU_LIBS"

      rm -f conf.poutest
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* my_strdup (const char *str);

char*
my_strdup (const char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = (char *)malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main (void)
{
  int major = 0, minor = 0, micro = 0;
  int n;
  char *tmp_version;

  system ("touch conf.poutest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_pou_version");

  n = sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) ;

  if (n != 2 && n != 3) {
     printf("%s, bad version string\n", "$min_pou_version");
     exit(1);
   }

   if (($pou_major_version > major) ||
      (($pou_major_version == major) && ($pou_minor_version > minor)) ||
      (($pou_major_version == major) && ($pou_minor_version == minor) && ($pou_micro_version >= micro)))
    {
      exit(0);
    }
  else
    {
      printf("\n*** 'pou-config --version' returned %d.%d.%d, but the minimum version\n", $pou_major_version, $pou_minor_version, $pou_micro_version);
      printf("*** of POU required is %d.%d.%d. If pou-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If pou-config was wrong, set the environment variable POU_CONFIG\n");
      printf("*** to point to the correct copy of pou-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      exit(1);
    }
}

],, no_pou=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_pou" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$POU_CONFIG" = "no" ; then
       echo "*** The pou-config script installed by POU could not be found"
       echo "*** If POU was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the POU_CONFIG environment variable to the"
       echo "*** full path to pou-config."
     else
       if test -f conf.gsltest ; then
        :
       else
          echo "*** Could not run POU test program, checking why..."
          CFLAGS="$CFLAGS $POU_CFLAGS"
          LIBS="$LIBS $POU_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding POU or finding the wrong"
          echo "*** version of POU. If it is not finding POU, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means POU was incorrectly installed"
          echo "*** or that you have moved POU since it was installed. In the latter case, you"
          echo "*** may want to edit the pou-config script: $POU_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
#     POU_CFLAGS=""
#     POU_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(POU_CFLAGS)
  AC_SUBST(POU_LIBS)
  rm -f conf.gsltest
])


