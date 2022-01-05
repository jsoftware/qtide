#ifndef TABS_H
#define TABS_H

#include "child.h"

class Form;
class Pane;
class TabWidget;

// ---------------------------------------------------------------------
class Tabs : public Child
{
  Q_OBJECT

public:
  Tabs(std::string n, std::string s, Form *f, Pane *p);
  std::string state();

  QWidget *activetab();
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  void tabend();
  void tabnew(std::string p);

  int index;

private slots:
  void currentChanged(int index);
  void mycornerClicked();
  void tabCloseRequested(int index);

};

#endif
