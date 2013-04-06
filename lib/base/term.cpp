#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QTime>
#include <QTimer>

#include "base.h"
#include "dialog.h"
#include "dlog.h"
#include "menu.h"
#include "note.h"
#include "proj.h"
#include "term.h"
#include "tedit.h"
#include "svr.h"
#include "state.h"
#include "recent.h"

using namespace std;

Term *term;
Tedit *tedit;

QString LastLaunch;
QTime LastLaunchTime;
QTimer *timer=0;

extern "C" void smact();

// ---------------------------------------------------------------------
OneWin::OneWin()
{
  note = new Note();
  split=new QSplitter(Qt::Vertical);
  split->addWidget(makeframe((QWidget *)note));
  split->addWidget(makeframe((QWidget *)term));
  QList<int> n;
  n << 0 << 1;
  //split->setSizes(n);
  QVBoxLayout *layout=new QVBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(split);
  setLayout(layout);
  term->setFocus();
  show();
}

// ---------------------------------------------------------------------
void OneWin::closeEvent(QCloseEvent *event)
{
  term->filequit();
  event->ignore();
}

// ---------------------------------------------------------------------
QFrame *OneWin::makeframe(QWidget *w)
{
  QFrame *f=new QFrame();
  //f->setFrameStyle(QFrame::Panel | QFrame::Raised);
  f->setFrameStyle(QFrame::StyledPanel);
  QVBoxLayout *b=new QVBoxLayout();
  b->setContentsMargins(0,0,0,0);
  b->addWidget(w);
  f->setLayout(b);
  return f;
}

// ---------------------------------------------------------------------
Term::Term()
{
  QVBoxLayout *layout=new QVBoxLayout;
  layout->setContentsMargins(layout->contentsMargins());
  layout->setSpacing(0);
  menuBar = new Menu();
  tedit = new Tedit;
  layout->addWidget(menuBar);
  layout->addWidget(tedit);
  setWindowTitle("Term");
  menuBar->createActions();
  menuBar->createMenus("term");
  setLayout(layout);
  timer=new QTimer;
  connect(timer, SIGNAL(timeout()),this,SLOT(systimer()));
  QMetaObject::connectSlotsByName(this);
#ifdef ANDROID
  activate();
#endif
}

// ---------------------------------------------------------------------
void Term::activate()
{
  activateWindow();
  tedit->setFocus();
}

// ---------------------------------------------------------------------
void Term::closeEvent(QCloseEvent *event)
{
  filequit();
  event->ignore();
}

// ---------------------------------------------------------------------
bool Term::filequit()
{
  dlog_write();
  if (note && (!note->saveall())) return false;
  if (note2 && (!note2->saveall())) return false;
  if ((!config.ConfirmClose) ||
      queryOK("Term","OK to exit " + config.Lang + "?")) {
    state_quit();
    QApplication::quit();
    return true;
  } else
    return false;
}

// ---------------------------------------------------------------------
// this run after configs read...
void Term::fini()
{
  menuBar->createMenus_fini("term");
  move(config.TermPos[0],config.TermPos[1]);
  resize(config.TermPos[2],config.TermPos[3]);
  tedit->setFont(config.Font);
  QPalette p = palette();
  p.setColor(QPalette::Active, QPalette::Base, QColor("cornsilk"));
  p.setColor(QPalette::Inactive, QPalette::Base, QColor("cornsilk"));
  p.setColor(QPalette::Text, QColor("blue"));
  tedit->setPalette(p);
  QString s=config.BinPath.filePath("icons/jgreen.png");
  setWindowIcon(QIcon(s));
  if (config.SingleWin)
    new OneWin();
  else if (ShowIde)
    show();
}

// ---------------------------------------------------------------------
void Term::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
  case Qt::Key_Escape:
    if (config.EscClose) {
      if (!filequit())
        event->accept();
    }
    break;
  default:
    QWidget::keyPressEvent(event);
  }
}

// ---------------------------------------------------------------------
// bug in Qt - this gets called twice, so need to check time...
void Term::launchpad_triggered(QAction *a)
{
  QString s=a->objectName();

  s=s.mid(config.LaunchPadPrefix.size());
  QTime t=QTime::currentTime();
  if (LastLaunch==s && LastLaunchTime.secsTo(t)<2) return;
  LastLaunch=s;
  LastLaunchTime=t;
  int i=config.LaunchPadKeys.indexOf(s);

  if (i<0) return;
  tedit->loadscript(config.LaunchPadValues.at(i),false);
}

// ---------------------------------------------------------------------
void Term::load(QString s, bool d)
{
  tedit->docmdx(var_load(s,d));
}

// ---------------------------------------------------------------------
void Term::pacman()
{
  var_cmd("require 'pacman ~addons/ide/qt/pacman.ijs'");
  var_cmd("runpacman_jpacman_ 0");
}

// ---------------------------------------------------------------------
void Term::projectenable()
{
  bool b=project.Id.size()>0;
  menuBar->runprojectAct->setEnabled(b);
  menuBar->projectcloseAct->setEnabled(b);
}

// ---------------------------------------------------------------------
void Term::resizeEvent(QResizeEvent *event)
{
  tedit->setresized(0);
  QWidget::resizeEvent(event);
}

// ---------------------------------------------------------------------
void Term::systimer()
{
  var_cmd("(i.0 0)\"_ sys_timer_z_$0");
}

// ---------------------------------------------------------------------
void Term::vieweditor()
{
  if (note)
    note->activate();
  else {
    note = new Note();
    if (recent.ProjectOpen)
      note->projectopen(true);
    note->show();
  }
}

// ---------------------------------------------------------------------
void smact()
{
#ifndef ANDROID
  term->activate();
#endif
}
