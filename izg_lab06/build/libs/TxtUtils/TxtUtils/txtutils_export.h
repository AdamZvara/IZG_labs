
#ifndef TXTUTILS_EXPORT_H
#define TXTUTILS_EXPORT_H

#ifdef TXTUTILS_STATIC_DEFINE
#  define TXTUTILS_EXPORT
#  define TXTUTILS_NO_EXPORT
#else
#  ifndef TXTUTILS_EXPORT
#    ifdef TxtUtils_EXPORTS
        /* We are building this library */
#      define TXTUTILS_EXPORT 
#    else
        /* We are using this library */
#      define TXTUTILS_EXPORT 
#    endif
#  endif

#  ifndef TXTUTILS_NO_EXPORT
#    define TXTUTILS_NO_EXPORT 
#  endif
#endif

#ifndef TXTUTILS_DEPRECATED
#  define TXTUTILS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef TXTUTILS_DEPRECATED_EXPORT
#  define TXTUTILS_DEPRECATED_EXPORT TXTUTILS_EXPORT TXTUTILS_DEPRECATED
#endif

#ifndef TXTUTILS_DEPRECATED_NO_EXPORT
#  define TXTUTILS_DEPRECATED_NO_EXPORT TXTUTILS_NO_EXPORT TXTUTILS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TXTUTILS_NO_DEPRECATED
#    define TXTUTILS_NO_DEPRECATED
#  endif
#endif

#endif /* TXTUTILS_EXPORT_H */
