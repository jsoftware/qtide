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
  Q_UNUSED(event);
  term->filequit(true);
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
  if (!ShowIde) setVisible(ShowIde);
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
void Term::displayform(int p)
{
  jcon->cmddo("0 0$9!:3[" + q2s((QString::number(p))));
}

// ---------------------------------------------------------------------
bool Term::filequit(bool ignoreconfirm)
{
  dlog_write();
  if (note && (!note->saveall())) return false;
  if (note2 && (!note2->saveall())) return false;

// save clipboard
  QClipboard *clipboard = app->clipboard();
  QEvent e=QEvent(QEvent::Clipboard);
  app->sendEvent(clipboard,&e);

  if (ignoreconfirm ||
      (!config.ConfirmClose) ||
      queryOK("Term","OK to exit " + config.Lang + "?")) {
    jcon->cmd("2!:55[0");
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
  p.setColor(QPalette::Base, config.TermBack.color);
  p.setColor(QPalette::Text, config.TermFore.color);
  p.setColor(QPalette::Highlight, config.TermSelb.color);
  p.setColor(QPalette::HighlightedText, config.TermSelt.color);
  tedit->setPalette(p);
  setWindowIcon(QIcon(":/images/jgreen.png"));
  if (config.TermSyntaxHighlight)
    highlight(tedit->document());
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
  case Qt::Key_Escape:
    if (config.EscClose)
      filequit(false);
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
  tedit->load(s,d);
}

// ---------------------------------------------------------------------
void Term::pacman()
{
  jcon->cmddo("require 'pacman ~addons/ide/qt/pacman.ijs'");
  jcon->cmddo("runpacman_jpacman_ 0");
}

// ---------------------------------------------------------------------
void Term::projectenable()
{
  bool b=project.Id.size()>0;
  menuBar->runprojectAct->setEnabled(b);
  menuBar->projectcloseAct->setEnabled(b);
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
  jcon->cmddo("(i.0 0)\"_ sys_timer_z_$0");
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
    note->activate();
  }
}

