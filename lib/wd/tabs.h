#ifndef TABS_H
#define TABS_H

#include "child.h"

class Form;
class Pane;
class QTabWidget;

// ---------------------------------------------------------------------
class Tabs : public Child
{
  Q_OBJECT

public:
  Tabs(string n, string s, Form *f, Pane *p);
  string state();

  void setp(string p,string v);
  void tabend();
  void tabnew(string p);

  int index;

private slots:
  void currentChanged(int index);
  void tabCloseRequested(int index);
  void updateit();

};

#endif
