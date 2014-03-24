
#include <QListWidget>

#include "wd.h"
#include "listbox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

extern int rc;

// ---------------------------------------------------------------------
ListBox::ListBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="listbox";
  QListWidget *w=new QListWidget(p);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  w->setObjectName(qn);
  if (opt.contains("multiple"))
    w->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(w,SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(itemActivated()));
  connect(w,SIGNAL(itemSelectionChanged()),
          this,SLOT(itemSelectionChanged()));
}

// ---------------------------------------------------------------------
string ListBox::get(string p, string v)
{

  if (p=="items") {
    rc=-1;
    return(getselection());
  } else
    return Child::get(p,v);
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
void ListBox::set(string p, string v)
{
  QListWidget *w=(QListWidget*) widget;
  if (p=="items") {
    w->clear();
    w->addItems(rsplit(v));
  } else if (p=="select") {
    w->setCurrentRow(c_strtoi(v));
  } else
    Child::set(p,v);
}

// ---------------------------------------------------------------------
string ListBox::state()
{
  QListWidget *w=(QListWidget*) widget;
  QList <QListWidgetItem*> list = w->selectedItems();
  string r;
  if (0==list.count()) {
    r+=spair(id,"");
    r+=spair(id+"_select","");
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
