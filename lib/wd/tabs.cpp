
#include <QIcon>
#include <QStyle>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "tabs.h"
#include "tabwidget.h"
#include "wd.h"

// ---------------------------------------------------------------------
Tabs::Tabs(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="tabs";
  index=0;

  TabWidget *w=new TabWidget;
  widget=(TabWidget*) w;

  form->tabs.append(this);
  form->tab=this;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"documentmode movable closable east west south nobar")) return;
  w->setObjectName(qn);
  childStyle(opt);
  w->setUsesScrollButtons(true);

  if (opt.contains("documentmode"))
    w->setDocumentMode(true);

  if (opt.contains("movable"))
    w->setMovable(true);

  if (opt.contains("closable"))
    w->setTabsClosable(true);

  if (opt.contains("east"))
    w->setTabPosition(QTabWidget::East);
  else if (opt.contains("west"))
    w->setTabPosition(QTabWidget::West);
  else if (opt.contains("south"))
    w->setTabPosition(QTabWidget::South);

  if (opt.contains("nobar"))
    w->nobar(true);

  connect(w,SIGNAL(currentChanged(int)),
          this,SLOT(currentChanged(int)));

  connect(w,SIGNAL(tabCloseRequested(int)),
          this,SLOT(tabCloseRequested(int)));

}

// ---------------------------------------------------------------------
void Tabs::currentChanged(int ndx)
{
  Q_UNUSED(ndx);
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
string Tabs::get(string p,string v)
{
  QTabWidget *w = (QTabWidget *)widget;
  string r;
  if (p=="property") {
    r+=string("label")+"\012"+ "select"+"\012";
    r+=Child::get(p,v);
  }
  if (p=="label"||p=="select") {
    int n=w->currentIndex();
    string s,t;
    for (int i=0; i<w->count(); i++)
      s+=q2s(w->tabText(i)) + '\377';
    t=(n>=0)?i2s(n):string("_1");
    if (p=="label")
      r=s;
    else
      r=t;
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Tabs::set(string p,string v)
{
  QTabWidget *w = (QTabWidget *)widget;
  QStringList opt=qsplit(v);
  if (opt.isEmpty()) return;
  int ndx=c_strtoi(q2s(opt.at(0)));
  if (p=="active")
    w->setCurrentIndex(ndx);
  else if (p=="tabclose")
    w->removeTab(ndx);
  else if (p=="label") {
    if (opt.size()<2) return;
    w->setTabText(ndx,s2q(remquotes(q2s(opt.at(1)))));
  } else if (p=="tabenabled") {
    if (opt.size()<2) w->setTabEnabled(ndx,true);
    w->setTabEnabled(ndx,remquotes(q2s(opt.at(1)))!="0");
  } else if (p=="icon") {
    if (opt.size()<2) return;
    string iconFile=remquotes(q2s(opt.at(1)));
    QIcon image;
    int spi;
    if (iconFile.substr(0,8)=="qstyle::" && -1!=(spi=wdstandardicon(iconFile)))
      w->setTabIcon(ndx,w->style()->standardIcon((QStyle::StandardPixmap)spi));
    else
      w->setTabIcon(ndx,QIcon(s2q(iconFile)));
  } else if (p=="tooltip") {
    if (opt.size()<2) w->setTabToolTip(ndx,"");
    w->setTabToolTip(ndx,s2q(remquotes(q2s(opt.at(1)))));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Tabs::state()
{
  QTabWidget *w=(QTabWidget*) widget;
  int n;
  if (this==pform->evtchild && event=="tabclose")
    n=index;
  else
    n=w->currentIndex();
  string r,s,t;
  for (int i=0; i<w->count(); i++)
    s+=q2s(w->tabText(i)) + '\377';
  t=(n>=0)?i2s(n):string("_1");
  r+=spair(id,s);
  r+=spair(id+"_select",t);
  return r;
}

// ---------------------------------------------------------------------
void Tabs::tabCloseRequested(int ndx)
{
  index=ndx;
  event="tabclose";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Tabs::tabend()
{
  QTabWidget *w=(QTabWidget *) widget;
  pform->pane->fini();

  form->tabs.removeLast();
  if (form->tabs.isEmpty())
    form->tab=0;
  else
    form->tab=form->tabs.last();

  if (index)
    w->setCurrentIndex(0);
}

// ---------------------------------------------------------------------
void Tabs::tabnew(string p)
{
  if (index)
    pform->pane->fini();
  QTabWidget *w=(QTabWidget *) widget;
  pform->addpane(1);
  w->addTab(pform->pane, s2q(p));
  index++;
}
