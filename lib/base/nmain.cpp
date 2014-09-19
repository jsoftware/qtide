#include <QAction>
#include <QBoxLayout>

#include "base.h"
#include "nmain.h"
#include "nedit.h"
#include "note.h"
#include "ntabs.h"
#include "state.h"

#ifdef QT_OS_ANDROID
extern float DM_density;
#endif

using namespace std;

// ---------------------------------------------------------------------
Nmain::Nmain(Note *n)
{
  QVBoxLayout *layout=new QVBoxLayout;
  layout->setContentsMargins(layout->contentsMargins());
  layout->setSpacing(0);
  tabBar = new Ntabs(n);
  n->tabs = tabBar;
  toolBar = new QToolBar(0);
  layout->addWidget(toolBar,0,0);
  layout->addWidget(tabBar,1,0);
  createActions();
  createToolBar();
  setLayout(layout);
}

// ---------------------------------------------------------------------
void Nmain::createActions()
{
  lastprojectAct = makeact("lastprojectAct","undo.png", "Open Last Project");
#ifdef QT_OS_ANDROID
  openprojectAct = makeact("openprojectAct","folder.png","Open Project");
  runallAct = makeact("runallAct","run.png","Run All Lines");
  xeditAct = makeact("xeditAct","regular.png","External editor");
#else
  openprojectAct = makeact("openprojectAct","folder.svg","Open Project");
  runallAct = makeact("runallAct","run.svg","Run All Lines");
#endif
}

// ---------------------------------------------------------------------
void Nmain::createToolBar()
{
#ifdef QT_OS_ANDROID
  toolBar->setIconSize(QSize((int)DM_density*(5.0/3)*16,(int)DM_density*(5.0/3)*16));
#else
  toolBar->setIconSize(QSize(16,16));
#endif
  toolBar->addAction(lastprojectAct);
  toolBar->addAction(openprojectAct);
  toolBar->addAction(runallAct);
#ifdef QT_OS_ANDROID
  toolBar->addSeparator();
  toolBar->addAction(xeditAct);
#endif
}

// ---------------------------------------------------------------------
QAction *Nmain::makeact(QString id, QString icon, QString text)
{
  QAction *r = new QAction(QIcon(":/images/" + icon),text,this);
  r->setObjectName(id);
  return r;
}
