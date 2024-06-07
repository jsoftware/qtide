// treeview.cpp
//
// based on a QTreeWidget
//
// currently a bare bones implementation
// no get functions - use event data instead
// several hard-coded options
//
// tree data given as:
// names - list of names in tree
// edges - two column table of parent/child edges, indices into names

#include <QTreeWidget>

#include "cmd.h"
#include "form.h"
#include "treeview.h"
#include "pane.h"
#include "wd.h"
#include "../base/state.h"

static QStringList Names;
static QVector<int> Edges;
static QVector<QTreeWidgetItem *> Nodes;

extern int rc;

// ---------------------------------------------------------------------
TreeWidget::TreeWidget(TreeView *parent) : QTreeWidget()
{
  p = parent;
}

// ---------------------------------------------------------------------
TreeView::TreeView(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="treeview";
  QTreeWidget *w=new TreeWidget(this);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);

// hard coded options:
  w->setHeaderHidden(true);
  w->setIndentation(12);
  w->setColumnCount(1);
  w->setSelectionMode(QAbstractItemView::SingleSelection);

  connect(w,&QTreeWidget::itemActivated,
          this,&TreeView::itemActivated);
  connect(w,&QTreeWidget::itemSelectionChanged,
          this,&TreeView::itemSelectionChanged);
}

// ---------------------------------------------------------------------
void TreeView::collapseall()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  int len = Nodes.size();
  for (int i=0; i<len; i++)
    w->collapseItem(Nodes[i]);
}

// ---------------------------------------------------------------------
void TreeView::expandall()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  int len = Nodes.size();
  for (int i=0; i<len; i++)
    w->expandItem(Nodes[i]);
}

// ---------------------------------------------------------------------
void TreeView::draw()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  QVector<int>children;
  QVector<int>parents;

  int nlen = Names.size();
  int elen = Edges.size();
  if (0 == nlen || 0 == elen) return;
  int clen = elen/2;

  for (int i=0; i<nlen; i++)
    Nodes.append(new QTreeWidgetItem({Names[i]}));
  for (int i=0; i<clen; i++) {
    children.append(Edges[1 + 2 * i]);
  }
  for (int i=0; i<nlen; i++) {
    if (!children.contains(i))
      parents.append(i);
  }
  for (int i=0; i<parents.size(); i++)
    w->addTopLevelItem(Nodes[parents[i]]);
  for (int i=0; i<clen; i++)
    Nodes[Edges[i*2]]->addChild(Nodes[Edges[1+i*2]]);
}

// ---------------------------------------------------------------------
// getxxx not yet ...
std::string TreeView::get(std::string p,std::string v)
{
  //QTreeWidget *w=(QTreeWidget*) widget;
  Q_UNUSED(p);
  Q_UNUSED(v);
  std::string r;
  return r;
}

// ---------------------------------------------------------------------
std::string TreeView::getselection()
{
  //QTreeWidget *w=(QTreeWidget*) widget;
  std::string s="";
  return(s);
}

// ---------------------------------------------------------------------
std::string TreeView::getselectionindex()
{
  //QTreeWidget *w=(QTreeWidget*) widget;
  std::string s="";
  return(s);
}

// ---------------------------------------------------------------------
void TreeView::itemActivated()
{
  event="button";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void TreeView::itemSelectionChanged()
{
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void TreeView::set(std::string p,std::string v)
{
  if (p=="collapseall")
    collapseall();
  else if (p=="expandall")
    expandall();
  else if (p=="names")
    setnames(v);
  else if (p == "edges")
    setedges(v);
  else
    Child::set(p,v);
}

// ---------------------------------------------------------------------
void TreeView::setedges(std::string v)
{
  Edges = qs2intlist(s2q(v));
  draw();
}

// ---------------------------------------------------------------------
void TreeView::setnames(std::string v)
{
  Names = qsplit(v);
}

// ---------------------------------------------------------------------
// the _select item in the state gives the list of indices
// to get to the item selected.
std::string TreeView::state()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  QList <QTreeWidgetItem*> list = w->selectedItems();
  QTreeWidgetItem* selected = w->currentItem();
  std::string r;
  if (0==w->selectedItems().count()) {
    r+=spair(id,(std::string)"");
    return r + spair(id+"_select",(std::string)"_1");
  }
  QModelIndex wx = w->currentIndex();
  std::string mark = i2s(wx.row());
  int m;
  while (true) {
    wx = wx.parent();
    m = wx.row();
    if (0 > m) break;
    mark = i2s(m) + " " + mark;
  }
  r+=spair(id,q2s(selected->text(0)));
  return r + spair(id+"_select",mark);
}
