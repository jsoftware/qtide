/* J tedit */

#include <QApplication>
#include <QKeyEvent>
#include <QScrollBar>
#include <QTextBlock>
#include <QTimer>

#include "base.h"
#include "tedit.h"

#include "dlog.h"
#include "state.h"
#include "term.h"
#include "svr.h"

using namespace std;
extern bool runshow;

// ---------------------------------------------------------------------
Tedit::Tedit()
{
  smprompt="";
  type=0;
  ifResized=Tw=Th=0;
#ifdef QT_OS_ANDROID
  backButtonPressed=0;
#endif
  hScroll=horizontalScrollBar();
  ensureCursorVisible();
  setLineWrapMode(PlainTextEdit::NoWrap);
}

// ---------------------------------------------------------------------
// append at end of text
// (note appendPlainText appends to new line)
void Tedit::append(QString s)
{
  appendPlainText(s);
}

// ---------------------------------------------------------------------
// append smoutput if called from jedo
void Tedit::append_smoutput(QString s)
{
  removeprompt();
  if (s.size())
    appendPlainText(s + "\n" + getprompt());
  else
    appendPlainText(getprompt());
}

#ifdef QT_OS_ANDROID
// ---------------------------------------------------------------------
void Tedit::backButtonTimer()
{
  backButtonPressed=0;
}
#endif

// ---------------------------------------------------------------------
void Tedit::docmd(QString t)
{
  runshow=false;
  dlog_add(t);
  var_run(t);
}

// ---------------------------------------------------------------------
void Tedit::docmdp(QString t,bool show,bool same)
{
  bool cmt="NB."==dlb(t).left(3);
  runshow=same && t.size()>0 && !cmt;
  if (show)
    promptreplace(getprompt() + t);
  if (runshow) {
    dlog_add(t);
    var_run("output_jrx_=:i.0 0");
    var_run("output_jrx_=:"+t);
    var_run("output_jrx_");
  } else
    docmd(t);
}

// ---------------------------------------------------------------------
void Tedit::docmds(QString t, bool show)
{
  runshow=false;
  var_runs(t, show);
}

// ---------------------------------------------------------------------
void Tedit::docmdx(QString t)
{
  promptreplace(t);
  docmd(t);
}

// ---------------------------------------------------------------------
void Tedit::enter()
{
  QTextCursor c = textCursor();
  QString txt = c.block().text();
  int row = c.blockNumber();
  int len = blockCount();
  if (row < len - 1) {
    QString p=getprompt();
    int pos;
    if (config.KeepCursorPosOnRecall)
      pos=getpositioninblock(c);
    else
      pos=-1;
    if (initialblanks(p) == p.length()) {
      int pad=qMax(0,p.length() - initialblanks(txt));
      QString hdr(pad,' ');
      txt=hdr + txt;
      pos=pos + hdr.size();
    }
    promptreplace(txt,pos);
  } else
    docmd(txt.trimmed());
}

// ---------------------------------------------------------------------
QString Tedit::getprompt()
{
  if (smprompt.size()) {
    prompt=smprompt;
    smprompt="";
  }
  return prompt;
}

// ---------------------------------------------------------------------
// insert at end of text
// (note appendPlainText appends to new line)
void Tedit::insert(QString s)
{
  moveCursor(QTextCursor::End);
  insertPlainText(s);
}

// ---------------------------------------------------------------------
void Tedit::itemActivated(QListWidgetItem *item)
{
  term->activateWindow();
  term->raise();
  promptreplace(getprompt() + item->text());
}

// ---------------------------------------------------------------------
void Tedit::keyPressEvent(QKeyEvent *e)
{
  Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();
  bool shift = mod.testFlag(Qt::ShiftModifier);
  bool ctrl = mod.testFlag(Qt::ControlModifier);
  int key = e->key();

  if (ctrl>shift) {
    switch (key) {
    case Qt::Key_D :
      new Slog();
      break;
    case Qt::Key_Return:
      enter();
      break;
    default:
      Bedit::keyPressEvent(e);
    }
    return;
  }

  if (shift>ctrl) {
    switch (key) {
    case Qt::Key_Home:
      homeshift();
      break;
    default:
      Bedit::keyPressEvent(e);
    }
    return;
  }

  if (shift) {
    switch (key) {
    case Qt::Key_Up :
      promptreplace(dlog_scroll (-1));
      break;
    case Qt::Key_Down :
      promptreplace(dlog_scroll (1));
      break;
    default:
      Bedit::keyPressEvent(e);
    }
    return;
  }

  switch (key) {
  case Qt::Key_Home:
    home();
    break;
    return;
  case Qt::Key_Return:
  case Qt::Key_Enter:
    enter();
    break;
  case Qt::Key_Escape:
    e->ignore();
    break;

  default:
    Bedit::keyPressEvent(e);
  }
}

// ---------------------------------------------------------------------
void Tedit::keyReleaseEvent(QKeyEvent *event)
{
// separate ANDROID code avoids compiler warnings
#ifdef QT_OS_ANDROID
  switch (event->key()) {
  case Qt::Key_Back:
    if (2>backButtonPressed) {
      if (0==backButtonPressed) QTimer::singleShot(2000, this, SLOT(backButtonTimer()));
      backButtonPressed++;
    } else {
      if (!term->filequit(true))
        event->accept();
    }
    break;
  default:
    Bedit::keyReleaseEvent(event);
  }
#else
  Bedit::keyReleaseEvent(event);
#endif
}

// ---------------------------------------------------------------------
void Tedit::loadscript(QString s,bool show)
{
  tedit->docmdp(var_load(s,false),show, false);
}

// ---------------------------------------------------------------------
void Tedit::promptreplace(QString t,int pos)
{
  if (t.size() == 0) return;
  moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  moveCursor(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
  moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
  textCursor().removeSelectedText();
  append(t);
  if (pos>-1) {
    moveCursor(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
    setcurpos(pos+readcurpos());
  }
}

// ---------------------------------------------------------------------
void Tedit::removeprompt()
{
  moveCursor(QTextCursor::End);
  QTextCursor c = textCursor();
  if (c.block().text().trimmed().isEmpty()) {
    c.select(QTextCursor::BlockUnderCursor);
    c.removeSelectedText();
  }
}

// ---------------------------------------------------------------------
// run all script
// implemented as loadd ...
void Tedit::runall(QString s, bool show)
{
  runshow=false;
  var_run(var_load(s,show));
}

// ---------------------------------------------------------------------
void Tedit::setprompt()
{
  append(getprompt());
  hScroll->triggerAction(QScrollBar::SliderToMinimum);
  moveCursor(QTextCursor::End);
}

// ---------------------------------------------------------------------
void Tedit::setresized(int s)
{
  this->ifResized = s;
}

