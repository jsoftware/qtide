
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
// this can be an option...
//  w->setDocumentMode(true);
  w->setObjectName(qn);
  w->setMovable(true);
  w->setTabsClosable(true);
  w->setUsesScrollButtons(true);
// force repaint on display
  QTimer::singleShot(0, this, SLOT(update()));
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
