
#ifndef GEGL_EXPORT_H
#define GEGL_EXPORT_H

#ifdef GEGL_STATIC_DEFINE
#  define GEGL_EXPORT
#  define GEGL_NO_EXPORT
#else
#  ifndef GEGL_EXPORT
#    ifdef geGL_EXPORTS
        /* We are building this library */
#      define GEGL_EXPORT 
#    else
        /* We are using this library */
#      define GEGL_EXPORT 
#    endif
#  endif

#  ifndef GEGL_NO_EXPORT
#    define GEGL_NO_EXPORT 
#  endif
#endif

#ifndef GEGL_DEPRECATED
#  define GEGL_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef GEGL_DEPRECATED_EXPORT
#  define GEGL_DEPRECATED_EXPORT GEGL_EXPORT GEGL_DEPRECATED
#endif

#ifndef GEGL_DEPRECATED_NO_EXPORT
#  define GEGL_DEPRECATED_NO_EXPORT GEGL_NO_EXPORT GEGL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef GEGL_NO_DEPRECATED
#    define GEGL_NO_DEPRECATED
#  endif
#endif

#endif /* GEGL_EXPORT_H */
