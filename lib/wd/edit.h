#ifndef EDIT_H
#define EDIT_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Edit : public Child
{
  Q_OBJECT

public:
  Edit(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:
  void returnPressed();
  void textChanged();

private:
  bool focusSelect;
  bool uppercase;

};

#endif
