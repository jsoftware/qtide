#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeWidget>
#include "child.h"

class Form;
class Pane;
class TreeView;

// ---------------------------------------------------------------------
class TreeWidget : public QTreeWidget
{
  Q_OBJECT

public:
  TreeWidget(TreeView *parent);
  TreeView *p;
};

// ---------------------------------------------------------------------
class TreeView : public Child
{
  Q_OBJECT

public:
  TreeView(std::string n, std::string s, Form *f, Pane *p);
  QString action;
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private:
  void collapseall();
  void draw();
  void expandall();
  std::string getselection();
  std::string getselectionindex();
  void setedges(std::string v);
  void setnames(std::string v);

private slots:
  void itemActivated();
  void itemSelectionChanged();

};

#endif
