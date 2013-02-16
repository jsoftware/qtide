
#include <QApplication>
#include <QClipboard>

#include "../base/base.h"
#include "clipboard.h"

static QByteArray tmpba;

// ---------------------------------------------------------------------
int wdclipwrite(char *s)
{
  QClipboard *clipboard = QApplication::clipboard();
  if (!clipboard) return 1;
  if (!s || !strlen(s)) clipboard->clear();
  else clipboard->setText(QString::fromUtf8(s));
  return 0;
}

// ---------------------------------------------------------------------
void *wdclipread(int *len)
{
  if (!len) {
    tmpba.clear();
    return 0;
  }
  QClipboard *clipboard = QApplication::clipboard();
  if (!clipboard) return 0;
  tmpba = clipboard->text().toUtf8();
  *len = tmpba.size();
  if (tmpba.size()) return (void *)tmpba.data();
  else return 0;
}
