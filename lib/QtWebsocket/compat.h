#ifndef QTWEBSOCKET_COMPAT_H
#define QTWEBSOCKET_COMPAT_H

// add missing RAND_MAX
#ifndef QT47
#include <cstdlib>
#endif

// add missing definitions
#ifndef QT50
#include <QtCore/qglobal.h>
typedef qptrdiff qintptr;
#endif

#endif
