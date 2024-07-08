// treeview.cpp
//
// based on a QTreeWidget
//
// no get functions - use event data instead
// several hard-coded options, TreeView::TreeView()
//
// tree data given as:
// Names - list of names in tree
// Parents - list of parent names in tree
// Edges - two column table of parent/child edges, indices into names

#include <QTreeWidget>

#include "cmd.h"
#include "form.h"
#include "treeview.h"
#include "pane.h"
#include "wd.h"
#include "../base/state.h"

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

// default options:
  w->setHeaderHidden(true);
  w->setIndentation(12);
  w->setColumnCount(1);

// following should enforce single selection, but
// allows more than one selection with setselect
  w->setSelectionMode(QAbstractItemView::SingleSelection);
  w->setSelectionBehavior(QAbstractItemView::SelectItems);

  connect(w,&QTreeWidget::itemActivated,
          this,&TreeView::itemActivated);
  connect(w,&QTreeWidget::itemCollapsed,
          this,&TreeView::itemCollapsed);
  connect(w,&QTreeWidget::itemExpanded,
          this,&TreeView::itemExpanded);
  connect(w,&QTreeWidget::itemSelectionChanged,
          this,&TreeView::itemSelectionChanged);
}

// ---------------------------------------------------------------------
void TreeView::clearselections()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  QList<QTreeWidgetItem *> sel = w->selectedItems();
  int len = sel.size();
  for (int i=0; i<len; i++)
    sel[i]->setSelected(false);
}

// ---------------------------------------------------------------------
void TreeView::collapse(std::string v)
{
  int i=c_strtoi(v);
  if (Parents.contains(i)) {
    QTreeWidget *w=(QTreeWidget*) widget;
    w->collapseItem(Nodes[i]);
  }
}

// ---------------------------------------------------------------------
void TreeView::collapseall()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  int len = Nodes.size();
  int sel = -1;
  for (int i=0; i<len; i++) {
    if (Nodes[i]->isSelected()) sel = i;
    w->collapseItem(Nodes[i]);
  }
  clearselections();
  if (sel < 0) return;
  len = Edges.size() / 2;
  for (int i=0; i<len; i++)
    if (sel == Edges[1+i*2]) {
      sel = Edges[i*2];
      break;
    }
  Nodes[sel]->setSelected(true);
}

// ---------------------------------------------------------------------
void TreeView::draw()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  Children.clear();
  Parents.clear();
  Nodes.clear();
  CurrentIndex=-1;

  int nlen = Names.size();
  int elen = Edges.size();
  if (0 == nlen || 0 == elen) return;
  int clen = elen/2;

  for (int i=0; i<nlen; i++)
    Nodes.append(new QTreeWidgetItem({Names[i]}));
  for (int i=0; i<clen; i++) {
    Children.append(Edges[1+2*i]);
  }
  for (int i=0; i<nlen; i++) {
    if (!Children.contains(i))
      Parents.append(i);
  }
  for (int i=0; i<Parents.size(); i++)
    w->addTopLevelItem(Nodes[Parents[i]]);

  for (int i=0; i<clen; i++)
    Nodes[Edges[i*2]]->addChild(Nodes[Edges[1+i*2]]);
}

// ---------------------------------------------------------------------
void TreeView::expand(std::string v)
{
  int i=c_strtoi(v);
  if (Parents.contains(i)) {
    QTreeWidget *w=(QTreeWidget*) widget;
    w->expandItem(Nodes[i]);
  }
}

// ---------------------------------------------------------------------
void TreeView::expandall()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  int len = Nodes.size();
  int sel = -1;
  for (int i=0; i<len; i++)
    if (Nodes[i]->isSelected()) sel = i;
  for (int i=0; i<len; i++) {
    QTreeWidgetItem *n=Nodes[i];
    if (n->childCount()>0 && !n->isExpanded())
      w->expandItem(Nodes[i]);
  }
  clearselections();
  if (sel >= 0) Nodes[sel]->setSelected(true);
}

// ---------------------------------------------------------------------
void TreeView::itemActivated()
{
  event="button";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
// select parent only if there is a selected child
void TreeView::itemCollapsed(QTreeWidgetItem *p)
{
  CurrentIndex = Nodes.indexOf(p);
  event="collapsed";
  pform->signalevent(this);
  int len=p->childCount();
  for (int i=0; i<len; i++) {
    if (p->child(i)->isSelected()) {
      p->setSelected(true);
      break;
    }
  }
}

// ---------------------------------------------------------------------
// auto select first item if no other child selected
void TreeView::itemExpanded(QTreeWidgetItem *p)
{
  CurrentIndex = Nodes.indexOf(p);
  event="expanded";
  pform->signalevent(this);
  QTreeWidget *w=(QTreeWidget*) widget;
  p->setSelected(false);
  int len = Children.size();
  for (int i=0; i<len; i++) {
    QTreeWidgetItem *n = Nodes[Children[i]];
    if (n->isSelected() && n->parent()->isExpanded()) return;
  }
  QTreeWidgetItem *c = p->child(0);
  clearselections();
  w->setCurrentItem(c);
  c->setSelected(true);
}

// ---------------------------------------------------------------------
void TreeView::itemSelectionChanged()
{
  if (event == "collapsed") return;
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void TreeView::set(std::string p,std::string v)
{
  if (p=="collapse")
    collapse(v);
  else if (p=="collapseall")
    collapseall();
  else if (p=="expand")
    expand(v);
  else if (p=="expandall")
    expandall();
  else if (p=="indent")
    setindent(v);
  else if (p=="select")
    setselect(v);
  else if (p == "togglexp")
    settogglexp();
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
  Edges = QVector::fromList(qs2intlist(s2q(v)));
  draw();
}

// ---------------------------------------------------------------------
void TreeView::setindent(std::string v)
{
  QTreeWidget *w=(QTreeWidget*) widget;
  w->setIndentation(c_strtoi(v));
}

// ---------------------------------------------------------------------
void TreeView::setnames(std::string v)
{
  Names = qsplit(v);
}

// ---------------------------------------------------------------------
// select index + expand parents to visible
// -1 means clear selections
void TreeView::setselect(std::string v)
{
  int i=c_strtoi(v);
  if (i < -1 || i >= Nodes.size()) return;
  clearselections();
  if (i == -1) return;
  QTreeWidgetItem *n=Nodes[i];
  QTreeWidget *w=(QTreeWidget*) widget;
  while (true) {
    n = n->parent();
    if (!n || n->isExpanded()) break;
    w->expandItem(n);
  }
  n->setSelected(true);
  w->setCurrentItem(n);
}

// ---------------------------------------------------------------------
void TreeView::settogglexp()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  QTreeWidgetItem* selected = w->currentItem();
  if (selected->childCount() == 0) return;
  selected->setExpanded(!selected->isExpanded());
}

// ---------------------------------------------------------------------
// the _select item in the state gives the list of indices
// to get to the item selected.
std::string TreeView::state()
{
  QTreeWidget *w=(QTreeWidget*) widget;
  QTreeWidgetItem *sel;
  std::string r;
  int index;

  if (event=="collapsed" || event=="expanded") {
    index=CurrentIndex;
    sel = Nodes[index];
  } else {
    sel = w->currentItem();
    index=Nodes.indexOf(sel);
  }

  if (!sel) {
    r+=spair(id,(std::string)"");
    return r + spair(id+"_select",(std::string)"_1");
  }

  r=spair(id,q2s(sel->text(0)));
  return r + spair(id+"_select",i2s(index));
}
