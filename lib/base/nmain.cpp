#include <QAction>
#include <QBoxLayout>

#include "base.h"
#include "nmain.h"
#include "nedit.h"
#include "note.h"
#include "ntabs.h"
#include "state.h"

// using namespace std;

// ---------------------------------------------------------------------
Nmain::Nmain(Note *n)
{
  QVBoxLayout *layout=new QVBoxLayout;
  layout->setContentsMargins(layout->contentsMargins());
  layout->setSpacing(0);
  tabBar = new Ntabs(n);
  n->tabs = tabBar;
  toolBar = new QToolBar(0);
  layout->addWidget(toolBar,0,Qt::Alignment());
  layout->addWidget(tabBar,1,Qt::Alignment());
  createActions();
  createToolBar();
  setLayout(layout);
}

// ---------------------------------------------------------------------
void Nmain::createActions()
{
  lastprojectAct = makeact("lastprojectAct","undo.png", "Open Last Project");
  openprojectAct = makeact("openprojectAct","folder.png","Open Project");
  runallAct = makeact("runallAct","run.png","Run All Lines");
}

// ---------------------------------------------------------------------
void Nmain::createToolBar()
{
  toolBar->setIconSize(QSize(16,16));
  toolBar->addAction(lastprojectAct);
  toolBar->addAction(openprojectAct);
  toolBar->addAction(runallAct);
}

// ---------------------------------------------------------------------
QAction *Nmain::makeact(QString id, QString icon, QString text)
{
  QAction *r = new QAction(QIcon(":/images/" + icon),text,this);
  r->setObjectName(id);
  return r;
}
