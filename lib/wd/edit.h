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
  void set(string p, string v);
  string state();

private slots:
  void returnPressed();

};

#endif
