#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class CheckBox : public Child
{
  Q_OBJECT

public:
  CheckBox(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();
  string iconFile;

private slots:
  void stateChanged();

};

#endif
