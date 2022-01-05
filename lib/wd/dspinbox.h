#ifndef DSPINBOX_H
#define DSPINBOX_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class DSpinBox : public Child
{
  Q_OBJECT

public:
  DSpinBox(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:
  void valueChanged();

private:
  double min;
  double max;
  int prec;

};

#endif
