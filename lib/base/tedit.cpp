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

// using namespace std;
extern bool runcmd;
extern bool runshow;
extern int runterm;

// ---------------------------------------------------------------------
Tedit::Tedit()
{
  smprompt="";
  type=0;
  ifResized=Tw=Th=0;
  hScroll=horizontalScrollBar();
  ensureCursorVisible();
  setLineWrapMode(TextEdit::NoWrap);
}

// ---------------------------------------------------------------------
// append at end of text
// (note appendPlainText appends to new line)
void Tedit::append(QString s)
{
  if (runcmd)
    append_smoutput(s);
  else
    appendPlainText(s);
  runcmd=false;
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
#ifdef DEBUG_JDO
  qDebug() << "tedit docmd " + t;
#endif
  runshow=false;
  runterm=1;
  t=uptoLF(t);
  dlog_add(jecallback?t:t.trimmed());
  jcon->cmddo(q2s(t));
}

// ---------------------------------------------------------------------
void Tedit::docmddo(QString t)
{
#ifdef DEBUG_JDO
  qDebug() << "tedit docmddo " + t;
#endif
  t=uptoLF(t);
  runshow=false;
  runterm=0;
  jcon->cmddo(q2s(t));
}

// ---------------------------------------------------------------------
void Tedit::docmdp(QString t,bool show,bool same)
{
#ifdef DEBUG_JDO
  qDebug() << "tedit docmdp " + t;
#endif
  t=uptoLF(t);
  bool cmt="NB."==dlb(t).left(3);
  runshow=same && t.trimmed().size()>0 && !cmt;
  if (show)
    promptreplace(getprompt() + t);
  if (runshow) {
    dlog_add(jecallback?t:t.trimmed());
    jcon->cmddo("output_jrx_=:i.0 0");
    jcon->cmddo("output_jrx_=:"+q2s(t));
    runterm=jecallback ? 3 : 1;
    jcon->cmddo("output_jrx_");
  } else
    docmd(t);
}

// ---------------------------------------------------------------------
void Tedit::docmds(QString s,bool show,bool same,bool term)
{
#ifdef DEBUG_JDO
  qDebug() << "tedit docmds " + s;
#endif
  runshow=same;
  runterm=(show || term) ? 1 : 0;
  std::string f=show ? "0!:101" : "0!:100";
  jcon->set("inputx_jrx_",q2s(s));
  jcon->immex(f + " inputx_jrx_");
}

// ---------------------------------------------------------------------
void Tedit::docmdx(QString t)
{
  t=uptoLF(t);
  runterm=1;
  promptreplace(t);
  docmd(t);
}

// ---------------------------------------------------------------------
void Tedit::enter()
{
  QTextCursor c = textCursor();
  QString txt = c.block().text();
  txt=uptoLF(txt);
  int row = c.blockNumber();
  int len = document()->blockCount();
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
    docmd(txt);
}

// ---------------------------------------------------------------------
QString Tedit::getprompt()
{
  if (smprompt.size()) {
    prompt=smprompt;
    smprompt="";
  } else if (!jecallback)
    prompt="   ";
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
  Qt::KeyboardModifiers mod = app->keyboardModifiers();
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
      TextEdit::keyPressEvent(e);
    }
    return;
  }

  if (shift>ctrl) {
    switch (key) {
    case Qt::Key_Home:
      homeshift();
      break;
    default:
      TextEdit::keyPressEvent(e);
    }
    return;
  }

  if (shift) {
    switch (key) {
    case Qt::Key_Up :
      promptreplace(dlog_scroll(-1));
      break;
    case Qt::Key_Down :
      promptreplace(dlog_scroll(1));
      break;
    default:
      TextEdit::keyPressEvent(e);
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
    TextEdit::keyPressEvent(e);
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
  QString r = d ? "loadd " : "load ";
  return r + quote(s);
}

// ---------------------------------------------------------------------
void Tedit::loadscript(QString s,bool show)
{
  tedit->docmdp(loadcmd(s,false),show, false);
}

// ---------------------------------------------------------------------
void Tedit::promptreplace(QString t,int pos)
{
  t=uptoLF(t);
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
  runterm=1;
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
  runcmd=true;
}

// ---------------------------------------------------------------------
void Tedit::setresized(int s)
{
  this->ifResized = s;
}
