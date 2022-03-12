#ifndef HBOARD_GLOBAL_H
#define HBOARD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HBOARD_LIBRARY)
#  define HBOARD_EXPORT Q_DECL_EXPORT
#else
#  define HBOARD_EXPORT Q_DECL_IMPORT
#endif

#endif // HBOARD_GLOBAL_H
