#ifndef SLIDER_H
#define SLIDER_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class Slider : public Child
{
  Q_OBJECT

public:
  Slider(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:
  void valueChanged();

private:
  int min;
  int max;

};

#endif
