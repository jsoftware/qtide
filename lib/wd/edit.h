#ifndef EDIT_H
#define EDIT_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class Edit : public Child
{
  Q_OBJECT

public:
  Edit(string s, string p, Form *f);
  void set(string p);
  string state();

private slots:
  void returnPressed();

};

#endif
