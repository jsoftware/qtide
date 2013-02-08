#ifndef STATIC_H
#define STATIC_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Static : public Child
{
  Q_OBJECT

public:
  Static(string n, string s, Form *f, Pane *p);
  void set(string p);
  void setp(string p,string v);
  void setminwh(int,int);

private slots:

};

#endif
