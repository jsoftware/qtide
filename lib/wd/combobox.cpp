
#include "../base/pcombobox.h"
#include "../base/state.h"

#include "wd.h"
#include "combobox.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
ComboBox::ComboBox(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="combobox";
  PComboBox *w=new PComboBox;
#ifdef QT_OS_ANDROID
  w->setStyleSheet(scrollbarstyle(config.ScrollBarSize*DM_density));
#endif
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("edit")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }
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
    r+=spair(id+"_select",(string)"");
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
