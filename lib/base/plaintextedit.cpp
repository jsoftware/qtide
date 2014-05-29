
#include "plaintextedit.h"
#include "state.h"

// ---------------------------------------------------------------------
PlainTextEdit::PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
#ifdef QT_OS_ANDROID
  setStyleSheet(scrollbarstyle(config.ScrollBarSize*DM_density));
#endif
  setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
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
