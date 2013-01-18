#ifndef BITMAP_H
#define BITMAP_H

extern "C" {
  void *wdreadimg(char *s, int *wh);
  void *wdgetimg(uchar *data, int len, int *wh);
  int wdwriteimg(uchar *p, int *wh, char *f, char *format, int quality);
  void *wdputimg(uchar *p, int *wh, int *len, char *format, int quality);
}

#endif
