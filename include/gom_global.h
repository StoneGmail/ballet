#ifndef GOM_GLOBAL_H
#define GOM_GLOBAL_H


// #  define SHARED_LIBRARY_EXPORT     __attribute__((visibility("default")))
// #  define SHARED_LIBRARY_IMPORT     __attribute__((visibility("default")))
// #  define SHARED_LIBRARY_HIDDEN     __attribute__((visibility("hidden")))

#ifndef SHARED_LIBRARY_EXPORT
#  ifdef _WINDOWS
#    define SHARED_LIBRARY_EXPORT     __declspec(dllexport)
#    define SHARED_LIBRARY_IMPORT     __declspec(dllimport)
#  elif defined(_UNIX)
#    define SHARED_LIBRARY_EXPORT
#    define SHARED_LIBRARY_IMPORT
#    define SHARED_LIBRARY_HIDDEN
#  endif
#endif

#if defined(GOM_LIBRARY)
#  define GOM_API SHARED_LIBRARY_EXPORT
#else
#  define GOM_API SHARED_LIBRARY_IMPORT
#endif



#endif // GOM_GLOBAL_H
