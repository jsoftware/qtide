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
  Menus(std::string n, std::string s, Form *f, Pane *p);
  QAction *makeact(std::string id, std::string parms);

  int menu(std::string c, std::string p);
  int menupop(std::string c);
  int menupopz();
  int menusep();
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

  QHash<QString,QAction*> items;

private slots:
  void menu_triggered();

private:
  QMenu *curMenu;
  QList<QMenu*> menus;

  QString selected;
};

#endif
