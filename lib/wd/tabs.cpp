
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

  QTabWidget *w=new QTabWidget;
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

}

// ---------------------------------------------------------------------
void Tabs::currentChanged(int index)
{
  Q_UNUSED(index);
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Tabs::setp(string p,string v)
{
  QTabWidget *w = (QTabWidget *)widget;
  QStringList opt=qsplit(v);
  if (opt.isEmpty()) return;
  int ndx=opt.at(0).toInt();

  if (p=="active") {
    w->setCurrentIndex(ndx);
  } else if (p=="label") {
    if (opt.size()<2) return;
    w->setTabText(ndx,opt.at(1));
  } else Child::setp(p,v);
}

// ---------------------------------------------------------------------
string Tabs::state()
{
  QTabWidget *w=(QTabWidget*) widget;
  int n=w->currentIndex();
  string r;
  if (n<0) {
    r+=spair(id,"");
    r+=spair(id+"_select","");
  } else {
    r+=spair(id,q2s(w->tabText(n)));
    r+=spair(id+"_select",i2s(n));
  }
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

// force repaint on display
  QTimer::singleShot(0, this, SLOT(updateit()));
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

// ---------------------------------------------------------------------
// try to fix initial display problem on l64 mint 13 (not working)
void Tabs::updateit()
{
//  qDebug() << "updateit";
  widget->update();
}
