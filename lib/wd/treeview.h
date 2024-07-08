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
  void set(std::string p,std::string v);
  std::string state();

private:
  void clearselections();
  void collapse(std::string v);
  void collapseall();
  void draw();
  void expand(std::string v);
  void expandall();
  void setedges(std::string v);
  void setindent(std::string v);
  void setnames(std::string v);
  void setselect(std::string v);
  void settogglexp();
  int CurrentIndex;
  QStringList Names;
  QList<int> Edges;
  QVector<QTreeWidgetItem *> Nodes;
  QVector<int> Children;
  QVector<int> Parents;

private slots:
  void itemActivated();
  void itemCollapsed(QTreeWidgetItem *);
  void itemExpanded(QTreeWidgetItem *);
  void itemSelectionChanged();

};

#endif
