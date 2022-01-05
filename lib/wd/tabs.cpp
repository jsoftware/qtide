
#include <QIcon>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "tabs.h"
#include "tabwidget.h"
#include "wd.h"

// ---------------------------------------------------------------------
Tabs::Tabs(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="tabs";
  index=0;

  TabWidget *w=new TabWidget;
  widget=(TabWidget*) w;

  pform->tabs.append(this);
  pform->tab=this;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"documentmode movable closable east west south nobar mycorner")) return;
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

// test of corner widget with style mycorner
// this is a one-off, can be replaced with a proper implementation
  if (opt.contains("mycorner")) {
    QPushButton* b = new QPushButton();
    b->setText("Add View");
    QWidget *cw = new QWidget(w);
    QVBoxLayout *vb = new QVBoxLayout(cw);
    vb->addWidget(b);
    vb->addStretch(1);
    w->setCornerWidget(cw);
    connect(b,SIGNAL(clicked()),
            this,SLOT(mycornerClicked()));
  }
}

// ---------------------------------------------------------------------
QWidget *Tabs::activetab()
{
  QTabWidget *w = (QTabWidget *)widget;
  return w->currentWidget();
}

// ---------------------------------------------------------------------
void Tabs::currentChanged(int ndx)
{
  Q_UNUSED(ndx);
  event="select";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string Tabs::get(std::string p,std::string v)
{
  QTabWidget *w = (QTabWidget *)widget;
  std::string r;
  if (p=="property") {
    r+=std::string("label")+"\012"+ "select"+"\012";
    r+=Child::get(p,v);
  }
  if (p=="label"||p=="select") {
    int n=w->currentIndex();
    std::string s,t;
    for (int i=0; i<w->count(); i++)
      s+=q2s(w->tabText(i)) + '\377';
    t=(n>=0)?i2s(n):std::string("_1");
    if (p=="label")
      r=s;
    else
      r=t;
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Tabs::mycornerClicked()
{
  event="mycorner";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Tabs::set(std::string p,std::string v)
{
  QTabWidget *w = (QTabWidget *)widget;
  QStringList opt=qsplit(v);
  if (opt.isEmpty()) return;
  int ndx=c_strtoi(q2s(opt.at(0)));
  if (p=="active")
    w->setCurrentIndex(ndx);
  else if (p=="tabclose")
    w->removeTab(ndx);
// following adds a tab with no contents:
  else if (p=="tabnew")
    tabnew(v);
  else if (p=="label") {
    if (opt.size()<2) return;
    w->setTabText(ndx,s2q(remquotes(q2s(opt.at(1)))));
  } else if (p=="tabenabled") {
    if (opt.size()<2) w->setTabEnabled(ndx,true);
    w->setTabEnabled(ndx,remquotes(q2s(opt.at(1)))!="0");
  } else if (p=="icon") {
    if (opt.size()<2) return;
    std::string iconFile=remquotes(q2s(opt.at(1)));
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
std::string Tabs::state()
{
  QTabWidget *w=(QTabWidget*) widget;
  int n;
  if (this==pform->evtchild && event=="tabclose")
    n=index;
  else
    n=w->currentIndex();
  std::string r,s,t;
  for (int i=0; i<w->count(); i++)
    s+=q2s(w->tabText(i)) + '\377';
  t=(n>=0)?i2s(n):std::string("_1");
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
  pform->tabpane=0;
  if (index)
    w->setCurrentIndex(0);
}

// ---------------------------------------------------------------------
void Tabs::tabnew(std::string p)
{
  if (index)
    pform->pane->fini();
  QTabWidget *w=(QTabWidget *) widget;
  pform->addpane(1);
  pform->tabpane=(QWidget*) pform->pane;
  w->addTab(pform->pane, s2q(p));
  index++;
}
