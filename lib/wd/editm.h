#ifndef EDITM_H
#define EDITM_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class Editm : public Child
{
  Q_OBJECT

public:
  Editm(string s, string p, Form *f);
  void set(string p);
  string state();

private slots:

};

#endif
