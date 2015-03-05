#ifndef SPINBOX_H
#define SPINBOX_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class SpinBox : public Child
{
  Q_OBJECT

public:
  SpinBox(string n, string s, Form *f, Pane *p);
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
