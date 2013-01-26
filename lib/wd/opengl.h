#ifndef OPENGL_H
#define OPENGL_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class Opengl : public Child
{
  Q_OBJECT

public:
  Opengl(string s, string p, Form *f);
  void setp(string p,string v);
  void setform();

private slots:

};

extern Opengl *opengl;

#endif
