#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <string>

#include "child.h"

class Form;
class Pane;

class ScrollArea : public Child
{
  Q_OBJECT

public:
  ScrollArea(std::string n, std::string s, Form *f, Pane *p);
  ~ScrollArea();
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

protected:

private:
  Child *view;

};

#endif

