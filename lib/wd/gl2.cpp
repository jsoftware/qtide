
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef Max
#define Max(a, b) ((a) < (b) ? (b) : (a))
#endif

#include "wd.h"
#include "font.h"
#include "gl2.h"
#include "isigraph.h"
#include "isigraph2.h"
#include "form.h"
#include "../base/state.h"
extern QList<Form *>Forms;

static int glfont0 (Isigraph *isigraph, char *s);

// ---------------------------------------------------------------------
// caller should free string
static char *
int2utf8 (const int *yi, int nc)
{
  int i;
  char *ys = (char *) malloc (1 + (size_t) nc);
  for (i = 0; i < nc; i++)
    *(ys + i) = *(yi + i);
  *(ys + nc) = 0;
  return ys;
}

static void
qtarcisi (const int *y, const int *y2, int *ang)
{
// center
  int xc = *y + 0.5 * *(y + 2);
  int yc = *(y + 1) + 0.5 * *(y + 3);
// 0 is 3-o'clock, and counter-clockwise is positive, so that no need to adjust angle
  int alpha = atan2 ((float)*(y2 + 1) - yc, (float)*(y2) - xc) * 16 * 180 / M_PI;          // ant2 (ya - yc),(xa - xc)
  int beta = atan2 ((float)*(y2 + 3) - yc, (float)*(y2 + 2) - xc) * 16 * 180 / M_PI;       // ant2 (yz - yc),(xz - xc)
// y downward is positive
  alpha= -alpha;
  beta= -beta;

  *ang = alpha;
  *(ang + 1) = ((beta<0)?(beta+5760):beta) - ((alpha<0)?(alpha+5760):alpha);
  if (*(ang + 1) < 0) *(ang + 1) = 5760 + *(ang + 1);
}

// ---------------------------------------------------------------------
int glpaint()
{
  qDebug() << "glpaint";
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (((Isigraph2 *)isigraph->widget)->epaint) return 1;
  qDebug() << "glpaint begin";
  ((Isigraph2 *)isigraph->widget)->nopaint=true;
  ((Isigraph2 *)isigraph->widget)->repaint(0,0,-1,-1);
  ((Isigraph2 *)isigraph->widget)->nopaint=false;
  qDebug() << "glpaint end";
  return 0;
}

// ---------------------------------------------------------------------
int glpaintx()
{
  qDebug() << "glpaintx";
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)) return 1;
  if (((Isigraph2 *)isigraph->widget)->jpaint) return 1;
  ((Isigraph2 *)isigraph->widget)->update();
  return 0;
}

// ---------------------------------------------------------------------
int glqhandles(void **p)
{
  if (!isigraph) return 1;
  *p = (void *)isigraph;
  return 0;
}

// ---------------------------------------------------------------------
int glqextent(char *s,int *wh)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (((Isigraph2 *)isigraph->widget)->font)->font );
  *(wh) = fm.width( QString::fromUtf8 (s));
  *(wh+1) = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glqextentw(char *s,int *w)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  QStringList n=(QString::fromUtf8 (s)).split("\n",QString::SkipEmptyParts);
  QFontMetrics fm = QFontMetrics ( (((Isigraph2 *)isigraph->widget)->font)->font );
  for (int i=0; i<n.size(); i++) {
    *(w + i) = fm.width ( n.at(i));
  }
  return 0;
}

// ---------------------------------------------------------------------
int glqpixels(const int *p, int *pix)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  const uchar *t = ((Isigraph2 *)isigraph->widget)->pixmap->copy (*(p), *(p + 1), *(p + 2), *(p + 3)).toImage().convertToFormat(QImage::Format_RGB32).bits();
  memcpy((uchar *)pix,t,4*(*(p + 2))*(*(p + 3)));
  return 0;
}

// ---------------------------------------------------------------------
// Height, Ascent, Descent, InternalLeading, ExternalLeading, AverageCharWidth, MaxCharWidth
int glqtextmetrics(int *tm)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (((Isigraph2 *)isigraph->widget)->font)->font );
  *(tm) = fm.height();
  *(tm+1) = fm.ascent();
  *(tm+2) = fm.descent();
  *(tm+3) = fm.leading();
  *(tm+4) = 0;
  *(tm+5) = fm.averageCharWidth();
  *(tm+6) = fm.maxWidth();

  return 0;
}

// ---------------------------------------------------------------------
int glqwh(int *wh)
{
  if (!(wh && isigraph && isigraph->widget)) return 1;
  wh[0] = ((Isigraph2 *)isigraph->widget)->width();
  wh[1] = ((Isigraph2 *)isigraph->widget)->height();
  return 0;
}

// ---------------------------------------------------------------------
int glsel(void *g)
{
  if (!g) return 1;
  Form *f;
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild((Child *)g)) {
//      qDebug() << "glsel ok "+ s2q(f->child->id);
      isigraph = (Isigraph *) g;
      f->child = (Child *) g;
      form = f;
      return 0;
    }
  }
  qDebug() << "glsel failed " + QString::number((SI)g);
  return 0;
}

// ---------------------------------------------------------------------
int glsel2(char *g)
{
  Child *cc;
  if (!g) return 1;
  string p=string(g);
  if (p.size()==0) {
//    qDebug() << "glsel2 empty ok";
    return 0;
  }
  Form *f;
  string q=p;
  if (q[0]=='_') q[0]='-';
  Child *n=(Child *) strtol(q.c_str(),NULL,0);
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild(n)) {
//      qDebug() << "glsel2 ok "+ s2q(f->child->id);
      isigraph = (Isigraph *) n;
      f->child = (Child *) n;
      form=f;
      return 0;
    } else if ((cc=f->id2child(g))) {
//      qDebug() << "glsel2 ok "+ s2q(f->child->id);
      isigraph = (Isigraph *) cc;
      f->child = (Child *) cc;
      form=f;
      return 0;
    }
  }
  qDebug() << "glsel2 failed "+ s2q(string(g));
  return 0;
}

// ---------------------------------------------------------------------
int glarc (const int *p)
{
  int dy[2];
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  qtarcisi (p,  p + 4, dy);
  ((Isigraph2 *)isigraph->widget)->painter->drawArc (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glbrush ()
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->brushcolor = QColor (((Isigraph2 *)isigraph->widget)->color);
  ((Isigraph2 *)isigraph->widget)->brush = QBrush (((Isigraph2 *)isigraph->widget)->brushcolor);
  ((Isigraph2 *)isigraph->widget)->painter->setBrush(((Isigraph2 *)isigraph->widget)->brush);
  ((Isigraph2 *)isigraph->widget)->brushnull = 0;
  return 0;
}

// ---------------------------------------------------------------------
int glbrushnull ()
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->brushnull = 1;
  ((Isigraph2 *)isigraph->widget)->painter->setBrush(Qt::NoBrush);
  return 0;
}

// ---------------------------------------------------------------------
int glcapture (int a)
{
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int glcaret (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  if (0 == *(p) || 0 == *(p + 1))
    return 0;
  QPen pen = ((Isigraph2 *)isigraph->widget)->painter->pen();
  QBrush brush = ((Isigraph2 *)isigraph->widget)->painter->brush();
  ((Isigraph2 *)isigraph->widget)->painter->setPen(QPen (QColor (0, 0, 0),1));
  ((Isigraph2 *)isigraph->widget)->painter->setBrush(QBrush (QColor (0, 0, 0)));
  ((Isigraph2 *)isigraph->widget)->painter->drawRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  ((Isigraph2 *)isigraph->widget)->painter->setPen(pen);
  ((Isigraph2 *)isigraph->widget)->painter->setBrush(brush);
  return 0;
}

// ---------------------------------------------------------------------
int glclear2 (void *p)
{
  Isigraph *isigraph = (Isigraph *) p;
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->color = QColor (0, 0, 0);

// messed up here, but ProFont should be part of config...
  string s=q2s(config.ProFont.family());
  glfont0(isigraph,(char *)s.c_str());

  ((Isigraph2 *)isigraph->widget)->painter->setWorldMatrixEnabled (true);
  ((Isigraph2 *)isigraph->widget)->painter->translate (-((Isigraph2 *)isigraph->widget)->orgx, -((Isigraph2 *)isigraph->widget)->orgy);
  ((Isigraph2 *)isigraph->widget)->orgx = 0;
  ((Isigraph2 *)isigraph->widget)->orgy = 0;
  ((Isigraph2 *)isigraph->widget)->painter->setClipping (false);
  ((Isigraph2 *)isigraph->widget)->clipped = 0;

  ((Isigraph2 *)isigraph->widget)->painter->setPen(QPen (QColor (255, 255, 255),1));
  ((Isigraph2 *)isigraph->widget)->painter->setBrush(QBrush (QColor (255, 255, 255)));
  ((Isigraph2 *)isigraph->widget)->painter->drawRect (0, 0, ((Isigraph2 *)isigraph->widget)->width(), ((Isigraph2 *)isigraph->widget)->height());

  ((Isigraph2 *)isigraph->widget)->pen = QPen (((Isigraph2 *)isigraph->widget)->color, 1); // TODO in user space
  ((Isigraph2 *)isigraph->widget)->textpen = QPen (((Isigraph2 *)isigraph->widget)->pen);
  ((Isigraph2 *)isigraph->widget)->painter->setPen(((Isigraph2 *)isigraph->widget)->pen);
  ((Isigraph2 *)isigraph->widget)->brushcolor = QColor (255, 255, 255);
  ((Isigraph2 *)isigraph->widget)->brush.setColor(((Isigraph2 *)isigraph->widget)->brushcolor);
  ((Isigraph2 *)isigraph->widget)->brushnull = 1;
  ((Isigraph2 *)isigraph->widget)->painter->setBrush(Qt::NoBrush);
  ((Isigraph2 *)isigraph->widget)->textx = 0;
  ((Isigraph2 *)isigraph->widget)->texty = 0;
  ((Isigraph2 *)isigraph->widget)->textcolor = QColor(((Isigraph2 *)isigraph->widget)->color);
  return 0;
}

// ---------------------------------------------------------------------
int glclear ()
{
  if (!isigraph) return 1;
  return glclear2 (isigraph);
}

// ---------------------------------------------------------------------
int glclip (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->clipped = 1;
  ((Isigraph2 *)isigraph->widget)->painter->setClipRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  ((Isigraph2 *)isigraph->widget)->painter->setClipping (true);
  return 0;
}

// ---------------------------------------------------------------------
int glclipreset ()
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  if (((Isigraph2 *)isigraph->widget)->clipped) {
    ((Isigraph2 *)isigraph->widget)->painter->setClipping (false);
    ((Isigraph2 *)isigraph->widget)->clipped = 0;
  }
  return 0;
}

// ---------------------------------------------------------------------
int glcursor (int a)
{
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int glellipse (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->painter->drawEllipse (*(p), *(p + 1),  *(p + 2),  *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
static int glfont_i (const int *p, int len)
{
  char *face = int2utf8 (p, len);
  int r = glfont (face);
  free (face);
  return r;
}

// ---------------------------------------------------------------------
static int glfont0 (Isigraph *isigraph, char *s)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->font = new Font (string(s));
  ((Isigraph2 *)isigraph->widget)->painter->setFont ((((Isigraph2 *)isigraph->widget)->font)->font);
  return 0;
}

// ---------------------------------------------------------------------
int glfont (char *s)
{
  return glfont0 (isigraph, s);
}

// ---------------------------------------------------------------------
int glfont2 (const int *p, int len)
{
  int size10, degree10, bold, italic, strikeout, underline;
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  size10 = *(p);
  bold = 1 & *(p + 1);
  italic = 2 & *(p + 1);
  underline = 4 & *(p + 1);
  strikeout = 8 & *(p + 1);
  degree10 = *(p + 2);
  char *face = int2utf8 (p + 3, len - 3);
  ((Isigraph2 *)isigraph->widget)->font = new Font (string(face), size10, !!bold, !!italic, !!strikeout, !!underline, degree10);
  ((Isigraph2 *)isigraph->widget)->painter->setFont ((((Isigraph2 *)isigraph->widget)->font)->font);
  return 0;
}

// ---------------------------------------------------------------------
int glfontangle (int a)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->font->angle = a;
  return 0;
}

// ---------------------------------------------------------------------
int gllines (const int *p, int len)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  ((Isigraph2 *)isigraph->widget)->painter->drawPolyline (pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int glnodblbuf (int a)
{
// TODO
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int glpen (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->pencolor = QColor (((Isigraph2 *)isigraph->widget)->color);
  ((Isigraph2 *)isigraph->widget)->pen = QPen (((Isigraph2 *)isigraph->widget)->pencolor, Max (0.5, *(p))); // TODO in user space
  ((Isigraph2 *)isigraph->widget)->painter->setPen(((Isigraph2 *)isigraph->widget)->pen);
  return 0;
}

// ---------------------------------------------------------------------
int glpie (const int *p)
{
  int dy[2];
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  qtarcisi (p, p + 4, dy);
  ((Isigraph2 *)isigraph->widget)->painter->drawPie (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glpixel (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->painter->drawPoint (*(p), *(p + 1));
  return 0;
}

// ---------------------------------------------------------------------
static int glpixels2(int x,int y,int w,int h,const uchar *p)
{
  if (w==0||h==0) return 0;
  QImage image = QImage(w,h,QImage::Format_RGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*w*h);

  ((Isigraph2 *)isigraph->widget)->painter->drawImage(x, y, image);

  return 0;
}

// ---------------------------------------------------------------------
int glpixels(const int *p, int len)
{
  Q_UNUSED(len);
  if (!isigraph) return 1;
//  if (!((Isigraph2*)isigraph->widget)->painter->isActive()) return 1;

  return glpixels2 (*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *)(p + 4));
}

// ---------------------------------------------------------------------
int glpixelsx (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
#if defined(_WIN64)||defined(__LP64__)
  Q_UNUSED(p);
  return 1;
#else
  return glpixels2 (*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *) * (p + 4));
#endif
}

// ---------------------------------------------------------------------
int glpolygon (const int *p, int len)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  ((Isigraph2 *)isigraph->widget)->painter->drawPolygon (pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int glrect (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->painter->drawRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int glrgb (const int *p)
{
  if (!isigraph) return 1;
  ((Isigraph2 *)isigraph->widget)->color = QColor (*(p), *(p + 1), *(p + 2));
  return 0;
}

// ---------------------------------------------------------------------
static int gltext_i (const int *p, int len)
{
  char *ys = int2utf8 ((p), len);
  int r = gltext (ys);
  free (ys);
  return r;
}

// ---------------------------------------------------------------------
int gltext (char *ys)
{
  Q_UNUSED(ys);
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->painter->setPen(((Isigraph2 *)isigraph->widget)->textpen);
  if (0 == ((Isigraph2 *)isigraph->widget)->font->angle)
    ((Isigraph2 *)isigraph->widget)->painter->drawText ( ((Isigraph2 *)isigraph->widget)->textx, ((Isigraph2 *)isigraph->widget)->texty, QString::fromUtf8 (ys));
  else {
    ((Isigraph2 *)isigraph->widget)->painter->save ();
    ((Isigraph2 *)isigraph->widget)->painter->translate ( ((Isigraph2 *)isigraph->widget)->textx, ((Isigraph2 *)isigraph->widget)->texty );
    ((Isigraph2 *)isigraph->widget)->painter->rotate ( - ((Isigraph2 *)isigraph->widget)->font->angle/10 );
    ((Isigraph2 *)isigraph->widget)->painter->drawText ( 0, 0, QString::fromUtf8 (ys));
    ((Isigraph2 *)isigraph->widget)->painter->restore ();
  }
  ((Isigraph2 *)isigraph->widget)->painter->setPen(((Isigraph2 *)isigraph->widget)->pen);
  return 0;
}

// ---------------------------------------------------------------------
int gltextcolor ()
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->textcolor = QColor (((Isigraph2 *)isigraph->widget)->color);
  ((Isigraph2 *)isigraph->widget)->textpen = QPen (((Isigraph2 *)isigraph->widget)->painter->pen());
  ((Isigraph2 *)isigraph->widget)->textpen.setColor (((Isigraph2 *)isigraph->widget)->textcolor);
  return 0;
}

// ---------------------------------------------------------------------
int gltextxy (const int *p)
{
  if (!isigraph) return 1;
  ((Isigraph2 *)isigraph->widget)->textx = *(p);
  ((Isigraph2 *)isigraph->widget)->texty = *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int glwindoworg (const int *p)
{
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;
  ((Isigraph2 *)isigraph->widget)->painter->translate (*(p), *(p + 1));
  ((Isigraph2 *)isigraph->widget)->orgx += *(p);
  ((Isigraph2 *)isigraph->widget)->orgy += *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int
glcmds (const int *ptr, int ncnt)
{
  int cnt;
  int p = 0;
  int errcnt = 0;

//  if (!form) return 1;
  if (!isigraph) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter) return 1;
  if (!((Isigraph2 *)isigraph->widget)->painter->isActive()) return 1;

  while (p < ncnt) {
    cnt = *(ptr + p);
    switch (*(ptr + p + 1)) {

    case 2001:		// glarc
      glarc (ptr + p + 2);
      break;

    case 2004:		// glbrush
      glbrush ();
      break;

    case 2005:		// glbrushnull
      glbrushnull ();
      break;

    case 2062:		//glcapture
      glcapture (*(ptr + p + 2));
      break;

    case 2065:		//glcaret
      glcaret (ptr + p + 2);
      break;

    case 2007:		//glclear
      glclear ();
      break;

    case 2078:		//glclip
      glclip (ptr + p + 2);
      break;

    case 2079:		//glclipreset
      glclipreset ();
      break;

    case 2069:		//glcursor
      glcursor (*(ptr + p + 2));
      break;

    case 2008:		// glellipse
      glellipse (ptr + p + 2);
      break;

    case 2012:		// glfont
      glfont_i ((ptr + p + 2), cnt-2);
      break;

    case 2312:		// glfont2
      glfont2 ((ptr + p + 2), cnt-2);
      break;

    case 2342:		//glfontangle
      glfontangle (*(ptr + p + 2));
      break;

    case 2015:		// gllines
      gllines ((ptr + p + 2), cnt-2);
      break;

    case 2070:		// glnodblbuf
      glnodblbuf (*(ptr + p + 2));
      break;

    case 2022:		// glpen
      glpen (ptr + p + 2);
      break;

    case 2023:		// glpie
      glpie (ptr + p + 2);
      break;

    case 2024:		// glpixel
      glpixel (ptr + p + 2);
      break;

    case 2076:		// glpixels
      glpixels ((ptr + p + 2), cnt-2);
      break;

    case 2075:		// glpixelsx
      glpixelsx (ptr + p + 2);
      break;

    case 2029:		// glpolygon
      glpolygon ((ptr + p + 2), cnt-2);
      break;

    case 2031:		// glrect
      glrect (ptr + p + 2);
      break;

    case 2032:		// glrgb
      glrgb (ptr + p + 2);
      break;

    case 2038:		// gltext
      gltext_i (ptr + p + 2, cnt-2);
      break;

    case 2040:		// gltextcolor
      gltextcolor ();
      break;

    case 2056:		// gltextxy
      gltextxy (ptr + p + 2);
      break;

    case 2045:		//glwindoworg
      glwindoworg (ptr + p + 2);
      break;

    default:
      ++errcnt;
      break;
    }
    p = p + cnt;
  }
  return errcnt;
}

// ---------------------------------------------------------------------
int glsetlocale (char *c)
{
  if (!isigraph) return 1;
  isigraph->locale = string(c);
  return 0;
}

