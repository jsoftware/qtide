#ifndef LISTBOX_H
#define LISTBOX_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class ListBox : public Child
{
  Q_OBJECT

public:
  ListBox(string n, string s, Form *f, Pane *p);
  void set(string p);
  string state();

private slots:
  void currentRowChanged();
  void itemActivated();

};

#endif
