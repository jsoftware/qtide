#ifndef MENUS_H
#define MENUS_H

#include "child.h"

class QMenu;
class Form;
class Pane;

class Menus : public Child
{
  Q_OBJECT

public:
  Menus(string n, string s, Form *f, Pane *p);
  QAction *makeact(string id, string parms);

  int menu(string c, string p);
  int menupop(string c);
  int menupopz();
  int menusep();
  string get(string p,string v);
  void set(string p,string v);
  string state();

  QHash<QString,QAction*> items;

private slots:
  void menu_triggered();

private:
  QMenu *curMenu;
  QList<QMenu*> menus;

  QString selected;
};

#endif
