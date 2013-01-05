
#include <QListWidget>

#include "wd.h"
#include "listbox.h"
#include "form.h"
#include "cmd.h"

// ---------------------------------------------------------------------
ListBox::ListBox(string s, string p, Form *f) : Child(s,p,f)
{
  type="listbox";
  QListWidget *w=new QListWidget;
  widget=(QWidget*) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
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
void ListBox::set(string p)
{
  QListWidget *w=(QListWidget*) widget;
  w->clear();
  w->addItems(qsplit(p));
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
