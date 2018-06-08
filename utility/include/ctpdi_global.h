#ifndef CTPDI_GLOBAL_H
#define CTPDI_GLOBAL_H


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

#if defined(CTPDI_LIBRARY)
#  define CTPDI_API SHARED_LIBRARY_EXPORT
#else
#  define CTPDI_API SHARED_LIBRARY_IMPORT
#endif


#endif // CTPDI_GLOBAL_H
