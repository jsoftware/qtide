#ifndef DSPINBOX_H
#define DSPINBOX_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class DSpinBox : public Child
{
  Q_OBJECT

public:
  DSpinBox(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

private slots:
  void valueChanged();

private:
  double min;
  double max;
  int prec;

};

#endif
