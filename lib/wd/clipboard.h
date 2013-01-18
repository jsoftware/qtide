#ifndef CLIPBOARD_H
#define CLIPBOARD_H

extern "C" {
  int wdclipwrite(char * s);
  void *wdclipread(int *len);
}

#endif
