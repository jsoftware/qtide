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
  ListBox(string n, string s, Form *f, Pane *p);
  void signaldrop();
  string get(string p,string v);
  void set(string p,string v);
  string state();

private:
  string getitems();
  string getselection();
  string getselectionindex();

private slots:
  void itemActivated();
  void itemSelectionChanged();

};

#endif
