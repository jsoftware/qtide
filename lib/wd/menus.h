#ifndef MENUS_H
#define MENUS_H

#include "child.h"

class QMenu;

class Menus : public Child
{
  Q_OBJECT

public:
  Menus(string s, string p, Form *f);
  QAction *makeact(string id, string parms);

  int menu(string c, string p);
  int menupop(string c);
  int menupopz();
  int menusep();
  void setenable(string p);
  void setp(string p,string v);

  QHash<QString,QAction*> items;

private slots:
  void menu_triggered(QAction *a);

private:
  QMenu *curMenu;
  QList<QMenu*> menus;

  QString selected;
};

#endif
