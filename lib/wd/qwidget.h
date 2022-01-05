#ifndef QWIDGEX_H
#define QWIDGEX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class QWidgex : public Child
{
  Q_OBJECT

public:
  QWidgex(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:

};

#endif
