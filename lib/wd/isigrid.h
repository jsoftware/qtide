#ifndef DGRID_H
#define DGRID_H

#include "child.h"

class Form;
class Pane;
class QGrid;

// ---------------------------------------------------------------------
class IsiGrid : public Child
{
  Q_OBJECT

public:
  IsiGrid(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

  QString style;
};

#endif
