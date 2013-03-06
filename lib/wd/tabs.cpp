
#include <QTabWidget>
#include <QTimer>

#include "wd.h"
#include "tabs.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Tabs::Tabs(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="tabs";
  index=0;

  QTabWidget *w=new QTabWidget(p);
  widget=(QWidget*) w;

  form->tabs.append(this);
  form->tab=this;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);

  w->setObjectName(qn);
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
void Tabs::tabCloseRequested(int ndx)
{
  index=ndx;
  event="tabclose";
  pform->signalevent(this);
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
    w->setTabText(ndx,opt.at(1));
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
  if (n>=0) t=i2s(n);
  r+=spair(id,s);
  r+=spair(id+"_select",t);
  return r;
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
