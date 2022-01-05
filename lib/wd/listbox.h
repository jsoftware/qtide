#ifndef LISTBOX_H
#define LISTBOX_H

#include <QDropEvent>
#include <QListWidget>
#include "child.h"

class Form;
class Pane;
class ListBox;

// ---------------------------------------------------------------------
class ListWidget : public QListWidget
{
  Q_OBJECT

public:
  ListWidget(ListBox *parent);
  ListBox *p;

protected:
  void dropEvent(QDropEvent *e);
};

// ---------------------------------------------------------------------
class ListBox : public Child
{
  Q_OBJECT

public:
  ListBox(std::string n, std::string s, Form *f, Pane *p);
  void signaldrop();
  QString action;
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private:
  std::string getitems();
  std::string getselection();
  std::string getselectionindex();

private slots:
  void itemActivated();
  void itemSelectionChanged();

};

#endif
