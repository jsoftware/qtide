
#include <QListWidget>

#include "cmd.h"
#include "form.h"
#include "listbox.h"
#include "pane.h"
#include "wd.h"
#include "../base/state.h"

extern int rc;

// ---------------------------------------------------------------------
ListWidget::ListWidget(QWidget *w) : QListWidget()
{
  p = (ListBox *)w;
}

// ---------------------------------------------------------------------
void ListWidget::dropEvent(QDropEvent *e)
{
  QListWidget::dropEvent(e);
  p->signaldrop();
}

// ---------------------------------------------------------------------
ListBox::ListBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="listbox";
  QListWidget *w=new ListWidget((QWidget *)this);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"drag drop multiple")) return;
  if (notbothopt(n,opt,"drag","multiple")) return;
  w->setObjectName(qn);
  childStyle(opt);
  if (opt.contains("drag"))
    w->setDragEnabled(true);
  if (opt.contains("drop")) {
    w->setDragDropMode(QAbstractItemView::DragDrop);
    w->viewport()->setAcceptDrops(true);
    w->setDropIndicatorShown(true);
    w->setDefaultDropAction(Qt::MoveAction);
  }
  if (opt.contains("multiple"))
    w->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(w,SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(itemActivated()));
  connect(w,SIGNAL(itemSelectionChanged()),
          this,SLOT(itemSelectionChanged()));
}

// ---------------------------------------------------------------------
string ListBox::get(string p,string v)
{
  QListWidget *w=(QListWidget*) widget;
  string r;
  if (p=="property") {
    r+=string("allitems")+"\012"+ "items"+"\012"+ "select"+"\012"+ "text"+"\012";
    r+=Child::get(p,v);
  } else if (p=="allitems")
    r=getitems();
  else if (p=="items")
    r=getselection();
  else if (p=="text"||p=="select") {
    QList <QListWidgetItem*> list = w->selectedItems();
    if (0==list.count()) {
      if (p=="text")
        r="";
      else
        r=i2s(-1);
    } else {
      if ((w->selectionMode()) == QAbstractItemView::ExtendedSelection) {
        if (p=="text")
          r=getselection();
        else
          r=getselectionindex();
      } else {
        int n=w->currentRow();
        if (p=="text")
          r=q2s(w->item(n)->text());
        else
          r=i2s(n);
      }
    }
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
string ListBox::getitems()
{
  QListWidget *w=(QListWidget*) widget;
  string s="";

  for (int i=0; i<w->count(); i++) {
    s += q2s(w->item(i)->text());
    s += "\012";
  }
  return(s);
}

// ---------------------------------------------------------------------
string ListBox::getselection()
{
  QListWidget *w=(QListWidget*) widget;
  QList <QListWidgetItem*> list = w->selectedItems();
  string s="";

  for (int i=0; i<list.count(); i++) {
    s += q2s(((QListWidgetItem*) list.at(i))->text());
    s += "\012";
  }
  return(s);
}

// ---------------------------------------------------------------------
string ListBox::getselectionindex()
{
  QListWidget *w=(QListWidget*) widget;
  QModelIndexList list = ((QItemSelectionModel *)w->selectionModel())->selectedIndexes();
  string s="";

  for (int i=0; i<list.size(); i++) {
    s += i2s(list[i].row());
    s += " ";
  }
  return(s);
}

// ---------------------------------------------------------------------
void ListBox::itemActivated()
{
  event="button";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void ListBox::itemSelectionChanged()
{
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void ListBox::set(string p,string v)
{
  QListWidget *w=(QListWidget*) widget;
  if (p=="items") {
    w->clear();
    w->addItems(qsplit(v));
  } else if (p=="select") {
    w->setCurrentRow(c_strtoi(v));
  } else
    Child::set(p,v);
}

// ---------------------------------------------------------------------
void ListBox::signaldrop()
{
  event="drop";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
string ListBox::state()
{
  QListWidget *w=(QListWidget*) widget;
  QList <QListWidgetItem*> list = w->selectedItems();
  string r;
  if (0==list.count()) {
    r+=spair(id,(string)"");
    r+=spair(id+"_select",(string)"_1");
  } else {
    if ((w->selectionMode()) == QAbstractItemView::ExtendedSelection) {
      r+=spair(id,getselection());
      r+=spair(id+"_select",getselectionindex());
    } else {
      int n=w->currentRow();
      r+=spair(id,q2s(w->item(n)->text()));
      r+=spair(id+"_select",i2s(n));
    }
  }
  return r;
}
