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
  string get(string p,string v);
  void set(string p,string v);
  string state();

private:
  string getitems();

private slots:
  void activated();

};

#endif
