#ifndef BASE_H
#define BASE_H

#ifdef _MSC_VER
#define Dllexport   __declspec( dllexport )
#else
#define Dllexport
#endif

#include <QDebug>
#include <stdint.h>
#include "util.h"
#include "utils.h"

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
#include <QtCore5Compat>
#endif

#endif
