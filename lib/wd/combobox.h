#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class ComboBox : public Child
{
  Q_OBJECT

public:
  ComboBox(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private:
  std::string getitems();

private slots:
  void activated();

};

#endif
