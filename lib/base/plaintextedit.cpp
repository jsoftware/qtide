
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#else
#include <QPrinter>
#endif
#endif

#include <QPainter>
#include <QTextBlock>

#include "plaintextedit.h"
#include "dialog.h"
#include "state.h"

// ---------------------------------------------------------------------
PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
  highlighter=0;
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void PlainTextEdit::print(QPrinter * printer)
{
  QTextDocument *d = document()->clone();
  if(highlighter) highlighter(d);
  d->print(printer);
  delete d;
}

void PlainTextEdit::printPreview(QPrinter * printer)
{
  QTextDocument *d = document()->clone();
  if(highlighter) highlighter(d);
  printpreview(printer,d);
  delete d;
}
#endif

// ---------------------------------------------------------------------
PlainTextEditLn::PlainTextEditLn(QWidget *parent) : PlainTextEdit(parent)
{
  lineNumberArea = new LineNumberArea(this);
  document()->setDocumentMargin(0);

  setCursorWidth(config.CursorWidth);
  settabwidth();
  showNos=false;
  type="edit";

  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

  highlightCurrentLine();
  updateLineNumberAreaWidth(0);
}

// ---------------------------------------------------------------------
void PlainTextEditLn::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;
    QColor lineColor = (type=="edit") ? config.EditHigh.color : config.ViewHigh.color;
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }
  setExtraSelections(extraSelections);
}

// ---------------------------------------------------------------------
int PlainTextEditLn::lineNumberAreaWidth()
{
  if (!showlineNumbers()) return 2;
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }
  digits=qMax(2,digits);
#ifdef QT512
  int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
#else
  int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;
#endif
  return space;
}

// ---------------------------------------------------------------------
void PlainTextEditLn::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  if (!showlineNumbers()) {
    QPainter painter(lineNumberArea);
    QColor backColor = (type=="edit") ? config.EditBack.color : config.ViewBack.color;
    painter.fillRect(event->rect(),backColor);
    return;
  }

  QPainter painter(lineNumberArea);
  QColor fillColor = (type=="edit") ? config.EditHigh.color : config.ViewHigh.color;
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
void PlainTextEditLn::resizeEvent(QResizeEvent *e)
{
  PlainTextEdit::resizeEvent(e);
  QRect cr = contentsRect();
  lineNumberArea->setGeometry(QRect(cr.left(),cr.top(),lineNumberAreaWidth(),cr.height()));
}

// ---------------------------------------------------------------------
// force resize event when setting linenos
void PlainTextEditLn::resizer()
{
  updateLineNumberAreaWidth(0);
  int w=size().width();
  int h=size().height();
  resize(w,h-1);
  resize(w,h);
}

// ---------------------------------------------------------------------
// in Qt5.10 setTabStopWidth was replaced by setTabStopDistance which
//  takes a real argument. The integer should be OK for most fixed fonts.
void PlainTextEditLn::settabwidth()
{
  int w=80;
  if (config.TabWidth >= 0)
    w=config.TabWidth * QFontMetrics(font()).averageCharWidth();
  setTabStopWidth(w);
}

// ---------------------------------------------------------------------
bool PlainTextEditLn::showlineNumbers()
{
  return (type=="edit") ? config.LineNos : showNos;
}

// ---------------------------------------------------------------------
void PlainTextEditLn::updateLineNumberAreaWidth(int newBlockCount)
{
  Q_UNUSED(newBlockCount);
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

// ---------------------------------------------------------------------
void PlainTextEditLn::updateLineNumberArea(const QRect &rect, int dy)
{
  if (!showlineNumbers()) return;
  if (dy)
    lineNumberArea->scroll(0, dy);
  else
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}

