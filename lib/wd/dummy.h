#ifndef DUMMY_H
#define DUMMY_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Dummy : public Child
{
  Q_OBJECT

public:
  Dummy(string n, string s, Form *f, Pane *p);

private slots:

};

#endif
