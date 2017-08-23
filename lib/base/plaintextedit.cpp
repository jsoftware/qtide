
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#else
#include <QPrinter>
#endif
#endif

#include "plaintextedit.h"
#include "dialog.h"
#include "state.h"

// ---------------------------------------------------------------------
PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void PlainTextEdit::printPreview(QPrinter * printer)
{
  printpreview(printer,document());
}
#endif
