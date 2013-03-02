#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class ProgressBar : public Child
{
  Q_OBJECT

public:
  ProgressBar(string n, string s, Form *f, Pane *p);
  void set(string p);
  void setp(string p,string v);

private slots:

private:
  int min;
  int max;

};

#endif
