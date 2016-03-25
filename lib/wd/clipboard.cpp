
#include <QApplication>
#include <QClipboard>
#include <QImage>

#include "../base/state.h"
#include "clipboard.h"

static QByteArray tmpba;

// ---------------------------------------------------------------------
int wdclipwrite(char *s)
{
  QClipboard *clipboard = app->clipboard();
  if (!clipboard) return 1;
  if (!s || !strlen(s)) clipboard->clear();
  else clipboard->setText(QString::fromUtf8(s));
  return 0;
}

// ---------------------------------------------------------------------
int wdclipwriteimage(char *s)
{
  QClipboard *clipboard = app->clipboard();
  if (!clipboard) return 1;
  if (!s || !strlen(s)) clipboard->clear();
  else {
    QImage image(s2q(s));
    if (image.isNull()) return 1;
    clipboard->setImage(image);
  }
  return 0;
}

// ---------------------------------------------------------------------
int wdclipwriteimagedata(const uchar *data,int len)
{
  QClipboard *clipboard = app->clipboard();
  if (!clipboard) return 1;
  if (!data || len<=0) clipboard->clear();
  else {
    QImage image=QImage();
    if (image.loadFromData(data, len) && !image.isNull()) {
      clipboard->setImage(image);
    } else return 1;
  }
  return 0;
}

// ---------------------------------------------------------------------
void *wdclipread(int *len)
{
  if (!len) {
    tmpba.clear();
    return 0;
  }
  QClipboard *clipboard = app->clipboard();
  if (!clipboard) return 0;
  tmpba = clipboard->text().toUtf8();
  *len = tmpba.size();
  if (tmpba.size()) return (void *)tmpba.data();
  else return 0;
}

// ---------------------------------------------------------------------
void *wdclipreadimage(char * s)
{
  QClipboard *clipboard = app->clipboard();
  if (!clipboard) return 0;
  if (!strlen(s)) return 0;
  QImage image = clipboard->image();
  if (!image.isNull()) {
    return (image.save(s))?(char *)-1:0;
  }
  return 0;
}
