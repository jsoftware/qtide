#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES // for C++ DM 130130
#include <cmath> // for M_PI  DM 130130
#endif

#ifndef Max
#define Max(a, b) ((a) < (b) ? (b) : (a))
#endif

#include "wd.h"
#include "font.h"
#include "ogl2.h"
#include "opengl.h"
#include "opengl2.h"
#include "form.h"
#include "../base/state.h"
extern QList<Form *>Forms;

static int gl_font0 (Opengl *opengl, char *s);

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
int gl_paint()
{
  return 0;
//  qDebug() << "gl_paint";
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w) return 1;
  if (w->epaint) return 1;
//  qDebug() << "gl_paint begin";
  w->nopaint=true;
  w->paintgl();
  w->nopaint=false;
//  qDebug() << "gl_paint end";
  return 0;
}

// ---------------------------------------------------------------------
int gl_paintx()
{
//  qDebug() << "gl_paintx";
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w) return 1;
  if (w->jpaint) return 1;
  w->updateGL();
  return 0;
}

// ---------------------------------------------------------------------
int gl_qhandles(void **p)
{
  if (!p) return 1;
  if (!opengl) return 1;
  *p = (void *)opengl;
#ifdef _WIN32
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (w) *(p+1) = (void *)w->getDC();
  else *(p+1) = (void *)0;
#else
  *(p+1) = (void *)0;
#endif
  return 0;
}

// ---------------------------------------------------------------------
int gl_qextent(char *s,int *wh)
{
  if (!s || !wh) return 1;
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (w->font)->font );
  *(wh) = fm.width( QString::fromUtf8 (s));
  *(wh+1) = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int gl_qextentw(char *s,int *wi)
{
  if (!s || !wi) return 1;
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  QStringList n=(QString::fromUtf8 (s)).split("\n",QString::KeepEmptyParts);
  QFontMetrics fm = QFontMetrics ( (w->font)->font );
  for (int i=0; i<n.size(); i++) {
    wi[i] = fm.width ( n.at(i));
  }
  return 0;
}

// ---------------------------------------------------------------------
int gl_qpixels(const int *p, int *pix)
{
  if (!p || !pix) return 1;
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  QPixmap *pm = w->pixmap;
  if (!pm || pm->isNull()) return 1;
  if (p[0]<0 || p[1]<0 || (p[0]+p[2])>pm->width() || (p[1]+p[3])>pm->height()) return 1;
// one-liner cause does not work ??
//  const uchar *t = (pm->copy(p[0], p[1], p[2], p[3])).toImage().convertToFormat(QImage::Format_RGB32).bits();
  QPixmap a1 = (pm->copy(p[0], p[1], p[2], p[3]));
  if (a1.isNull()) return 1;
  QImage a2 = a1.toImage();
  if (a2.isNull()) return 1;
  QImage a3 = a2.convertToFormat(QImage::Format_RGB32);
  if (a3.isNull()) return 1;
  const uchar *t = a3.bits();
  if (!t) return 1;
  memcpy((uchar *)pix,t,4*p[2]*p[3]);
  return 0;
}

// ---------------------------------------------------------------------
// Height, Ascent, Descent, InternalLeading, ExternalLeading, AverageCharWidth, MaxCharWidth
int gl_qtextmetrics(int *tm)
{
  if (!tm) return 1;
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (w->font)->font );
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
int gl_qwh(int *wh)
{
  if (!(wh && opengl && opengl->widget)) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  wh[0] = w->width();
  wh[1] = w->height();
  return 0;
}

// ---------------------------------------------------------------------
int gl_sel(void *g)
{
  if (!g) return 1;
  Form *f;
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild((Child *)g)) {
      if ((((Child *)g)->type == "opengl") && ((Child *)g)->widget) {
        opengl = (Opengl *) g;
        f->child = (Child *) g;
        form = f;
        return 0;
      }
    }
  }
  qDebug() << "gl_sel failed " + QString::number((SI)g);
  return 1;
}

// ---------------------------------------------------------------------
int gl_sel2(char *g)
{
  Child *cc;
  if (!g) return 1;
  string p=string(g);
  if (p.size()==0) {
    return 0;
  }
  if ((p[0]=='_')||(p[0]=='-')||(p[0]=='0')||(p[0]=='1')||(p[0]=='2')||(p[0]=='3')||(p[0]=='4')||(p[0]=='5')||(p[0]=='6')||(p[0]=='7')||(p[0]=='8')||(p[0]=='9')) {
    return gl_sel((void *) c_strtol(p));
  }
  Form *f;
// search current form first
  if (form) {
    f=form;
    if ((cc=f->id2child(g))) {
      if ((cc->type == "opengl") && (cc->widget)) {
        opengl = (Opengl *) cc;
        f->child = cc;
        form=f;
        return 0;
      }
    }
  }
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if ((cc=f->id2child(g))) {
      if ((cc->type == "opengl") && (cc->widget)) {
        opengl = (Opengl *) cc;
        f->child = cc;
        form=f;
        return 0;
      }
    }
  }
  qDebug() << "gl_sel2 failed "+ s2q(string(g));
  return 1;
}

// ---------------------------------------------------------------------
int gl_arc (const int *p)
{
  int dy[2];
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  qtarcisi (p,  p + 4, dy);
  w->painter->drawArc (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int gl_brush ()
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->brushcolor = QColor (w->color);
  w->brush = QBrush (w->brushcolor);
  w->painter->setBrush(w->brush);
  w->brushnull = 0;
  return 0;
}

// ---------------------------------------------------------------------
int gl_brushnull ()
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->brushnull = 1;
  w->painter->setBrush(Qt::NoBrush);
  return 0;
}

// ---------------------------------------------------------------------
int gl_capture (int a)
{
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int gl_caret (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  if (0 == *(p) || 0 == *(p + 1))
    return 0;
  QPen pen = w->painter->pen();
  QBrush brush = w->painter->brush();
  w->painter->setPen(QPen (QColor (0, 0, 0),1));
  w->painter->setBrush(QBrush (QColor (0, 0, 0)));
  w->painter->drawRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  w->painter->setPen(pen);
  w->painter->setBrush(brush);
  return 0;
}

// ---------------------------------------------------------------------
int gl_clear2 (void *p)
{
  Opengl *opengl = (Opengl *) p;
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->color = QColor (0, 0, 0);

// messed up here, but ProFont should be part of config...
  string s=q2s(config.ProFont.family());
  gl_font0(opengl,(char *)s.c_str());

  w->painter->setWorldMatrixEnabled (true);
  w->painter->translate (-w->orgx, -w->orgy);
  w->orgx = 0;
  w->orgy = 0;
  w->painter->setClipping (false);
  w->clipped = 0;

  w->painter->setPen(QPen (QColor (255, 255, 255),1));
  w->painter->setBrush(QBrush (QColor (255, 255, 255)));
// prevent erase opengl
//  w->painter->drawRect (0, 0, w->width(), w->height());

  w->pen = QPen (w->color, 1); // TODO in user space
  w->textpen = QPen (w->pen);
  w->painter->setPen(w->pen);
  w->brushcolor = QColor (255, 255, 255);
  w->brush.setColor(w->brushcolor);
  w->brushnull = 1;
  w->painter->setBrush(Qt::NoBrush);
  w->textx = 0;
  w->texty = 0;
  w->textcolor = QColor(w->color);
  return 0;
}

// ---------------------------------------------------------------------
int gl_clear ()
{
  if (!opengl) return 1;
  return gl_clear2 (opengl);
}

// ---------------------------------------------------------------------
int gl_clip (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->clipped = 1;
  w->painter->setClipRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  w->painter->setClipping (true);
  return 0;
}

// ---------------------------------------------------------------------
int gl_clipreset ()
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  if (w->clipped) {
    w->painter->setClipping (false);
    w->clipped = 0;
  }
  return 0;
}

// ---------------------------------------------------------------------
int gl_cursor (int a)
{
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int gl_ellipse (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->painter->drawEllipse (*(p), *(p + 1),  *(p + 2),  *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
static int gl_font_i (const int *p, int len)
{
  char *face = int2utf8 (p, len);
  int r = gl_font (face);
  free (face);
  return r;
}

// ---------------------------------------------------------------------
static int gl_font0 (Opengl *opengl, char *s)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->font = new Font (string(s));
  w->painter->setFont ((w->font)->font);
  QFontMetrics fm = QFontMetrics ( (w->font)->font );
  w->fontheight = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int gl_font (char *s)
{
  return gl_font0 (opengl, s);
}

// ---------------------------------------------------------------------
int gl_font2 (const int *p, int len)
{
  int size10, degree10, bold, italic, strikeout, underline;
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  size10 = *(p);
  bold = 1 & *(p + 1);
  italic = 2 & *(p + 1);
  underline = 4 & *(p + 1);
  strikeout = 8 & *(p + 1);
  degree10 = *(p + 2);
  char *face = int2utf8 (p + 3, len - 3);
  w->font = new Font (string(face), size10, !!bold, !!italic, !!strikeout, !!underline, degree10);
  w->painter->setFont ((w->font)->font);
  QFontMetrics fm = QFontMetrics ( (w->font)->font );
  w->fontheight = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int gl_fontangle (int a)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->font->angle = a;
  return 0;
}

// ---------------------------------------------------------------------
int gl_lines (const int *p, int len)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  w->painter->drawPolyline (pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int gl_nodblbuf (int a)
{
// TODO
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int gl_pen (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->pencolor = QColor (w->color);
  w->pen = QPen (w->pencolor, Max (0.5, *(p))); // TODO in user space
  w->painter->setPen(w->pen);
  return 0;
}

// ---------------------------------------------------------------------
int gl_pie (const int *p)
{
  int dy[2];
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  qtarcisi (p, p + 4, dy);
  w->painter->drawPie (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int gl_pixel (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->painter->drawPoint (*(p), *(p + 1));
  return 0;
}

// ---------------------------------------------------------------------
static int glpixels2(int x,int y,int wi,int h,const uchar *p)
{
  if (!wi || !h || !p) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  QImage image = QImage(wi,h,QImage::Format_RGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*wi*h);

  w->painter->drawImage(x, y, image);

  return 0;
}

// ---------------------------------------------------------------------
int gl_pixels(const int *p, int len)
{
  Q_UNUSED(len);
  if (!opengl) return 1;
//  if (!((Opengl2*)opengl->widget)->painter->isActive()) return 1;

  return glpixels2 (*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *)(p + 4));
}

// ---------------------------------------------------------------------
int gl_pixelsx (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
#if defined(_WIN64)||defined(__LP64__)
  Q_UNUSED(p);
  return 1;
#else
  return glpixels2 (*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *) * (p + 4));
#endif
}

// ---------------------------------------------------------------------
int gl_polygon (const int *p, int len)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  w->painter->drawPolygon (pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int gl_rect (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->painter->drawRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int gl_rgb (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  w->color = QColor (*(p), *(p + 1), *(p + 2));
  return 0;
}

// ---------------------------------------------------------------------
static int gl_text_i (const int *p, int len)
{
  char *ys = int2utf8 ((p), len);
  int r = gl_text (ys);
  free (ys);
  return r;
}

// ---------------------------------------------------------------------
int gl_text (char *ys)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  if (!w->font) return 1;
  QFontMetrics fm = QFontMetrics ( (w->font)->font );
  QString qs = QString::fromUtf8 (ys);

  w->painter->setPen(w->textpen);
  if ((!w->font) || (0 == w->font->angle))
    w->painter->drawText ( w->textx, w->texty + fm.ascent(), qs);
  else {
    w->painter->save ();
    w->painter->translate ( w->textx, w->texty );
    w->painter->rotate ( - w->font->angle/10 );
    w->painter->drawText(0, fm.ascent(), qs);
    w->painter->restore ();
  }
  w->painter->setPen(w->pen);
  return 0;
}

// ---------------------------------------------------------------------
int gl_textcolor ()
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->textcolor = QColor (w->color);
  w->textpen = QPen (w->painter->pen());
  w->textpen.setColor (w->textcolor);
  return 0;
}

// ---------------------------------------------------------------------
int gl_textxy (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  w->textx = *(p);
  w->texty = *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int gl_windoworg (const int *p)
{
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;
  w->painter->translate (*(p), *(p + 1));
  w->orgx += *(p);
  w->orgy += *(p + 1);
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
  if (!opengl) return 1;
  Opengl2 *w = (Opengl2 *)opengl->widget;
  if (!w->painter) return 1;
  if (!w->painter->isActive()) return 1;

  while (p < ncnt) {
    cnt = *(ptr + p);
    switch (*(ptr + p + 1)) {

    case 2001:		// gl_arc
      gl_arc (ptr + p + 2);
      break;

    case 2004:		// gl_brush
      gl_brush ();
      break;

    case 2005:		// gl_brushnull
      gl_brushnull ();
      break;

    case 2062:		//gl_capture
      gl_capture (*(ptr + p + 2));
      break;

    case 2065:		//gl_caret
      gl_caret (ptr + p + 2);
      break;

    case 2007:		//gl_clear
      gl_clear ();
      break;

    case 2078:		//gl_clip
      gl_clip (ptr + p + 2);
      break;

    case 2079:		//gl_clipreset
      gl_clipreset ();
      break;

    case 2069:		//gl_cursor
      gl_cursor (*(ptr + p + 2));
      break;

    case 2008:		// gl_ellipse
      gl_ellipse (ptr + p + 2);
      break;

    case 2012:		// gl_font
      gl_font_i ((ptr + p + 2), cnt-2);
      break;

    case 2312:		// gl_font2
      gl_font2 ((ptr + p + 2), cnt-2);
      break;

    case 2342:		//gl_fontangle
      gl_fontangle (*(ptr + p + 2));
      break;

    case 2015:		// gl_lines
      gl_lines ((ptr + p + 2), cnt-2);
      break;

    case 2070:		// gl_nodblbuf
      gl_nodblbuf (*(ptr + p + 2));
      break;

    case 2022:		// gl_pen
      gl_pen (ptr + p + 2);
      break;

    case 2023:		// gl_pie
      gl_pie (ptr + p + 2);
      break;

    case 2024:		// gl_pixel
      gl_pixel (ptr + p + 2);
      break;

    case 2076:		// gl_pixels
      gl_pixels ((ptr + p + 2), cnt-2);
      break;

    case 2075:		// gl_pixelsx
      gl_pixelsx (ptr + p + 2);
      break;

    case 2029:		// gl_polygon
      gl_polygon ((ptr + p + 2), cnt-2);
      break;

    case 2031:		// gl_rect
      gl_rect (ptr + p + 2);
      break;

    case 2032:		// gl_rgb
      gl_rgb (ptr + p + 2);
      break;

    case 2038:		// gl_text
      gl_text_i (ptr + p + 2, cnt-2);
      break;

    case 2040:		// gl_textcolor
      gl_textcolor ();
      break;

    case 2056:		// gl_textxy
      gl_textxy (ptr + p + 2);
      break;

    case 2045:		//gl_windoworg
      gl_windoworg (ptr + p + 2);
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
int gl_setlocale (char *c)
{
  if (!opengl) return 1;
  opengl->locale = string(c);
  return 0;
}

// ---------------------------------------------------------------------
int gl_updategl(void *g)
{
  if (!g) return 1;
  Form *f;
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild((Child *)g)) {
      if ((((Child *)g)->type == "opengl") && ((Child *)g)->widget) {
//      qDebug() << "gl_updategl ok "+ s2q(f->child->id);
        ((Opengl2 *)(((Opengl *) g)->widget))->updateGL();
        return 0;
      }
    }
  }
  qDebug() << "gl_updategl failed " + QString::number((SI)g);
  return 1;
}
