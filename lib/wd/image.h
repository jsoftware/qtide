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
  void set(string p);
  void setp(string p,string v);

private:
  QLabel *lab;

};

#endif
