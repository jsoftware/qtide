
#include <QApplication>

#include "../base/pcombobox.h"
#include "../base/state.h"

#include "cmd.h"
#include "combobox.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

extern Font *fontdef;

// ---------------------------------------------------------------------
ComboBox::ComboBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="combobox";
  PComboBox *w=new PComboBox;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"edit")) return;
  w->setObjectName(qn);
  childStyle(opt);
  if (opt.contains("edit"))
    w->setEditable(true);
  connect(w,SIGNAL(activated(int)),
          this,SLOT(activated()));
}

// ---------------------------------------------------------------------
void ComboBox::activated()
{
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
string ComboBox::get(string p,string v)
{
  PComboBox *w=(PComboBox*) widget;
  string r;
  if (p=="property") {
    r+=string("edit")+"\012"+ "allitems"+"\012"+ "select"+"\012"+ "text"+"\012";
    r+=Child::get(p,v);
  } else if (p=="edit") {
    r=i2s(w->isEditable());
  } else if (p=="allitems") {
    r=getitems();
  } else if (p=="text"||p=="select") {
    int n=w->currentIndex();
    if (n<0) {
      if (p=="text")
        r="";
      else
        r=i2s(-1);
    } else {
      if (p=="text")
        r=q2s(w->currentText());
      else
        r=i2s(n);
    }
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
string ComboBox::getitems()
{
  PComboBox *w=(PComboBox*) widget;
  string s="";

  for (int i=0; i<w->count(); i++) {
    s += q2s(w->itemText(i));
    s += "\012";
  }
  return(s);
}

// ---------------------------------------------------------------------
void ComboBox::set(string p,string v)
{
  PComboBox *w=(PComboBox*) widget;
  if (p=="items") {
    w->clear();
    w->addItems(qsplit(v));
  } else if (p=="select")
    ((PComboBox *)widget)->setCurrentIndex(atoi(v.c_str()));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
string ComboBox::state()
{
  PComboBox *w=(PComboBox*) widget;
  int n=w->currentIndex();
  string r;
  if (n<0) {
    r+=spair(id,(string)"");
    r+=spair(id+"_select",(string)"_1");
  } else {
    r+=spair(id,q2s(w->currentText()));
    r+=spair(id+"_select",i2s(n));
  }
  return r;
}
