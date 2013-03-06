
#include <QListWidget>

#include "wd.h"
#include "listbox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
ListBox::ListBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
//  qDebug() << "make ListBox";
  type="listbox";
  QListWidget *w=new QListWidget(p);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  connect(w,SIGNAL(currentRowChanged(int)),
          this,SLOT(currentRowChanged()));
  connect(w,SIGNAL(itemActivated(QListWidgetItem*)),
          this,SLOT(itemActivated()));
}

// ---------------------------------------------------------------------
void ListBox::currentRowChanged()
{
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void ListBox::itemActivated()
{
  event="button";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void ListBox::set(string p, string v)
{
  QListWidget *w=(QListWidget*) widget;
  if (p=="items") {
    w->clear();
    w->addItems(qsplit(v));
  } else if (p=="select") {
    w->setCurrentRow(s2q(v).toInt());

  } else
    Child::set(p,v);
}

// ---------------------------------------------------------------------
string ListBox::state()
{
  QListWidget *w=(QListWidget*) widget;
  int n=w->currentRow();
  string r;
  if (n<0) {
    r+=spair(id,"");
    r+=spair(id+"_select","");
  } else {
    r+=spair(id,q2s(w->item(n)->text()));
    r+=spair(id+"_select",i2s(n));
  }
  return r;
}
