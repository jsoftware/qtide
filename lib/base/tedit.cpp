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
extern bool runterm;

// ---------------------------------------------------------------------
Tedit::Tedit()
{
  smprompt="";
  type=0;
  ifResized=Tw=Th=0;
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

// ---------------------------------------------------------------------
void Tedit::docmd(QString t)
{
  runshow=false;
  runterm=true;
  dlog_add(t);
  jcon->cmddo(q2s(t));
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
    jcon->cmddo("output_jrx_=:i.0 0");
    jcon->cmddo("output_jrx_=:"+q2s(t));
    runterm=true;
    jcon->cmddo("output_jrx_");
  } else
    docmd(t);
}

// ---------------------------------------------------------------------
void Tedit::docmds(QString s, bool show,bool same)
{
  runshow=same;
  runterm=true;
  string f=show ? "0!:101" : "0!:100";
  jcon->set("inputx_jrx_",q2s(s));
  jcon->immex(f + " inputx_jrx_");
}

// ---------------------------------------------------------------------
void Tedit::docmdx(QString t)
{
  runterm=true;
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
    int pad=0;
    int pos=-1;
    if (initialblanks(p) == p.length()) {
      pad=qMax(0,p.length() - initialblanks(txt));
      QString hdr(pad,' ');
      txt=hdr + txt;
    }
    if (config.KeepCursorPosOnRecall)
      pos=getpositioninblock(c) + pad;
    promptreplace(txt,pos);
  } else
    docmd(txt.trimmed());
}

// ---------------------------------------------------------------------
QString Tedit::getprompt()
{
  QString r=jecallback ? "      " : "   ";
  if (smprompt.size()) {
    r=smprompt;
    smprompt="";
  }
  return r;
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
void Tedit::load(QString s, bool d)
{
  docmdx((loadcmd(s,d)));
}

// ---------------------------------------------------------------------
QString Tedit::loadcmd(QString s, bool d)
{
  QString r = d ? "loadd" : "load";
  return r + " '" + s + "'";
}

// ---------------------------------------------------------------------
void Tedit::loadscript(QString s,bool show)
{
  tedit->docmdp(loadcmd(s,false),show, false);
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
  } else
    moveCursor(QTextCursor::End);
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
  runterm=true;
  jcon->cmddo(q2s(loadcmd(s,show)));
}

// ---------------------------------------------------------------------
void Tedit::setprompt()
{
  append(getprompt());
  hScroll->triggerAction(QScrollBar::SliderToMinimum);
  moveCursor(QTextCursor::End);
}

// ---------------------------------------------------------------------
void Tedit::showcmd(QString t)
{
  append_smoutput(getprompt()+t);
}

// ---------------------------------------------------------------------
void Tedit::setresized(int s)
{
  this->ifResized = s;
}
