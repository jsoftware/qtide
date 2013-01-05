#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class RadioButton : public Child
{
  Q_OBJECT

public:
  RadioButton(string s, string p, Form *f);
  void set(string p);
  void setp(string p,string v);
  string state();

private slots:
  void toggled(bool checked);

};

#endif
