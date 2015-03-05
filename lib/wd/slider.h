#ifndef SLIDER_H
#define SLIDER_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class Slider : public Child
{
  Q_OBJECT

public:
  Slider(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:
  void valueChanged();

private:
  int min;
  int max;

};

#endif
