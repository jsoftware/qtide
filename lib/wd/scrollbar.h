#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class ScrollBar : public Child
{
  Q_OBJECT

public:
  ScrollBar(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

private slots:
  void valueChanged();

private:
  int min;
  int max;

};

#endif
