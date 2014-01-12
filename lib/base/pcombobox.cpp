
#include "pcombobox.h"

// ---------------------------------------------------------------------
PComboBox::PComboBox(QWidget *parent) : QComboBox(parent)
{
  acceptKeyBack=false;
}

// ---------------------------------------------------------------------
void PComboBox::keyReleaseEvent(QKeyEvent *event)
{
#ifdef QT_OS_ANDROID
  if ((event->key()==Qt::Key_Back) && (!isEditable()) && (!acceptKeyBack)) {
    QComboBox::keyReleaseEvent(event);
    event->ignore();
    return;
  } else QComboBox::keyReleaseEvent(event);
#else
  QComboBox::keyReleaseEvent(event);
#endif
}
