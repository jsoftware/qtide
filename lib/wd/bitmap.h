#ifndef BITMAP_H
#define BITMAP_H

#include "../base/base.h"

extern "C" {
  Dllexport void *wdreadimg(char *s, int *wh);
  Dllexport void *wdgetimg(uchar *data, int len, int *wh);
  Dllexport int wdwriteimg(uchar *p, int *wh, char *f, char *format, int quality);
  Dllexport void *wdputimg(uchar *p, int *wh, int *len, char *format, int quality);
}

#endif
