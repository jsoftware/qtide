#ifndef ISIGRAPH_H
#define ISIGRAPH_H

#include <string>

#include "child.h"

class Form;
class Pane;

class Isigraph : public Child
{
  Q_OBJECT

public:
  Isigraph(std::string n, std::string s, Form *f, Pane *p);
  void setform();
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();
};

#endif

