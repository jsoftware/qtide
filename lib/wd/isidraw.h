#ifndef ISIDRAW_H
#define ISIDRAW_H

#include <string>

#include "child.h"
#include "isigraph.h"

class Form;
class Pane;

class Isidraw : public Child
{
  Q_OBJECT

public:
  Isidraw(string n, string s, Form *f, Pane *p);
  virtual ~Isidraw();
  void setform();
  string get(string p,string v);
  void set(string p,string v);
  string state();
};

#endif
