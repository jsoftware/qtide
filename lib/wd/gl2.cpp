
#ifdef _MSC_VER
#define _USE_MATH_DEFINES // for C++ DM 130130
#include <cmath> // for M_PI  DM 130130
#endif

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
#include "isidraw.h"
#include "form.h"
#include "../base/state.h"

extern QList<Form *>Forms;

static int glfont0(Child *isigraph, char *s);

// ---------------------------------------------------------------------
// caller should free string
static char *
int2utf8(const int *yi, int nc)
{
  int i;
  char *ys = (char *) malloc(1 + (size_t) nc);
  for (i = 0; i < nc; i++)
    *(ys + i) = *(yi + i);
  *(ys + nc) = 0;
  return ys;
}

static void
qtarcisi(const int *y, const int *y2, int *ang)
{
// center
  int xc = *y + 0.5 * *(y + 2);
  int yc = *(y + 1) + 0.5 * *(y + 3);
// 0 is 3-o'clock, and counter-clockwise is positive, so that no need to adjust angle
  int alpha = atan2((float)*(y2 + 1) - yc, (float)*(y2) - xc) * 16 * 180 / M_PI;           // ant2 (ya - yc),(xa - xc)
  int beta = atan2((float)*(y2 + 3) - yc, (float)*(y2 + 2) - xc) * 16 * 180 / M_PI;        // ant2 (yz - yc),(xz - xc)
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
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (w->type=="isigraph" && w->painter) return 1;
  w->update();
  return 0;
}

// ---------------------------------------------------------------------
int glpaintx()
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (w->type=="isigraph") return 1;
  w->repaint();
  return 0;
}

// ---------------------------------------------------------------------
int glqhandles(void **p)
{
  if (!p) return 1;
  if (!isigraph) return 1;
  *p = (void *)isigraph;
#ifdef _WIN32
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
#ifndef QT50
  if (w) *(p+1) = (void *)w->getDC();
#else
  if (w) *(p+1) = (void *)0;
#endif
  else *(p+1) = (void *)0;
#else
  *(p+1) = (void *)0;
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glqextent(char *s,int *wh)
{
  if (!s || !wh) return 1;
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  QFontMetrics fm = QFontMetrics((w->font)->font);
  *(wh) = fm.width(QString::fromUtf8(s));
  *(wh+1) = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glqextentw(char *s,int *wi)
{
  if (!s || !wi) return 1;
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  QStringList n=(QString::fromUtf8(s)).split("\n",QString::KeepEmptyParts);
  QFontMetrics fm = QFontMetrics((w->font)->font);
  for (int i=0; i<n.size(); i++) {
    wi[i] = fm.width(n.at(i));
  }
  return 0;
}

// ---------------------------------------------------------------------
int glqpixels(const int *p, int *pix)
{
  if (!p || !pix) return 1;
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  QPixmap pm=w->getpixmap();
  if (!pm || pm.isNull()) return 1;
  if (p[0]<0 || p[1]<0 || (p[0]+p[2])>pm.width() || (p[1]+p[3])>pm.height()) return 1;
  QPixmap a1 = (pm.copy(p[0], p[1], p[2], p[3]));
  if (a1.isNull()) return 1;
  QImage a2 = a1.toImage();
  if (a2.isNull()) return 1;
  QImage a3 = a2.convertToFormat(QImage::Format_ARGB32);
  if (a3.isNull()) return 1;
  const uchar *t = a3.bits();
  if (!t) return 1;
  memcpy((uchar *)pix,t,4*p[2]*p[3]);
  return 0;
}

// ---------------------------------------------------------------------
// Height, Ascent, Descent, InternalLeading, ExternalLeading, AverageCharWidth, MaxCharWidth
int glqtextmetrics(int *tm)
{
  if (!tm) return 1;
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  QFontMetrics fm = QFontMetrics((w->font)->font);
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
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  wh[0] = w->width();
  wh[1] = w->height();
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
      if (((((Child *)g)->type == "isigraph")||(((Child *)g)->type == "isidraw")) && ((Child *)g)->widget) {
        isigraph = (Isigraph *) g;
        f->child = (Child *) g;
        form = f;
        return 0;
      }
    }
  }
  qDebug() << "glsel failed " + QString::number((SI)g);
  return 1;
}

// ---------------------------------------------------------------------
int glsel2(char *g)
{
  Child *cc;
  if (!g) return 1;
  string p=string(g);
  if (p.size()==0) {
    return 0;
  }
  if ((p[0]=='_')||(p[0]=='-')||(p[0]=='0')||(p[0]=='1')||(p[0]=='2')||(p[0]=='3')||(p[0]=='4')||(p[0]=='5')||(p[0]=='6')||(p[0]=='7')||(p[0]=='8')||(p[0]=='9')) {
    return glsel((void *) c_strtol(p));
  }
  Form *f;
// search current form first
  if (form) {
    f=form;
    if ((cc=f->id2child(g))) {
      if (((cc->type == "isigraph")||(cc->type == "isidraw")) && (cc->widget)) {
        isigraph = (Isigraph *) cc;
        f->child = cc;
        form=f;
        return 0;
      }
    }
  }
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if ((cc=f->id2child(g))) {
      if (((cc->type == "isigraph")||(cc->type == "isidraw")) && (cc->widget)) {
        isigraph = (Isigraph *) cc;
        f->child = cc;
        form=f;
        return 0;
      }
    }
  }
  qDebug() << "glsel2 failed "+ s2q(string(g));
  return 1;
}

// ---------------------------------------------------------------------
int glarc(const int *p)
{
  int dy[2];
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  qtarcisi(p,  p + 4, dy);
  w->painter->drawArc(*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glbrush()
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->brushcolor = QColor(w->color);
  w->brush = QBrush(w->brushcolor);
  w->painter->setBrush(w->brush);
  w->brushnull = 0;
  return 0;
}

// ---------------------------------------------------------------------
int glbrushnull()
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->brushnull = 1;
  w->painter->setBrush(Qt::NoBrush);
  return 0;
}

// ---------------------------------------------------------------------
int glcapture(int a)
{
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int glcaret(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  if (0 == *(p) || 0 == *(p + 1))
    return 0;
  QPen pen = w->painter->pen();
  QBrush brush = w->painter->brush();
  w->painter->setPen(QPen(QColor(0, 0, 0),1));
  w->painter->setBrush(QBrush(QColor(0, 0, 0)));
  w->painter->drawRect(*(p), *(p + 1), *(p + 2), *(p + 3));
  w->painter->setPen(pen);
  w->painter->setBrush(brush);
  return 0;
}

// ---------------------------------------------------------------------
int glclear2(void *p)
{
  Isigraph *isigraph = (Isigraph *) p;
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->color = QColor(0, 0, 0);

  string s=q2s(config.ProFont.family());
  glfont0(isigraph,(char *)s.c_str());

  w->painter->translate(-w->orgx, -w->orgy);
  w->orgx = 0;
  w->orgy = 0;
  w->clipped = 0;

  w->painter->setPen(QPen(QColor(255, 255, 255),1));
  w->painter->setBrush(QBrush(QColor(255, 255, 255)));
  w->painter->drawRect(0, 0, w->width(), w->height());

  w->pen = QPen(w->color, 1);  // TODO in user space
  w->textpen = QPen(w->pen);
  w->painter->setPen(w->pen);
  w->brushcolor = QColor(255, 255, 255);
  w->brush.setColor(w->brushcolor);
  w->brushnull = 1;
  w->painter->setBrush(Qt::NoBrush);
  w->textx = 0;
  w->texty = 0;
  w->textcolor = QColor(w->color);
  return 0;
}

// ---------------------------------------------------------------------
int glclear()
{
  if (!isigraph) return 1;
  return glclear2(isigraph);
}

// ---------------------------------------------------------------------
int glclip(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->clipped = 1;
  w->painter->setClipRect(*(p), *(p + 1), *(p + 2), *(p + 3));
  w->painter->setClipping(true);
  return 0;
}

// ---------------------------------------------------------------------
int glclipreset()
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  if (w->clipped) {
    w->painter->setClipping(false);
    w->clipped = 0;
  }
  return 0;
}

// ---------------------------------------------------------------------
int glcursor(int a)
{
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int glellipse(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->painter->drawEllipse(*(p), *(p + 1),  *(p + 2),  *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int glfill(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->fill(p);
  return 0;
}

// ---------------------------------------------------------------------
static int glfont_i(const int *p, int len)
{
  char *face = int2utf8(p, len);
  int r = glfont(face);
  free(face);
  return r;
}

// ---------------------------------------------------------------------
static int glfont0(Child *isigraph, char *s)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->font = new Font(string(s));
  w->painter->setFont((w->font)->font);
  QFontMetrics fm = QFontMetrics((w->font)->font);
  w->fontheight = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glfont(char *s)
{
  return glfont0(isigraph, s);
}

// ---------------------------------------------------------------------
int glfont2(const int *p, int len)
{
  int size10, degree10, bold, italic, strikeout, underline;
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  size10 = *(p);
  bold = 1 & *(p + 1);
  italic = 2 & *(p + 1);
  underline = 4 & *(p + 1);
  strikeout = 8 & *(p + 1);
  degree10 = *(p + 2);
  char *face = int2utf8(p + 3, len - 3);
  w->font = new Font(string(face), size10, !!bold, !!italic, !!strikeout, !!underline, degree10);
  w->painter->setFont((w->font)->font);
  QFontMetrics fm = QFontMetrics((w->font)->font);
  w->fontheight = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glfontangle(int a)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->font->angle = a;
  return 0;
}

// ---------------------------------------------------------------------
int gllines(const int *p, int len)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint(*(p + 2 * i), *(p + 1 + 2 * i));
  w->painter->drawPolyline(pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int glnodblbuf(int a)
{
// TODO
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int glpen(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->pencolor = QColor(w->color);
  w->pen = QPen(w->pencolor, Max(0.5, *(p)));   // TODO in user space
  w->painter->setPen(w->pen);
  return 0;
}

// ---------------------------------------------------------------------
int glpie(const int *p)
{
  int dy[2];
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  qtarcisi(p, p + 4, dy);
  w->painter->drawPie(*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glpixel(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->painter->drawPoint(*(p), *(p + 1));
  return 0;
}

// ---------------------------------------------------------------------
static int glpixels2(int x,int y,int wi,int h,const uchar *p)
{
  if (!wi || !h || !p) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  QImage image = QImage(wi,h,QImage::Format_ARGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*wi*h);
  w->painter->drawImage(x, y, image);
  return 0;
}

// ---------------------------------------------------------------------
int glpixels(const int *p, int len)
{
  Q_UNUSED(len);
  if (!isigraph) return 1;
  if (!((Isigraph2*)isigraph->widget)->painter) return 1;
  return glpixels2(*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *)(p + 4));
}

// ---------------------------------------------------------------------
int glpixelsx(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
#if defined(_WIN64)||defined(__LP64__)
  Q_UNUSED(p);
  return 1;
#else
  return glpixels2(*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *) * (p + 4));
#endif
}

// ---------------------------------------------------------------------
int glpolygon(const int *p, int len)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint(*(p + 2 * i), *(p + 1 + 2 * i));
  w->painter->drawPolygon(pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int glrect(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->painter->drawRect(*(p), *(p + 1), *(p + 2), *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int glrgb(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  w->color = QColor(*(p), *(p + 1), *(p + 2));
  return 0;
}

// ---------------------------------------------------------------------
int glrgba(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  w->color = QColor(*(p), *(p + 1), *(p + 2), *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
static int gltext_i(const int *p, int len)
{
  char *ys = int2utf8((p), len);
  int r = gltext(ys);
  free(ys);
  return r;
}

// ---------------------------------------------------------------------
int gltext(char *ys)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  if (!w->font) return 1;
  QFontMetrics fm = QFontMetrics((w->font)->font);
  QString qs = QString::fromUtf8(ys);

  w->painter->setPen(w->textpen);
  if (0 == w->font->angle)
    w->painter->drawText(w->textx, w->texty + fm.ascent(), qs);
  else {
    w->painter->save();
    w->painter->translate(w->textx, w->texty);
    w->painter->rotate(- w->font->angle/10);
    w->painter->drawText(0, fm.ascent(), qs);
    w->painter->restore();
  }
  w->painter->setPen(w->pen);
  return 0;
}

// ---------------------------------------------------------------------
int gltextcolor()
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->textcolor = QColor(w->color);
  w->textpen = QPen(w->painter->pen());
  w->textpen.setColor(w->textcolor);
  return 0;
}

// ---------------------------------------------------------------------
int gltextxy(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  w->textx = *(p);
  w->texty = *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int glwindoworg(const int *p)
{
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;
  w->painter->translate(*(p), *(p + 1));
  w->orgx += *(p);
  w->orgy += *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int
glcmds(const int *ptr, int ncnt)
{
  int cnt;
  int p = 0;
  int errcnt = 0;

//  if (!form) return 1;
  if (!isigraph) return 1;
  Isigraph2 *w = (Isigraph2 *)isigraph->widget;
  if (!w->painter) return 1;

  while (p < ncnt) {
    cnt = *(ptr + p);
    switch (*(ptr + p + 1)) {

    case 2001:		// glarc
      glarc(ptr + p + 2);
      break;

    case 2004:		// glbrush
      glbrush();
      break;

    case 2005:		// glbrushnull
      glbrushnull();
      break;

    case 2062:		//glcapture
      glcapture(*(ptr + p + 2));
      break;

    case 2065:		//glcaret
      glcaret(ptr + p + 2);
      break;

    case 2007:		//glclear
      glclear();
      break;

    case 2078:		//glclip
      glclip(ptr + p + 2);
      break;

    case 2079:		//glclipreset
      glclipreset();
      break;

    case 2069:		//glcursor
      glcursor(*(ptr + p + 2));
      break;

    case 2008:		// glellipse
      glellipse(ptr + p + 2);
      break;

    case 2012:		// glfont
      glfont_i((ptr + p + 2), cnt-2);
      break;

    case 2312:		// glfont2
      glfont2((ptr + p + 2), cnt-2);
      break;

    case 2342:		// glfontangle
      glfontangle(*(ptr + p + 2));
      break;

    case 2015:		// gllines
      gllines((ptr + p + 2), cnt-2);
      break;

    case 2070:		// glnodblbuf
      glnodblbuf(*(ptr + p + 2));
      break;

    case 2022:		// glpen
      glpen(ptr + p + 2);
      break;

    case 2023:		// glpie
      glpie(ptr + p + 2);
      break;

    case 2024:		// glpixel
      glpixel(ptr + p + 2);
      break;

    case 2076:		// glpixels
      glpixels((ptr + p + 2), cnt-2);
      break;

    case 2075:		// glpixelsx
      glpixelsx(ptr + p + 2);
      break;

    case 2029:		// glpolygon
      glpolygon((ptr + p + 2), cnt-2);
      break;

    case 2031:		// glrect
      glrect(ptr + p + 2);
      break;

    case 2032:		// glrgb
      glrgb(ptr + p + 2);
      break;

    case 2343:		// glrgba
      glrgba(ptr + p + 2);
      break;

    case 2038:		// gltext
      gltext_i(ptr + p + 2, cnt-2);
      break;

    case 2040:		// gltextcolor
      gltextcolor();
      break;

    case 2056:		// gltextxy
      gltextxy(ptr + p + 2);
      break;

    case 2045:		//glwindoworg
      glwindoworg(ptr + p + 2);
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
int glsetlocale(char *c)
{
  if (!isigraph) return 1;
  isigraph->locale = string(c);
  return 0;
}

