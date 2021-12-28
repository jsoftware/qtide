#ifndef QTWEBSOCKET_COMPAT_H
#define QTWEBSOCKET_COMPAT_H


// add missing definitions
#ifndef QT50
#include <QtCore/qglobal.h>
typedef qptrdiff qintptr;
#endif

#endif
