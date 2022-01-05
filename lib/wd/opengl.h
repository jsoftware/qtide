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
  Opengl(std::string n, std::string s, Form *f, Pane *p);
  virtual ~Opengl();
  void setform();
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:

};

#endif
