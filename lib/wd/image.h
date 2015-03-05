#ifndef IMAGE_H
#define IMAGE_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class Image : public Child
{
  Q_OBJECT

public:
  Image(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private:
  QLabel *lab;
  string imageFile;

};

#endif
