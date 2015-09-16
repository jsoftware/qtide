
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#else
#include <QPrinter>
#endif
#endif

#include "plaintextedit.h"
#include "state.h"

// ---------------------------------------------------------------------
PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void PlainTextEdit::printPreview(QPrinter * printer)
{
  QTextDocument *dd=document()->clone();
#ifdef QT50
  dd->documentLayout()->setPaintDevice((QPagedPaintDevice *)printer);
  dd->setPageSize(QSizeF(printer->pageRect().size()));
  dd->print((QPagedPaintDevice *)printer);
#else
  dd->documentLayout()->setPaintDevice(printer);
  dd->setPageSize(QSizeF(printer->pageRect().size()));
  dd->print(printer);
#endif
  delete dd;
}
#endif
