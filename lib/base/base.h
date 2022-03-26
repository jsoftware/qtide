#ifndef BASE_H
#define BASE_H

#ifdef _MSC_VER
#define Dllexport   __declspec( dllexport )
#else
#define Dllexport
#endif

#include <QDebug>
#include <stdint.h>

#if defined(QT60)
#include <QtCore5Compat>
#else
#define playbackStateChanged stateChanged
#define PlaybackState State
#endif
#if defined(QT515)
#define _KeepEmptyParts Qt::KeepEmptyParts
#define _SkipEmptyParts Qt::SkipEmptyParts
#define setTabStopWidth setTabStopDistance
#else
#define _KeepEmptyParts QString::KeepEmptyParts
#define _SkipEmptyParts QString::SkipEmptyParts
#endif

#include "util.h"
#include "utils.h"

#endif
