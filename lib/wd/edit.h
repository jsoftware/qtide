#ifndef EDIT_H
#define EDIT_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Edit : public Child
{
  Q_OBJECT

public:
  Edit(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:
  void returnPressed();
  void textChanged();

private:
  bool focusSelect;
  bool uppercase;

};

#endif
