#ifndef HBOARD_GLOBAL_H
#define HBOARD_GLOBAL_H

#if defined(HBOARD_LIBRARY)
#define HBOARD_EXPORT __declspec(dllexport)
#else
#define HBOARD_EXPORT __declspec(dllimport)
#endif

#endif  // HBOARD_GLOBAL_H
