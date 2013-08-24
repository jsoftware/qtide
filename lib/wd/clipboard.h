#ifndef CLIPBOARD_H
#define CLIPBOARD_H

extern "C" {
  Dllexport int wdclipwrite(char * s);
  Dllexport int wdclipwriteimage(char * s);
  Dllexport int wdclipwriteimagedata(const uchar * data, int len);
  Dllexport void *wdclipread(int *len);
  Dllexport void *wdclipreadimage(char * s);
}

#endif
