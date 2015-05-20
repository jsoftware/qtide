
#ifndef QT_NO_PRINTER
#include <QtPrintSupport/QPrinter>
#endif

#include "plaintextedit.h"
#include "state.h"

// ---------------------------------------------------------------------
PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
  acceptKeyBack=false;
}

// ---------------------------------------------------------------------
void PlainTextEdit::keyReleaseEvent(QKeyEvent *event)
{
#ifdef QT_OS_ANDROID
  if ((event->key()==Qt::Key_Back) && (!acceptKeyBack)) {
    QPlainTextEdit::keyReleaseEvent(event);
    event->ignore();
    return;
  } else QPlainTextEdit::keyReleaseEvent(event);
#else
  QPlainTextEdit::keyReleaseEvent(event);
#endif
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void PlainTextEdit::print(QPrinter * printer)
{
  QPlainTextEdit::print(printer);
}
#endif
