#ifndef BUTTONS_H
#define BUTTONS_H

#include <QPushButton>
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Button : public Child
{
  Q_OBJECT

public:
  Button(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();
  std::string iconFile;

private slots:
  void clicked();
};

#endif
