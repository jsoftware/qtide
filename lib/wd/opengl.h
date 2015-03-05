#ifndef OPENGL_H
#define OPENGL_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Opengl : public Child
{
  Q_OBJECT

public:
  Opengl(string n, string s, Form *f, Pane *p);
  virtual ~Opengl();
  void setform();
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:

};

#endif
