#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class RadioButton : public Child
{
  Q_OBJECT

public:
  RadioButton(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:
  void toggled(bool checked);

};

#endif
