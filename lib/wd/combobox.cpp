
#include <QApplication>
#include <QComboBox>

#include "../base/state.h"

#include "cmd.h"
#include "combobox.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

extern Font *fontdef;

// ---------------------------------------------------------------------
ComboBox::ComboBox(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="combobox";
  QComboBox *w=new QComboBox;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"edit")) return;
  w->setObjectName(qn);
  childStyle(opt);
  w->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  if (opt.contains("edit"))
    w->setEditable(true);
  if (f->maxvisibleitems)
    w->setMaxVisibleItems(f->maxvisibleitems);
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
std::string ComboBox::get(std::string p,std::string v)
{
  QComboBox *w=(QComboBox*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("edit")+"\012"+ "allitems"+"\012"+ "select"+"\012"+ "text"+"\012";
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
std::string ComboBox::getitems()
{
  QComboBox *w=(QComboBox*) widget;
  std::string s="";

  for (int i=0; i<w->count(); i++) {
    s += q2s(w->itemText(i));
    s += "\012";
  }
  return(s);
}

// ---------------------------------------------------------------------
void ComboBox::set(std::string p,std::string v)
{
  QComboBox *w=(QComboBox*) widget;
  if (p=="items") {
    w->clear();
    w->addItems(qsplit(v));
  } else if (p=="maxvisibleitems")
    ((QComboBox *)widget)->setMaxVisibleItems(atoi(v.c_str()));
  else if (p=="select")
    ((QComboBox *)widget)->setCurrentIndex(atoi(v.c_str()));
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string ComboBox::state()
{
  QComboBox *w=(QComboBox*) widget;
  int n=w->currentIndex();
  std::string r;
  if (n<0) {
    r+=spair(id,(std::string)"");
    r+=spair(id+"_select",(std::string)"_1");
  } else {
    r+=spair(id,q2s(w->currentText()));
    r+=spair(id+"_select",i2s(n));
  }
  return r;
}
