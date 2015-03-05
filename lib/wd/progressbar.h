#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class ProgressBar : public Child
{
  Q_OBJECT

public:
  ProgressBar(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:

private:
  int min;
  int max;

};

#endif
