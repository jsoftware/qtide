#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class RadioButton : public Child
{
  Q_OBJECT

public:
  RadioButton(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();
  std::string iconFile;

private slots:
  void toggled(bool checked);

};

#endif
