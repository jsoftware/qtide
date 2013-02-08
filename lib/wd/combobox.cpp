
#include <QComboBox>

#include "wd.h"
#include "combobox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
ComboBox::ComboBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="combobox";
  QComboBox *w=new QComboBox;
  widget=(QWidget*) w;
  if (s.substr(0,4)=="edit")
    w->setEditable(true);
  QString qn=s2q(n);
  w->setObjectName(qn);
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
void ComboBox::set(string p)
{
  QComboBox *w=(QComboBox*) widget;
  w->clear();
  w->addItems(qsplit(p));
}

// ---------------------------------------------------------------------
void ComboBox::setp(string p,string v)
{
  if (p=="select")
    ((QComboBox *)widget)->setCurrentIndex(atoi(v.c_str()));
  else Child::setp(p,v);
}

// ---------------------------------------------------------------------
string ComboBox::state()
{
  QComboBox *w=(QComboBox*) widget;
  int n=w->currentIndex();
  string r;
  if (n<0) {
    r+=spair(id,"");
    r+=spair(id+"_select","");
  } else {
    r+=spair(id,q2s(w->currentText()));
    r+=spair(id+"_select",i2s(n));
  }
  return r;
}

// ---------------------------------------------------------------------
void ComboBox::setminwh(int w, int h)
{
  if (widget && w && h) {
    widget->setMinimumSize(w,0);
    widget->updateGeometry();
  }
}
