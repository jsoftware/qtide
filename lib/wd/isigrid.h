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
  IsiGrid(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

  QString style;
};

#endif
