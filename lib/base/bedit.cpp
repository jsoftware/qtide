
#include <QScrollBar>
#include <QTextBlock>
#ifdef TABCOMPLETION
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QAbstractItemModel>
#endif

#include "plaintextedit.h"
#include "base.h"
#include "state.h"
#include "bedit.h"

// ---------------------------------------------------------------------
Bedit::Bedit(QWidget *parent) : PlainTextEditLn(parent)
{
#ifdef TABCOMPLETION
  c= 0 ;
#endif

  settabwidth();
  if (config.LineWrap)
    setLineWrapMode(PlainTextEdit::WidgetWidth);
}

// ---------------------------------------------------------------------
Bedit::~Bedit()
{
}

// ---------------------------------------------------------------------
void Bedit::home()
{
  QString ws=" \t";
  QTextCursor c = textCursor();
  QString txt = c.block().text();
  int pos=getpositioninblock(c);
  txt = txt.left(pos);
  int mov=0;
  for (int i=0; i<pos; i++)
    if (!ws.contains(txt.at(i))) {
      mov=i;
      break;
    }
  c.setPosition(c.position()-pos+mov);
  setTextCursor(c);
}

// ---------------------------------------------------------------------
void Bedit::homeshift()
{
  QString ws=" \t";
  QTextCursor c = textCursor();
  QString txt = c.block().text();
  int pos=getpositioninblock(c);
  txt = txt.left(pos);
  int mov=0;
  for (int i=0; i<pos; i++)
    if (!ws.contains(txt.at(i))) {
      mov=i;
      break;
    }
  int cur=c.position();
  setselect(cur-pos+mov,pos-mov);
}


// ---------------------------------------------------------------------
int Bedit::readcurpos()
{
  return textCursor().position();
}

// ---------------------------------------------------------------------
QString Bedit::readselected()
{
  QString s=textCursor().selectedText();
  s.replace((short)0x2029,'\n');
  return s;
}

// ---------------------------------------------------------------------
// get lines with selected text
QString Bedit::readselect_line(int *pos, int *len)
{
  int bgn, end;
  QString txt=toPlainText();
  QTextCursor c = textCursor();
  bgn=c.selectionStart();
  end=c.selectionEnd();

  if (bgn)
    bgn=1+txt.lastIndexOf('\n',bgn-1);
  if (bgn < end && txt[end-1]=='\n')
    end--;
  end=txt.indexOf('\n',end);

  if (end==-1) end=txt.size();
  *pos=bgn;
  *len=end-bgn;
  return txt;
}

// ---------------------------------------------------------------------
// get selected text
QString Bedit::readselect_text(int *pos, int *len)
{
  int bgn, end;
  QTextCursor c = textCursor();
  bgn=c.selectionStart();
  end=c.selectionEnd();
  *pos=bgn;
  *len=end-bgn;
  return toPlainText();
}

// ---------------------------------------------------------------------
int Bedit::readtop()
{
  return qMax(0,firstVisibleBlock().blockNumber());
}

// ---------------------------------------------------------------------
void Bedit::selectline(int p)
{
  if (0>p) return;
  QTextCursor c = textCursor();
  int d = p-c.blockNumber();

  int t=qMax(0,firstVisibleBlock().blockNumber());
  QPoint bottom(0,viewport()->height()-1);
  int b = cursorForPosition(bottom).blockNumber();
  if (p<t || p>b)
    settop(qMax(0,p-(b-t)/2));

  if (d<0)
    c.movePosition(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor,-d);
  else if (d>0)
    c.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,d);
  c.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
  c.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
  setTextCursor(c);
}

// ---------------------------------------------------------------------
void Bedit::setcurpos(int pos)
{
  QTextCursor c=textCursor();
  c.setPosition(pos);
  setTextCursor(c);
}

// ---------------------------------------------------------------------
void Bedit::setselect(int p, int len)
{
  QTextCursor c = textCursor();
  c.setPosition(p+len,QTextCursor::MoveAnchor);
  c.setPosition(p,QTextCursor::KeepAnchor);
  setTextCursor(c);
}

// ---------------------------------------------------------------------
void Bedit::settop(int p)
{
  int len = blockCount()-p-1;
  QTextCursor c = textCursor();
  c.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
  c.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
  setTextCursor(c);
  for (int i=0; i<len; i++)
    c.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor);
  setTextCursor(c);
}


#ifdef TABCOMPLETION
// ---------------------------------------------------------------------
void Bedit::setCompleter(QCompleter *completer)
{
  if (c) QObject::disconnect(c, 0, this, 0);
  c = completer;
  if (!c) return;

  c->setWidget(this);
  c->setCompletionMode(QCompleter::PopupCompletion);
  c->setCaseSensitivity(Qt::CaseInsensitive);
  QObject::connect(c, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
}

// ---------------------------------------------------------------------
QCompleter *Bedit::completer() const
{
  return c;
}

// ---------------------------------------------------------------------
void Bedit::insertCompletion(const QString& completion)
{
  if (c->widget() != this) return;
  QTextCursor tc = textCursor();
  int extra = completion.length() - c->completionPrefix().length();
  tc.movePosition(QTextCursor::Left);
  tc.movePosition(QTextCursor::EndOfWord);
  tc.insertText(completion.right(extra));
  setTextCursor(tc);
}

// ---------------------------------------------------------------------
QString Bedit::textUnderCursor() const
{
  QTextCursor tc = textCursor();
  tc.select(QTextCursor::WordUnderCursor);
  return tc.selectedText();
}

// ---------------------------------------------------------------------
void Bedit::focusInEvent(QFocusEvent *e)
{
  if (c) c->setWidget(this);
  PlainTextEdit::focusInEvent(e);
}

// ---------------------------------------------------------------------
void Bedit::keyPressEvent(QKeyEvent *e)
{
  if (c && c->popup()->isVisible()) {
    // The following keys are forwarded by the completer to the widget
    switch (e->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Escape:
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
      e->ignore();
      return; // let the completer do default behavior
    default:
      break;
    }
  }

  bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_P); // CTRL+P
  if (!c || !isShortcut) // do not process the shortcut when we have a completer
    PlainTextEdit::keyPressEvent(e);

  const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (!c || (ctrlOrShift && e->text().isEmpty()))
    return;

  static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
  bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
  QString completionPrefix = textUnderCursor();

  if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3 || eow.contains(e->text().right(1)))) {
    c->popup()->hide();
    return;
  }

  if (completionPrefix != c->completionPrefix()) {
    c->setCompletionPrefix(completionPrefix);
    c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
  }
  QRect cr = cursorRect();
  cr.setWidth(c->popup()->sizeHintForColumn(0) + c->popup()->verticalScrollBar()->sizeHint().width());
  c->complete(cr); // popup it up!
}
#endif
