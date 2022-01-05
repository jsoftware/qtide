#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class CheckBox : public Child
{
  Q_OBJECT

public:
  CheckBox(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();
  std::string iconFile;

private slots:
  void stateChanged();

};

#endif
