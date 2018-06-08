#ifndef UTILITY_GLOBAL_H
#define UTILITY_GLOBAL_H


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


//SHARED_LIBRARY_EXPORT
#if defined(UTILITY_LIBRARY)
#  define UTILITY_API SHARED_LIBRARY_EXPORT
#else
#  define UTILITY_API SHARED_LIBRARY_IMPORT
#endif


#endif // UTILITY_GLOBAL_H
