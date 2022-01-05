
#include "edit.h"
#include "form.h"
#include "lineedit.h"
#include "../base/utils.h"

// ---------------------------------------------------------------------
LineEdit::LineEdit(Child *c, QWidget *parent) : QLineEdit(parent)
{
  Q_UNUSED(parent);
  pchild = c;
}

// ---------------------------------------------------------------------
void LineEdit::keyPressEvent(QKeyEvent *event)
{
  int key1=0;
  int key=event->key();
  if (ismodifier(key)) return;
  if (!(event->modifiers() & Qt::CTRL) && !(event->modifiers() & Qt::SHIFT)) {
    switch (key) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Escape:
      QLineEdit::keyPressEvent(event);
      return;
    default:
      break;
    }
  }
  if ((key==Qt::Key_unknown)||((key>=Qt::Key_F1)&&(key<=Qt::Key_F35))) {
    QLineEdit::keyPressEvent(event);
    return;
  } else
    key1=translateqkey(key);
  // Ctrl+anything becomes (possibly) a _fkey event; others become _char
  if (!event->modifiers().testFlag(Qt::ControlModifier)) {
    char sysdata[20];
    if (key==key1)
      sprintf(sysdata, "%s", event->text().toUtf8().constData());
    else sprintf(sysdata, "%s", QString(QChar(key1)).toUtf8().constData());

    pchild->event=std::string("char");
    pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
    pchild->sysdata=std::string(sysdata);
    pchild->pform->signalevent(pchild);
    // for ESC key, abort further processing lest we generate a second J event.
    if (key==Qt::Key_Escape) return;
  }
  QLineEdit::keyPressEvent(event);
}
