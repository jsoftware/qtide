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
  Isidraw(std::string n, std::string s, Form *f, Pane *p);
  void setform();
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();
};

#endif
