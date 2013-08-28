
#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>

#include "base.h"
#include "bedit.h"
#include "state.h"

// ---------------------------------------------------------------------
Bedit::Bedit(QWidget *parent) : QPlainTextEdit(parent)
{
  lineNumberArea = new LineNumberArea(this);
  document()->setDocumentMargin(0);

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

  if (config.LineWrap)
    setLineWrapMode(QPlainTextEdit::WidgetWidth);

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();
}

// ---------------------------------------------------------------------
int Bedit::lineNumberAreaWidth()
{
  if (!config.LineNos) return 2;
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }
  digits=qMax(2,digits);
  int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;
  return space;
}

// ---------------------------------------------------------------------
void Bedit::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;
    QColor lineColor = (type==0)?config.TermHigh.color:config.EditHigh.color;
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }
  setExtraSelections(extraSelections);
}

// ---------------------------------------------------------------------
void Bedit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  if (!config.LineNos) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), config.TermBack.color);
    return;
  }

  QPainter painter(lineNumberArea);
  QColor fillColor = (type==0)?config.TermHigh.color:config.EditHigh.color;
  painter.fillRect(event->rect(), fillColor);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int) blockBoundingRect(block).height();
  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width()-5, fontMetrics().height(),
                       Qt::AlignRight, number);
    }
    block = block.next();
    top = bottom;
    bottom = top + (int) blockBoundingRect(block).height();
    ++blockNumber;
  }
}

// ---------------------------------------------------------------------
int Bedit::readcurpos()
{
  return textCursor().position();
}

// ---------------------------------------------------------------------
// get class, position and text
QString Bedit::readhelptext(int t)
{
  int bgn, end;
  QString txt=toPlainText();
  QTextCursor c = textCursor();
  bgn=c.selectionStart();
  end=c.selectionEnd();
  QString hdr=QString::number(t)+" "
              +QString::number(bgn)+" "
              +QString::number(end)+" ";
  return hdr+txt;
}

// ---------------------------------------------------------------------
QString Bedit::readselected()
{
  QString s=textCursor().selectedText();
  s.replace(0x2029,'\n');
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
void Bedit::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);
  QRect cr = contentsRect();
  lineNumberArea->setGeometry(QRect(cr.left(),cr.top(),lineNumberAreaWidth(),cr.height()));
}

// ---------------------------------------------------------------------
// code here just forces a resize event when setting linenos
// there should be a better way...
void Bedit::resizer()
{
  updateLineNumberAreaWidth(0);
  int w=size().width();
  int h=size().height();
  resize(w,h-1);
  resize(w,h);
}

// ---------------------------------------------------------------------
void Bedit::selectline(int p)
{
  if (0>p) return;
  QTextCursor c = textCursor();
  int d = p-c.blockNumber();
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

// ---------------------------------------------------------------------
void Bedit::updateLineNumberAreaWidth(int newBlockCount)
{
  Q_UNUSED(newBlockCount);
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

// ---------------------------------------------------------------------
void Bedit::updateLineNumberArea(const QRect &rect, int dy)
{
  if (!config.LineNos) return;
  if (dy)
    lineNumberArea->scroll(0, dy);
  else
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}
