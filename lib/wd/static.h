#ifndef STATIC_H
#define STATIC_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class Static : public Child
{
  Q_OBJECT

public:
  Static(string s, string p, Form *f);
  void set(string p);
  void setp(string p,string v);
  void setminwh(int,int);

private slots:

};

#endif
