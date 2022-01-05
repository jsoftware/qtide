#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class ProgressBar : public Child
{
  Q_OBJECT

public:
  ProgressBar(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:

private:
  int min;
  int max;

};

#endif
