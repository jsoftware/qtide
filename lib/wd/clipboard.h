#ifndef CLIPBOARD_H
#define CLIPBOARD_H

extern "C" {
  Dllexport int wdclipwrite(char * s);
  Dllexport void *wdclipread(int *len);
}

#endif
