#ifndef BUTTONS_H
#define BUTTONS_H

#include <QPushButton>
#include "child.h"
#include "form.h"

class Form;

// ---------------------------------------------------------------------
class Button : public Child
{
  Q_OBJECT

public:
  Button(string s, string p, Form *f);
  void setp(string p,string v);
};

#endif
