#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class CheckBox : public Child
{
  Q_OBJECT

public:
  CheckBox(string s, string p, Form *f);
  void set(string p);
  void setp(string p,string v);
  string state();

private slots:
  void stateChanged();

};

#endif
