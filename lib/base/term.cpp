#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#ifdef TABCOMPLETION
#include <QCompleter>
QCompleter *completer=0;
#endif

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

#ifdef QT_OS_ANDROID
#include "../wd/form.h"
static int fkeys[]= {Qt::Key_F1,Qt::Key_F2,Qt::Key_F3,Qt::Key_F4,Qt::Key_F5,Qt::Key_F6,Qt::Key_F7,Qt::Key_F8,Qt::Key_F9,Qt::Key_F10,Qt::Key_F11,Qt::Key_F12};
#endif

using namespace std;

Term *term=0;
Tedit *tedit=0;

QString LastLaunch;
QTime LastLaunchTime;
QTimer *timer=0;

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
  term->filequit(true);
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
#ifdef QT_OS_ANDROID
#ifdef SMALL_SCREEN
#define nfunc 6
#else
#define nfunc 12
#endif
  QPushButton *w[nfunc];
  if ((1==androidVfuncPos)||(2==androidVfuncPos)) {
    vfunc=new QHBoxLayout;
    for(int i=0; i<nfunc; i++) {
      w[i]=new QPushButton("F"+QString::number(i+1),this);
      w[i]->setObjectName(QString::number(i+1));
      w[i]->setFocusPolicy(Qt::NoFocus);
      QObject::connect(w[i], SIGNAL(clicked()), this, SLOT(vfuncClicked()));
      vfunc->addWidget(w[i]);
    }
  }
#endif
  layout->addWidget(menuBar);
#ifdef QT_OS_ANDROID
  if (1==androidVfuncPos)
    layout->addLayout(vfunc);
#endif
  layout->addWidget(tedit);
#ifdef QT_OS_ANDROID
  if (2==androidVfuncPos)
    layout->addLayout(vfunc);
#endif
  setWindowTitle("Term");
  menuBar->createActions();
  menuBar->createMenus("term");
  setLayout(layout);
  timer=new QTimer;
  connect(timer, SIGNAL(timeout()),this,SLOT(systimer()));
  QMetaObject::connectSlotsByName(this);
}

// ---------------------------------------------------------------------
void Term::activate()
{
  if (!term->isVisible()) return;
  activateWindow();
  raise();
  tedit->setFocus();
}

// ---------------------------------------------------------------------
void Term::changeEvent(QEvent *event)
{
  if (event->type()==QEvent::ActivationChange && isActiveWindow())
    setactivewindow(this);
  QWidget::changeEvent(event);
}

// ---------------------------------------------------------------------
void Term::cleantemp()
{
  QRegExp re("\\d*");
  QDir d=QDir(cpath("~temp"));
  d.setFilter(QDir::Files|QDir::Writable);
  QStringList t=d.entryList(QStringList() << "*.ijs");
  foreach (QString e,t)
    if (re.exactMatch(e.left(e.size()-4))) {
      QFile f(d.filePath(e));
      if (f.size()==0)
        f.remove();
    }
}

// ---------------------------------------------------------------------
void Term::closeEvent(QCloseEvent *event)
{
  filequit(false);
  event->ignore();
}

// ---------------------------------------------------------------------
bool Term::filequit(bool ignoreconfirm)
{
  dlog_write();
  if (note && (!note->saveall())) return false;
  if (note2 && (!note2->saveall())) return false;

// save clipboard
  QClipboard *clipboard = QApplication::clipboard();
  QEvent e=QEvent(QEvent::Clipboard);
  QApplication::sendEvent(clipboard,&e);

#ifdef QT_OS_ANDROID
// QMessageBox not work inside keypress event
  if (ignoreconfirm) {
#else
  Q_UNUSED(ignoreconfirm);
  if ((!config.ConfirmClose) ||
      queryOK("Term","OK to exit " + config.Lang + "?")) {
#endif
    var_cmddo("2!:55[0");
    cleantemp();
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
  tedit->setFont(config.Font);
  QPalette p = palette();
  p.setColor(QPalette::Active, QPalette::Base, config.TermBack.color);
  p.setColor(QPalette::Inactive, QPalette::Base, config.TermBack.color);
  p.setColor(QPalette::Text, config.TermFore.color);
  tedit->setPalette(p);
  setWindowIcon(QIcon(":/images/jgreen.png"));
  if (config.TermSyntaxHighlight)
    highlight(tedit->document());
#ifdef QT_OS_ANDROID
  tedit->setStyleSheet(scrollbarstyle(config.ScrollBarSize*DM_density));
#endif
#ifdef TABCOMPLETION
  completer = new QCompleter(this);
  completer->setModel(getcompletermodel(completer,config.ConfigPath.filePath(config.CompletionFile)));
  completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setWrapAround(false);
  if (config.Completion)
    tedit->setCompleter(completer);
  else
    tedit->setCompleter(0);
#endif
  tedit->setprompt();
  if (config.SingleWin)
    new OneWin();
  else if (ShowIde)
    show();
  move(config.TermPosX[0],config.TermPosX[1]);
  resize(config.TermPosX[2],config.TermPosX[3]);
}

// ---------------------------------------------------------------------
void Term::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
#ifdef JQT
  case Qt::Key_Escape:
    if (config.EscClose) {
      if (!filequit(false))
        event->accept();
    }
    break;
#endif
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
void Term::refresh()
{
  if (!term->isVisible()) return;
  tedit->resizer();
}

// ---------------------------------------------------------------------
void Term::removeprompt()
{
  tedit->removeprompt();
}

// ---------------------------------------------------------------------
void Term::resizeEvent(QResizeEvent *event)
{
  tedit->setresized(0);
  QWidget::resizeEvent(event);
}

// ---------------------------------------------------------------------
void Term::smact()
{
  if (!term->isVisible()) return;
  term->activateWindow();
  term->raise();
  term->repaint();
}

// ---------------------------------------------------------------------
void Term::smprompt(QString s)
{
  tedit->smprompt=s;
}

// ---------------------------------------------------------------------
void Term::systimer()
{
  var_cmddo("(i.0 0)\"_ sys_timer_z_$0");
}

// ---------------------------------------------------------------------
void Term::vieweditor()
{
  if (note) {
    note->activate();
  } else {
    note = new Note();
    if (recent.ProjectOpen)
      note->projectopen(true);
    note->show();
  }
}

#ifdef QT_OS_ANDROID
// ---------------------------------------------------------------------
void Term::vfuncClicked()
{
// menu shortcut does not work in android
  int c = sender()->objectName().toInt() - 1;
  switch (fkeys[c]) {
  case Qt::Key_F1:
    if (!Forms.isEmpty()) {
      form=Forms.last();
      wdactivateform();
    } else term->repaint();
    break;
  case Qt::Key_F2:
    tedit->cu0 = tedit->textCursor();
    break;
  case Qt::Key_F6:
    tedit->docmds("labs_run_jqtide_ 0", false);
    break;
  default:
//    tedit->docmds("fkey"+sender()->objectName()+"_run_jqtide_$0", false);
    break;
  }
}
#endif

