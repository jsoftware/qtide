
#include <QImage>
#include <QBuffer>

#include "bitmap.h"

static QImage tmpimage;
static QByteArray tmpba;

// ---------------------------------------------------------------------
void *wdreadimg(char *s, int *wh)
{
  if (!tmpimage.isNull()) tmpimage=QImage();
  if (!s || !wh || !strlen(s)) return 0;
  if (!strlen(s)) return 0;
  QImage image=QImage(s);
  if (!image.isNull()) {
    wh[0]=image.width();
    wh[1]=image.height();
    tmpimage = image.convertToFormat(QImage::Format_ARGB32);
    return (void *) tmpimage.bits();
  } else return 0;
}

// ---------------------------------------------------------------------
void *wdgetimg(uchar *data, int len, int *wh)
{
  if (!tmpimage.isNull()) tmpimage=QImage();
  if (!data || !wh || (len<=0)) return 0;
  QImage image=QImage();
  if (image.loadFromData(data, len) && !image.isNull()) {
    wh[0]=image.width();
    wh[1]=image.height();
    tmpimage = image.convertToFormat(QImage::Format_ARGB32);
    return (void *) tmpimage.bits();
  } else return 0;
}

/*
Format              Description               Qt's support
BMP    Windows Bitmap                         Read/write
GIF    Graphic Interchange Format (optional)  Read
JPG    Joint Photographic Experts Group       Read/write
JPEG   Joint Photographic Experts Group       Read/write
PNG    Portable Network Graphics              Read/write
PBM    Portable Bitmap                        Read
PGM    Portable Graymap                       Read
PPM    Portable Pixmap                        Read/write
TIFF   Tagged Image File Format               Read/write
XBM    X11 Bitmap                             Read/write
XPM    X11 Pixmap                             Read/write
*/

// ---------------------------------------------------------------------
int wdwriteimg(uchar *p, int *wh, char *f, char *format, int quality)
{
  if (!p||!wh||!f) return 0;
  if (wh[0]==0||wh[1]==0) return 0;
  QImage image = QImage(wh[0],wh[1],QImage::Format_ARGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*wh[0]*wh[1]);
  return image.save(QString::fromUtf8(f), format, quality);
}

// ---------------------------------------------------------------------
void *wdputimg(uchar *p, int *wh, int *len, char *format, int quality)
{
  if (!tmpba.isNull()) tmpba=QByteArray();
  if (!p || !wh || !len) return 0;
  if (wh[0]==0||wh[1]==0) return 0;
  QImage image = QImage(wh[0],wh[1],QImage::Format_ARGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*wh[0]*wh[1]);
  QBuffer buffer(&tmpba);
  buffer.open(QIODevice::WriteOnly);
  if (!(image.save(&buffer, format, quality))) return 0;
  buffer.close();
  *len=tmpba.size();
  return (void *) tmpba.data();
}

