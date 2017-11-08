dnl configure.ac of libgmr
dnl Process this file with auto tools
AC_PREREQ([2.59])
AC_INIT(libgmr, 0.0.1, bugs@bsgroup.org, gmr, http://gmr.bsgroup.org/)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.11 subdir-objects])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([disable])
AC_PREFIX_DEFAULT([/usr])
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_HEADER_STDC
PKG_PROG_PKG_CONFIG

test "$prefix" = NONE && prefix=/usr

AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_TYPE_SIZE_T

LT_INIT

AC_SEARCH_LIBS(socket, socket, [], [AC_MSG_ERROR([Socket needed])])
AC_SEARCH_LIBS(gethostbyname, nsl)
AC_SEARCH_LIBS(malloc_usable_size, malloc)
AC_SEARCH_LIBS(pthread_spin_lock, pthread, [], [AC_MSG_ERROR([Pthread needed])])
AC_SEARCH_LIBS(log2, m, [], [AC_MSG_ERROR([GNU math library needed])])
AC_SEARCH_LIBS(gethugepagesizes, hugetlbfs)

AC_CHECK_FUNCS([ \
    dup2 \
    gettimeofday \
    sigignore \
    mmap \
    getaddrinfo \
    munmap \
    strerror \
    strnlen
], [], [AC_MSG_ERROR([Functions missed])])

AC_CHECK_FUNCS([ \
    getpagesizes \
    memcntl
])

AC_CHECK_HEADERS([ \
    ctype.h \
    errno.h \
    error.h \
    malloc.h \
    math.h \
    inttypes.h \
    fcntl.h \
    limits.h \
    netdb.h \
    pthread.h \
    sched.h \
    signal.h \
    stdarg.h \
    stdint.h \
    stdlib.h \
    stdio.h \
    string.h \
    time.h \
    unistd.h \
    sys/stat.h \
    sys/socket.h \
    sys/sysinfo.h \
    sys/syscall.h \
    sys/uio.h \
    sys/mman.h \
    sys/resource.h \
    sys/time.h \
    sys/types.h \
    sys/un.h \
    net/if.h \
    netinet/tcp.h \
    netinet/udp.h \
    arpa/inet.h
], [], [AC_MSG_ERROR([System header missed])])

AC_CHECK_HEADERS([ \
    pthread_np.h \
    sys/select.h
])

AC_MSG_CHECKING([Operating System])
case $host in
*-linux*)
    # Linux
    AC_MSG_RESULT([Linux back-end])
    AC_DEFINE(OS_LINUX, 1, [Linux implementations])
    AC_SUBST(OS_LINUX)
    AC_CHECK_HEADERS([ \
        sys/epoll.h \
        sys/eventfd.h \
        sys/timerfd.h \
        sys/signalfd.h
    ], [
        AC_DEFINE(EVENT_USE_EPOLL, 1, [Epoll supported])
    ], [
        AC_DEFINE(EVENT_USE_SELECT, 1, [Epoll unsupported])
    ])
    ;;
*-darwin*|*-*bsd*)
    # MacOS & BSD
    AC_MSG_RESULT([BSD back-end])
    AC_DEFINE(OS_BSD, 1, [BSD implementations])
    AC_SUBST(OS_BSD)
    AC_CHECK_HEADERS([ \
        sys/event.h
    ], [
        AC_DEFINE(EVENT_USE_KQUEUE, 1, [Kqueue supported])
    ], [
        AC_DEFINE(EVENT_USE_SELECT, 1, [Kqueue unsupported])
    ])
    ;;
*)
    # Other system
    AC_MSG_RESULT([Other general OS])
    AC_DEFINE(OS_OTHER, 1, [Other general OS])
    AC_SUBST(OS_OTHER)
    AC_CHECK_HEADERS([ \
        sys/select.h
    ])
    AC_DEFINE(EVENT_USE_SELECT, 1, [Other OS])
    ;;
esac

CPPFLAGS="-I$PWD -I$PWD/src"
CFLAGS="-O3 -g -Wall"

AC_DEFUN([AC_C_ENDIAN],
[AC_CACHE_CHECK(for endianness, ac_cv_c_endian,
[
    AC_RUN_IFELSE(
        [AC_LANG_PROGRAM([], [
            long val = 1;
            char *c = (char *) &val;
            exit(*c == 1);
        ])
    ],[
        ac_cv_c_endian=big
    ],[
        ac_cv_c_endian=little
    ])
])
if test $ac_cv_c_endian = big; then
    AC_DEFINE(ENDIAN_BIG, 1, [machine is big-endian])
fi
if test $ac_cv_c_endian = little; then
    AC_DEFINE(ENDIAN_LITTLE, 1, [machine is little-endian])
fi
])

AC_C_ENDIAN

#dnl Lock
#dnl Default lock is original pthread lock
#dnl <spinlock> to show the pthread spinlock
#dnl <tinylock> means the built-in tiny-spinlock in libgmr
lock="default"
AC_ARG_WITH([lock],
    [AS_HELP_STRING([--with-lock=NAME], [Specify the thread lock: default / spinlock / tinylock])],
    [
        if test "x$withval" != "xdefault"; then
            lock=$withval
        fi
    ]
)

if test "$lock" = "spinlock"; then
    AC_DEFINE_UNQUOTED(MUTEX_LOCK, "spinlock", [Mutex: pthread spinlock])
    AC_SUBST([ac_cv_lock_spinlock], 1)
    AC_SUBST([ac_cv_lock_tinylock], 0)
    AC_SUBST([ac_cv_lock_default], 0)
elif test "$lock" = "tinylock"; then
    AC_DEFINE_UNQUOTED(MUTEX_LOCK, "tinylock", [Mutex: built-in tinylock])
    AC_SUBST([ac_cv_lock_spinlock], 0)
    AC_SUBST([ac_cv_lock_tinylock], 1)
    AC_SUBST([ac_cv_lock_default], 0)
else
    AC_DEFINE_UNQUOTED(MUTEX_LOCK, "default", [Mutex: pthread mutex])
    AC_SUBST([ac_cv_lock_spinlock], 0)
    AC_SUBST([ac_cv_lock_tinylock], 0)
    AC_SUBST([ac_cv_lock_default], 1)
fi

#dnl Memory allocator
#dnl Default allocator is malloc/free, and <lockless> allocator was built-in
#dnl You can set allocator by --with-allocator=[NAME] which name in (default(default) / lockless / jemalloc / tcmalloc)
#dnl The default memory allocator in modern linux distro is ptmalloc2 , jemalloc in *BSD, and OSX just allocate memory in kernel layer.
allocator="default"
AC_ARG_WITH([allocator],
    [AS_HELP_STRING([--with-allocator=NAME], [Specify the memory allocator: default / lockless (built-in) / jemalloc (third-party dependence) / tcmalloc (third-party dependence) / tcmalloc-minimal (without the heap profiler and checker) (third-party dependence)])],
    [
        if test "x$withval" != "xdefault"; then
            allocator=$withval
        fi
    ]
)

ll_dir="(built-in)"
je_dir="(system)"
tc_dir="(system)"
#dnl Self-specify path
AC_ARG_WITH([jemalloc-dir],
    [AS_HELP_STRING([--with-jemalloc-dir=DIR], [Specify the installation path of jemalloc])],
    [je_dir=$withval]
)

AC_ARG_WITH([tcmalloc-dir],
    [AS_HELP_STRING([--with-tcmalloc-dir=DIR], [Specify the installation path of tcmalloc])],
    [tc_dir=$withval]
)

#dnl Check allocator
if test "$allocator" = "jemalloc"; then
    # Jemalloc
    je_found=no
    pkg_has_jemalloc=no

    if test "$je_dir" = "(system)"; then
        # Pkg-config first
        PKG_CHECK_MODULES(Jemalloc, [jemalloc],
            [
                pkg_has_jemalloc=yes
                je_found=yes
            ],
            [
                pkg_has_jemalloc=no
            ]
        )
    fi

    if test "x$je_found" = "xno"; then
        # Check jemalloc from system wide
        saved_LIBS="$LIBS"
        saved_LDFLAGS="$LDFLAGS"
        saved_CPPFLAGS="$CPPFLAGS"
        AC_CACHE_CHECK([for libjemalloc installation], ac_cv_je_dir,
        [
            for je in $je_dir ./ /usr /usr/local; do
                LIBS="-ljemalloc $saved_LIBS"
                LDFLAGS="$saved_LDFLAGS"
                CPPFLAGS="$saved_CPPFLAGS"
                if [test -d "$je"] || [test $je = "(system)"]; then
                    # RH/CentOS
                    if test -d "$je/lib64"; then
                        LDFLAGS="-L$je/lib64 $saved_LDFLAGS"
                    # Debian etc ...
                    elif test -d "$je/lib"; then
                        LDFLAGS="-L$je/lib $saved_LDFLAGS"
                    # Or just (System)
                    else
                        LDFLAGS="-L$je $saved_LDFLAGS"
                    fi

                    # POSIX like
                    if test -d "$je/include"; then
                        CPPFLAGS="-I$je/include $saved_CPPFLAGS"
                    # No include directory found
                    else
                        CPPFLAGS="-I$je $saved_CPPFLAGS"
                    fi

                    # Try link
                    AC_TRY_LINK(
                        [#include <jemalloc/jemalloc.h>],
                        [malloc(1);],
                        [
                            je_linked=yes
                        ],
                        [
                            je_linked=no
                        ]
                    )
                    if test "x$je_linked"=xyes; then
                        ac_cv_je_dir=$je
                        je_found=yes
                        break
                    fi
                else
                    continue;
                fi
            done

            if test "x$je_found"="xno"; then
                AC_MSG_ERROR([Cannot find jemalloc])
            fi
        ])
    fi
elif [test "$allocator" = "tcmalloc"] || [test "$allocator" = "tcmalloc-minimal"]; then
    # Tcmalloc
    tc_found=no
    pkg_has_tcmalloc=no

    if test "$tc_dir" = "(system)"; then
        # pkg-config with "libtcmalloc"
        if test "$sallocator" = "tcmalloc"; then
            PKG_CHECK_MODULES(Tcmalloc, [libtcmalloc],
            [
                pkg_has_tcmalloc=standard
                tc_found=yes
            ],
            [
                pkg_has_tcmalloc=no
            ])
        else
            PKG_CHECK_MODULES(Tcmalloc_minimal, [libtcmalloc-minimal],
            [
                pkg_has_tcmalloc=minimal
                tc_found=yes
            ],
            [
                pkg_has_tcmalloc=no
            ])
        fi
    fi

    if test "x$tc_found" = "xno"; then
        # Check tcmalloc / tcmalloc-minimal
        saved_LIBS="$LIBS"
        saved_LDFLAGS="$LDFLAGS"
        saved_CPPFLAGS="$CPPFLAGS"
        AC_CACHE_CHECK([for libjemalloc installation], ac_cv_je_dir,
        [
            for tc in $tc_dir ./ /usr /usr/local; do
                if test "$allocator" = "tcmalloc"; then
                    LIBS="-ljemalloc $saved_LIBS"
                else
                    LIBS="-ljemalloc_minimal $saved_LIBS"
                fi
                LDFLAGS="$saved_LDFLAGS"
                CPPFLAGS="$saved_CPPFLAGS"
                if [test -d "$tc"] || [test $tc = "(system)"]; then
                    # RH/CentOS
                    if test -d "$tc/lib64"; then
                        LDFLAGS="-L$tc/lib64 $saved_LDFLAGS"
                    # Debian etc ...
                    elif test -d "$tc/lib"; then
                        LDFLAGS="-L$tc/lib $saved_LDFLAGS"
                    # Or just (System)
                    else
                        LDFLAGS="-L$tc $saved_LDFLAGS"
                    fi

                    # POSIX like
                    if test -d "$tc/include"; then
                        CPPFLAGS="-I$tc/include $saved_CPPFLAGS"
                    # No include directory found
                    else
                        CPPFLAGS="-I$tc $saved_CPPFLAGS"
                    fi

                    # Try link
                    AC_TRY_LINK(
                        [#include <gperftools/tcmalloc.h>],
                        [malloc(1);],
                        [
                            tc_linked=yes
                        ],
                        [
                            tc_linked=no
                        ]
                    )
                    if test "x$tc_linked"=xyes; then
                        ac_cv_tc_dir=$tc
                        tc_found=yes
                        break
                    fi
                else
                    continue;
                fi
            done

            if test "x$tc_found"="xno"; then
                AC_MSG_ERROR([Cannot find tcmalloc])
            fi
        ])
    fi
elif test "$allocator" = "lockless"; then
    ll_found=yes
else
    # Default malloc
    default_found=yes
fi

if test "$allocator" = "lockless"; then
    AC_SUBST([ac_cv_allocator_lockless], 1)
else
    AC_SUBST([ac_cv_allocator_lockless], 0)
fi

if test "$allocator" = "jemalloc"; then
    AC_SUBST([ac_cv_allocator_jemalloc], 1)
else
    AC_SUBST([ac_cv_allocator_jemalloc], 0)
fi

if [test "$allocator" = "tcmalloc"] || [test "$allocator" = "tcmalloc-minimal"]; then
    AC_SUBST([ac_cv_allocator_tcmalloc], 1)
else
    AC_SUBST([ac_cv_allocator_tcmalloc], 0)
fi

build_time=`date +%Y%m%d%H%M%S`
AC_SUBST([ac_cv_build_time], $build_time)
AC_CONFIG_FILES([gmr/gmr.h])

AC_OUTPUT([
Makefile
gmr/gmr.pc
gmr/Makefile
])
