#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class ComboBox : public Child
{
  Q_OBJECT

public:
  ComboBox(string n, string s, Form *f, Pane *p);
  void set(string p);
  void setp(string p,string v);
  string state();
  void setminwh(int,int);

private slots:
  void activated();

};

#endif
