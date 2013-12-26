/* J tedit */

#include <QApplication>
#include <QKeyEvent>
#include <QScrollBar>
#include <QTextBlock>

#include "base.h"
#include "tedit.h"

#include "dlog.h"
#include "state.h"
#include "term.h"
#include "svr.h"

using namespace std;


// ---------------------------------------------------------------------
Tedit::Tedit()
{
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
  moveCursor(QTextCursor::End);
  QTextCursor c = textCursor();
  if (c.block().text().trimmed().isEmpty()) {
    c.select(QTextCursor::BlockUnderCursor);
    c.removeSelectedText();
  }
  appendPlainText(s + "\n" + getprompt());
}

// ---------------------------------------------------------------------
void Tedit::docmd(QString t)
{
  dlog_add(t);
  var_run(t);
}

// ---------------------------------------------------------------------
void Tedit::docmdp(QString t,bool show)
{
  if (show)
    promptreplace(getprompt() + t);
  docmd(t);
}

// ---------------------------------------------------------------------
void Tedit::docmds(QString t)
{
  var_runs(t);
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
  if (row < len - 1)
    promptreplace(txt);
  else
    docmd(txt.trimmed());
}

// ---------------------------------------------------------------------
QString Tedit::getprompt()
{
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
      PlainTextEdit::keyPressEvent(e);
    }
    return;
  }

  if (shift>ctrl) {
    PlainTextEdit::keyPressEvent(e);
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
      PlainTextEdit::keyPressEvent(e);
    }
    return;
  }

  switch (key) {
  case Qt::Key_Return:
  case Qt::Key_Enter:
    enter();
    break;
  case Qt::Key_Escape:
    e->ignore();
    break;

  default:
    PlainTextEdit::keyPressEvent(e);
  }
}

// ---------------------------------------------------------------------
void Tedit::loadscript(QString s,bool show)
{
  tedit->docmdp(var_load(s,false),show);
}

// ---------------------------------------------------------------------
void Tedit::promptreplace(QString t)
{
  if (t.size() == 0) return;
  moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  moveCursor(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
  moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
  textCursor().removeSelectedText();
  append(t);
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

