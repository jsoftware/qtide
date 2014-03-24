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
  string get(string p, string v);
  void set(string p, string v);
  string state();

private:
  string getselection();
  string getselectionindex();

private slots:
  void itemActivated();
  void itemSelectionChanged();

};

#endif
