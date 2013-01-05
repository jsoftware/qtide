#ifndef DUMMY_H
#define DUMMY_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class Dummy : public Child
{
  Q_OBJECT

public:
  Dummy(string s, string p, Form *f);

private slots:

};

#endif
