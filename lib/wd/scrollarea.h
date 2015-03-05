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
  ScrollArea(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

protected:

private:
  Child *view;

};

#endif

