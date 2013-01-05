#ifndef LISTBOX_H
#define LISTBOX_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class ListBox : public Child
{
  Q_OBJECT

public:
  ListBox(string s, string p, Form *f);
  void set(string p);
  string state();

private slots:
  void currentRowChanged();
  void itemActivated();

};

#endif
