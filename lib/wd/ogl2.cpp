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
  if (!((Opengl2 *)opengl->widget)) return 1;
  if (((Opengl2 *)opengl->widget)->epaint) return 1;
//  qDebug() << "gl_paint begin";
  ((Opengl2 *)opengl->widget)->nopaint=true;
  ((Opengl2 *)opengl->widget)->paintgl();
  ((Opengl2 *)opengl->widget)->nopaint=false;
//  qDebug() << "gl_paint end";
  return 0;
}

// ---------------------------------------------------------------------
int gl_paintx()
{
//  qDebug() << "gl_paintx";
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)) return 1;
  if (((Opengl2 *)opengl->widget)->jpaint) return 1;
  ((Opengl2 *)opengl->widget)->updateGL();
  return 0;
}

// ---------------------------------------------------------------------
int gl_qhandles(void **p)
{
  if (!opengl) return 1;
  *p = (void *)opengl;
  return 0;
}

// ---------------------------------------------------------------------
int gl_qextent(char *s,int *wh)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (((Opengl2 *)opengl->widget)->font)->font );
  *(wh) = fm.width( QString::fromUtf8 (s));
  *(wh+1) = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int gl_qextentw(char *s,int *w)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  QStringList n=(QString::fromUtf8 (s)).split("\n",QString::SkipEmptyParts);
  QFontMetrics fm = QFontMetrics ( (((Opengl2 *)opengl->widget)->font)->font );
  for (int i=0; i<n.size(); i++) {
    *(w + i) = fm.width ( n.at(i));
  }
  return 0;
}

// ---------------------------------------------------------------------
int gl_qpixels(const int *p, int *pix)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  const uchar *t = ((Opengl2 *)opengl->widget)->pixmap->copy (*(p), *(p + 1), *(p + 2), *(p + 3)).toImage().convertToFormat(QImage::Format_RGB32).bits();
  memcpy((uchar *)pix,t,4*(*(p + 2))*(*(p + 3)));
  return 0;
}

// ---------------------------------------------------------------------
// Height, Ascent, Descent, InternalLeading, ExternalLeading, AverageCharWidth, MaxCharWidth
int gl_qtextmetrics(int *tm)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (((Opengl2 *)opengl->widget)->font)->font );
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
  wh[0] = ((Opengl2 *)opengl->widget)->width();
  wh[1] = ((Opengl2 *)opengl->widget)->height();
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
//      qDebug() << "glsel ok "+ s2q(f->child->id);
        opengl = (Opengl *) g;
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
int gl_sel2(char *g)
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
      if ((n->type == "opengl") && n->widget) {
//      qDebug() << "glsel2 ok "+ s2q(f->child->id);
        opengl = (Opengl *) n;
        f->child = (Child *) n;
        form=f;
        return 0;
      } else if ((cc=f->id2child(g))) {
//      qDebug() << "glsel2 ok "+ s2q(f->child->id);
        opengl = (Opengl *) cc;
        f->child = (Child *) cc;
        form=f;
        return 0;
      }
    }
  }
  qDebug() << "glsel2 failed "+ s2q(string(g));
  return 1;
}

// ---------------------------------------------------------------------
int gl_arc (const int *p)
{
  int dy[2];
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  qtarcisi (p,  p + 4, dy);
  ((Opengl2 *)opengl->widget)->painter->drawArc (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int gl_brush ()
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->brushcolor = QColor (((Opengl2 *)opengl->widget)->color);
  ((Opengl2 *)opengl->widget)->brush = QBrush (((Opengl2 *)opengl->widget)->brushcolor);
  ((Opengl2 *)opengl->widget)->painter->setBrush(((Opengl2 *)opengl->widget)->brush);
  ((Opengl2 *)opengl->widget)->brushnull = 0;
  return 0;
}

// ---------------------------------------------------------------------
int gl_brushnull ()
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->brushnull = 1;
  ((Opengl2 *)opengl->widget)->painter->setBrush(Qt::NoBrush);
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  if (0 == *(p) || 0 == *(p + 1))
    return 0;
  QPen pen = ((Opengl2 *)opengl->widget)->painter->pen();
  QBrush brush = ((Opengl2 *)opengl->widget)->painter->brush();
  ((Opengl2 *)opengl->widget)->painter->setPen(QPen (QColor (0, 0, 0),1));
  ((Opengl2 *)opengl->widget)->painter->setBrush(QBrush (QColor (0, 0, 0)));
  ((Opengl2 *)opengl->widget)->painter->drawRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  ((Opengl2 *)opengl->widget)->painter->setPen(pen);
  ((Opengl2 *)opengl->widget)->painter->setBrush(brush);
  return 0;
}

// ---------------------------------------------------------------------
int gl_clear2 (void *p)
{
  Opengl *opengl = (Opengl *) p;
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->color = QColor (0, 0, 0);

// messed up here, but ProFont should be part of config...
  string s=q2s(config.ProFont.family());
  gl_font0(opengl,(char *)s.c_str());

  ((Opengl2 *)opengl->widget)->painter->setWorldMatrixEnabled (true);
  ((Opengl2 *)opengl->widget)->painter->translate (-((Opengl2 *)opengl->widget)->orgx, -((Opengl2 *)opengl->widget)->orgy);
  ((Opengl2 *)opengl->widget)->orgx = 0;
  ((Opengl2 *)opengl->widget)->orgy = 0;
  ((Opengl2 *)opengl->widget)->painter->setClipping (false);
  ((Opengl2 *)opengl->widget)->clipped = 0;

  ((Opengl2 *)opengl->widget)->painter->setPen(QPen (QColor (255, 255, 255),1));
  ((Opengl2 *)opengl->widget)->painter->setBrush(QBrush (QColor (255, 255, 255)));
  ((Opengl2 *)opengl->widget)->painter->drawRect (0, 0, ((Opengl2 *)opengl->widget)->width(), ((Opengl2 *)opengl->widget)->height());

  ((Opengl2 *)opengl->widget)->pen = QPen (((Opengl2 *)opengl->widget)->color, 1); // TODO in user space
  ((Opengl2 *)opengl->widget)->textpen = QPen (((Opengl2 *)opengl->widget)->pen);
  ((Opengl2 *)opengl->widget)->painter->setPen(((Opengl2 *)opengl->widget)->pen);
  ((Opengl2 *)opengl->widget)->brushcolor = QColor (255, 255, 255);
  ((Opengl2 *)opengl->widget)->brush.setColor(((Opengl2 *)opengl->widget)->brushcolor);
  ((Opengl2 *)opengl->widget)->brushnull = 1;
  ((Opengl2 *)opengl->widget)->painter->setBrush(Qt::NoBrush);
  ((Opengl2 *)opengl->widget)->textx = 0;
  ((Opengl2 *)opengl->widget)->texty = 0;
  ((Opengl2 *)opengl->widget)->textcolor = QColor(((Opengl2 *)opengl->widget)->color);
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->clipped = 1;
  ((Opengl2 *)opengl->widget)->painter->setClipRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  ((Opengl2 *)opengl->widget)->painter->setClipping (true);
  return 0;
}

// ---------------------------------------------------------------------
int gl_clipreset ()
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  if (((Opengl2 *)opengl->widget)->clipped) {
    ((Opengl2 *)opengl->widget)->painter->setClipping (false);
    ((Opengl2 *)opengl->widget)->clipped = 0;
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->painter->drawEllipse (*(p), *(p + 1),  *(p + 2),  *(p + 3));
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->font = new Font (string(s));
  ((Opengl2 *)opengl->widget)->painter->setFont ((((Opengl2 *)opengl->widget)->font)->font);
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  size10 = *(p);
  bold = 1 & *(p + 1);
  italic = 2 & *(p + 1);
  underline = 4 & *(p + 1);
  strikeout = 8 & *(p + 1);
  degree10 = *(p + 2);
  char *face = int2utf8 (p + 3, len - 3);
  ((Opengl2 *)opengl->widget)->font = new Font (string(face), size10, !!bold, !!italic, !!strikeout, !!underline, degree10);
  ((Opengl2 *)opengl->widget)->painter->setFont ((((Opengl2 *)opengl->widget)->font)->font);
  return 0;
}

// ---------------------------------------------------------------------
int gl_fontangle (int a)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->font->angle = a;
  return 0;
}

// ---------------------------------------------------------------------
int gl_lines (const int *p, int len)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  ((Opengl2 *)opengl->widget)->painter->drawPolyline (pt,c);
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->pencolor = QColor (((Opengl2 *)opengl->widget)->color);
  ((Opengl2 *)opengl->widget)->pen = QPen (((Opengl2 *)opengl->widget)->pencolor, Max (0.5, *(p))); // TODO in user space
  ((Opengl2 *)opengl->widget)->painter->setPen(((Opengl2 *)opengl->widget)->pen);
  return 0;
}

// ---------------------------------------------------------------------
int gl_pie (const int *p)
{
  int dy[2];
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  qtarcisi (p, p + 4, dy);
  ((Opengl2 *)opengl->widget)->painter->drawPie (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int gl_pixel (const int *p)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->painter->drawPoint (*(p), *(p + 1));
  return 0;
}

// ---------------------------------------------------------------------
static int glpixels2(int x,int y,int w,int h,const uchar *p)
{
  if (w==0||h==0) return 0;
  QImage image = QImage(w,h,QImage::Format_RGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*w*h);

  ((Opengl2 *)opengl->widget)->painter->drawImage(x, y, image);

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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  ((Opengl2 *)opengl->widget)->painter->drawPolygon (pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int gl_rect (const int *p)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->painter->drawRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int gl_rgb (const int *p)
{
  if (!opengl) return 1;
  ((Opengl2 *)opengl->widget)->color = QColor (*(p), *(p + 1), *(p + 2));
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
  Q_UNUSED(ys);
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->painter->setPen(((Opengl2 *)opengl->widget)->textpen);
  if (0 == ((Opengl2 *)opengl->widget)->font->angle)
    ((Opengl2 *)opengl->widget)->painter->drawText ( ((Opengl2 *)opengl->widget)->textx, ((Opengl2 *)opengl->widget)->texty, QString::fromUtf8 (ys));
  else {
    ((Opengl2 *)opengl->widget)->painter->save ();
    ((Opengl2 *)opengl->widget)->painter->translate ( ((Opengl2 *)opengl->widget)->textx, ((Opengl2 *)opengl->widget)->texty );
    ((Opengl2 *)opengl->widget)->painter->rotate ( - ((Opengl2 *)opengl->widget)->font->angle/10 );
    ((Opengl2 *)opengl->widget)->painter->drawText ( 0, 0, QString::fromUtf8 (ys));
    ((Opengl2 *)opengl->widget)->painter->restore ();
  }
  ((Opengl2 *)opengl->widget)->painter->setPen(((Opengl2 *)opengl->widget)->pen);
  return 0;
}

// ---------------------------------------------------------------------
int gl_textcolor ()
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->textcolor = QColor (((Opengl2 *)opengl->widget)->color);
  ((Opengl2 *)opengl->widget)->textpen = QPen (((Opengl2 *)opengl->widget)->painter->pen());
  ((Opengl2 *)opengl->widget)->textpen.setColor (((Opengl2 *)opengl->widget)->textcolor);
  return 0;
}

// ---------------------------------------------------------------------
int gl_textxy (const int *p)
{
  if (!opengl) return 1;
  ((Opengl2 *)opengl->widget)->textx = *(p);
  ((Opengl2 *)opengl->widget)->texty = *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int gl_windoworg (const int *p)
{
  if (!opengl) return 1;
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;
  ((Opengl2 *)opengl->widget)->painter->translate (*(p), *(p + 1));
  ((Opengl2 *)opengl->widget)->orgx += *(p);
  ((Opengl2 *)opengl->widget)->orgy += *(p + 1);
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
  if (!((Opengl2 *)opengl->widget)->painter) return 1;
  if (!((Opengl2 *)opengl->widget)->painter->isActive()) return 1;

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

