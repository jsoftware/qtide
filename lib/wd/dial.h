#ifndef DIAL_H
#define DIAL_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class Dial : public Child
{
  Q_OBJECT

public:
  Dial(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

private slots:
  void valueChanged();

private:
  int min;
  int max;

};

#endif
