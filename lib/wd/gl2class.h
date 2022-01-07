
#ifdef _MSC_VER
#define _USE_MATH_DEFINES // for C++ DM 130130
#include <cmath> // for M_PI  DM 130130
#endif

#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef Abs
#define Abs(a) ((a) < 0 ? (-a) : (a))
#endif

#ifndef Max
#define Max(a, b) ((a) < (b) ? (b) : (a))
#endif

#include <QApplication>
#include <QPainter>
#include "font.h"
#include "form.h"
#include "isidraw.h"
#include "wd.h"
#include "../base/state.h"

extern Form *form;
extern Child *isigraph;
extern Child *opengl;

#if defined(GLISIGRAPH)

#include "gl2.h"
#include "isigraph.h"
#include "isigraph2.h"
#define QTWidget Isigraph
#define QTwidget isigraph
#define QTWidget2 Isigraph2
#define QTwidget2 isigraph2

#define CHKPAINTER  \
  if (!isigraph) return 1; \
  Isigraph2 *w = (Isigraph2 *)isigraph->widget; \
  if (!w->painter) return 1; \
  if (!w->painter->isActive()) return 1;
#define CHKPAINTER2 \
  if (!isigraph) return 1; \
  Isigraph2 *w = (Isigraph2 *)isigraph->widget; \
  Q_UNUSED(w);

#elif defined(GLOPENGL)

#include "ogl2.h"
#include "opengl.h"
#include "opengl2.h"
#define QTWidget Opengl
#define QTWidget2 Opengl2
#define QTwidget opengl
#define QTwidget2 opengl2

#define CHKPAINTER  \
  if (!opengl) return 1; \
  Opengl2 *w = (Opengl2 *)opengl->widget; \
  if (!w->painter) return 1; \
  if (!w->painter->isActive()) return 1;
#define CHKPAINTER2  \
  if (!opengl) return 1; \
  Opengl2 *w = (Opengl2 *)opengl->widget; \
  Q_UNUSED(w);

#define glarc gl_arc
#define glbrush gl_brush
#define glbrushnull gl_brushnull
#define glcapture gl_capture
#define glcaret gl_caret
#define glclear gl_clear
#define glclear2 gl_clear2
#define glclip gl_clip
#define glclipreset gl_clipreset
#define glcmds gl_cmds
#define glcursor gl_cursor
#define glellipse gl_ellipse
#define glfill gl_fill
#define glfont gl_font
#define glfont2 gl_font2
#define glfontangle gl_fontangle
#define glfontextent gl_fontextent
#define gllines gl_lines
#define glnodblbuf gl_nodblbuf
#define glpaint gl_paint
#define glpaintx gl_paintx
#define glpen gl_pen
#define glpie gl_pie
#define glpixel gl_pixel
#define glpixels gl_pixels
#define glpixelsx gl_pixelsx
#define glpolygon gl_polygon
#define glqextent gl_qextent
#define glqextentw gl_qextentw
#define glqhandles gl_qhandles
#define glqpixels gl_qpixels
#define glqpixelm gl_qpixelm
#define glqtextmetrics gl_qtextmetrics
#define glqtype gl_qtype
#define glqwh gl_qwh
#define glrect gl_rect
#define glrgb gl_rgb
#define glrgba gl_rgba
#define glsel gl_sel
#define glsel2 gl_sel2
#define glsetlocale gl_setlocale
#define gltext gl_text
#define gltextcolor gl_textcolor
#define gltextxy gl_textxy
#define glwindoworg gl_windoworg

#elif defined(GLPRINTER)

#ifdef QT50
#include <QtPrintSupport/QPrinter>
#else
#include <QPrinter>
#endif

#include "glz.h"
#include "prtobj.h"
#define QTWidget QPrinter
#define QTwidget Printer
#define QTWidget2 Prtobj
#define QTwidget2 prtobj

extern QPrinter *Printer;
extern Prtobj *prtobj;

#define CHKPAINTER  if ((!Printer) || !Printer->isValid()) return 1; \
  Prtobj *w=prtobj; \
  if (!w->painter) return 1; \
  if (!w->painter->isActive()) return 1;
#define CHKPAINTER2  if ((!Printer) || !Printer->isValid()) return 1; \
  Prtobj *w=prtobj; \
  Q_UNUSED(w);

#define glarc glzarc
#define glbrush glzbrush
#define glbrushnull glzbrushnull
#define glcapture glzcapture
#define glcaret glzcaret
#define glclear glzclear
#define glclear2 glzclear2
#define glclip glzclip
#define glclipreset glzclipreset
#define glcmds glzcmds
#define glcursor glzcursor
#define glellipse glzellipse
#define glfill glzfill
#define glfont glzfont
#define glfont2 glzfont2
#define glfontangle glzfontangle
#define glfontextent glzfontextent
#define gllines glzlines
#define glnodblbuf glznodblbuf
#define glpaint glzpaint
#define glpaintx glzpaintx
#define glpen glzpen
#define glpie glzpie
#define glpixel glzpixel
#define glpixels glzpixels
#define glpixelsx glzpixelsx
#define glpolygon glzpolygon
#define glqextent glzqextent
#define glqextentw glzqextentw
#define glqhandles glzqhandles
#define glqpixels glzqpixels
#define glqpixelm glzqpixelm
#define glqtextmetrics glzqtextmetrics
#define glqtype glzqtype
// #define glqwh glzqwh
#define glrect glzrect
#define glrgb glzrgb
#define glrgba glzrgba
#define glsel glzsel
#define glsel2 glzsel2
#define glsetlocale glzsetlocale
#define gltext glztext
#define gltextcolor glztextcolor
#define gltextxy glztextxy
#define glwindoworg glzwindoworg

#elif defined(GLPAINTER)

#include "glc.h"
#include "drawobj.h"
#define QTWidget2 Drawobj
#define QTwidget2 drawobj

extern Drawobj *drawobj;

#define CHKPAINTER  if (!drawobj) return 1; \
  Drawobj *w=drawobj; \
  Q_UNUSED(w);
#define CHKPAINTER2  if (!drawobj) return 1; \
  Drawobj *w=drawobj; \
  Q_UNUSED(w);

#define glarc glc_arc
#define glbrush glc_brush
#define glbrushnull glc_brushnull
#define glcapture glc_capture
#define glcaret glc_caret
#define glclear glc_clear
#define glclear2 glc_clear2
#define glclip glc_clip
#define glclipreset glc_clipreset
#define glcmds glc_cmds
#define glcursor glc_cursor
#define glellipse glc_ellipse
#define glfill glc_fill
#define glfont glc_font
#define glfont2 glc_font2
#define glfontangle glc_fontangle
#define glfontextent glc_fontextent
#define gllines glc_lines
#define glnodblbuf glc_nodblbuf
#define glpaint glc_paint
#define glpaintx glc_paintx
#define glpen glc_pen
#define glpie glc_pie
#define glpixel glc_pixel
#define glpixels glc_pixels
#define glpixelsx glc_pixelsx
#define glpolygon glc_polygon
#define glqextent glc_qextent
#define glqextentw glc_qextentw
#define glqhandles glc_qhandles
#define glqpixels glc_qpixels
#define glqpixelm glc_qpixelm
#define glqtextmetrics glc_qtextmetrics
#define glqtype glc_qtype
#define glqwh glc_qwh
#define glrect glc_rect
#define glrgb glc_rgb
#define glrgba glc_rgba
#define glsel glc_sel
#define glsel2 glc_sel2
#define glsetlocale glc_setlocale
#define gltext glc_text
#define gltextcolor glc_textcolor
#define gltextxy glc_textxy
#define glwindoworg glc_windoworg

#endif

extern QList<Form *>Forms;

static int glfont0(void *wid, char *s);
static int glfont_i(const int *p, int len);
static int glpixels2(void *wid, int x,int y,int wi,int h,const uchar *p);
static int gltext_i(const int *p, int len);

// ---------------------------------------------------------------------
// caller should free std::string
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
  CHKPAINTER2
#if defined(GLISIGRAPH)
  if (w->type=="isigraph" && w->painter) return 1;
  w->update();
#elif defined(GLOPENGL)
  if (w->painter) return 1;
  w->updateGL();
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glpaintx()
{
  CHKPAINTER2
#if defined(GLISIGRAPH)
  if (w->type=="isigraph" && w->painter) return 1;
  w->repaint();
#elif defined(GLOPENGL)
  return glpaint();
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glqhandles(void **p)
{
  if (!p) return 1;
  CHKPAINTER2
#if defined(GLPRINTER) || defined(GLPAINTER)
  return 1;
#else
  *p = (void *)QTwidget;
#ifdef _WIN32
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
#endif
}

// ---------------------------------------------------------------------
int glqextent(char *s,int *wh)
{
  if (!s || !wh) return 1;
  if (!FontExtent) FontExtent= new Font(q2s("\""+app->font().family())+"\"",app->font().pointSizeF());
  if (FontExtent->error) {
    delete FontExtent;
    FontExtent=0;
    return 1;
  }
#if defined(GLPRINTER)
  CHKPAINTER
  QFontMetrics fm = QFontMetrics(FontExtent->font,prtobj->painter->device());
#else
  QFontMetrics fm = QFontMetrics(FontExtent->font);
#endif
#ifdef QT512
  *(wh) = fm.horizontalAdvance(QString::fromUtf8(s));
#else
  *(wh) = fm.width(QString::fromUtf8(s));
#endif
  *(wh+1) = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glqextentw(char *s,int *wi)
{
  if (!s || !wi) return 1;
  QStringList n=(QString::fromUtf8(s)).split("\n",_KeepEmptyParts);
  if (!FontExtent) FontExtent= new Font(q2s("\""+app->font().family())+"\"",app->font().pointSizeF());
  if (FontExtent->error) {
    delete FontExtent;
    FontExtent=0;
    return 1;
  }
#if defined(GLPRINTER)
  CHKPAINTER
  QFontMetrics fm = QFontMetrics(FontExtent->font,prtobj->painter->device());
#else
  QFontMetrics fm = QFontMetrics(FontExtent->font);
#endif
  for (int i=0; i<n.size(); i++) {
#ifdef QT512
    wi[i] = fm.horizontalAdvance(n.at(i));
#else
    wi[i] = fm.width(n.at(i));
#endif
  }
  return 0;
}

// ---------------------------------------------------------------------
int glqpixelm(const int *p, int *shape, int *pix)
{
  if (!p || !shape || !pix) return 1;
  CHKPAINTER2
#ifdef GLPRINTER
  return 1;
#else
  QPixmap pm=w->getpixmap();
  if (!pm || pm.isNull()) return 1;
  int x=p[0];
  int y=p[1];
  int r=p[2];
  int s=p[3];
  if (x<0 || x>=pm.width() || y<0 || y>=pm.height()) return 1;
  if (r==-1) r=pm.width()-x;
  if (s==-1) s=pm.height()-y;
  r=qMin(r,pm.width()-x);
  s=qMin(s,pm.height()-y);
  QPixmap a1 = (pm.copy(x, y, r, s));
  if (a1.isNull()) return 1;
  QImage a2 = a1.toImage();
  if (a2.isNull()) return 1;
  QImage a3 = a2.convertToFormat(QImage::Format_ARGB32);
  if (a3.isNull()) return 1;
  const uchar *t = a3.bits();
  if (!t) return 1;
  shape[0]=s;
  shape[1]=r;
  memcpy((uchar *)pix,t,4*r*s);
  return 0;
#endif
}

// ---------------------------------------------------------------------
int glqpixels(const int *p, int *pix)
{
  if (!p || !pix) return 1;
  CHKPAINTER2
#ifdef GLPRINTER
  return 1;
#else
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
#endif
}

// ---------------------------------------------------------------------
// Height, Ascent, Descent, InternalLeading, ExternalLeading, AverageCharWidth, MaxCharWidth
int glqtextmetrics(int *tm)
{
  if (!tm) return 1;
  if (!FontExtent) FontExtent= new Font(q2s("\""+app->font().family())+"\"",app->font().pointSizeF());
#if defined(GLPRINTER)
  CHKPAINTER
  QFontMetrics fm = QFontMetrics(FontExtent->font,prtobj->painter->device());
#else
  QFontMetrics fm = QFontMetrics(FontExtent->font);
#endif
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
int glqtype(int *type)
{
  if (!type) return 1;
  CHKPAINTER2
#if defined(GLISIGRAPH)
  type[0] = (w->type=="isigraph")?0:1;
#elif defined(GLOPENGL)
  type[0] = 2;
#elif defined(GLPRINTER)
  type[0] = 3;
#elif defined(GLPAINTER)
  type[0] = 4;
#else
  type[0] = 0;
#endif
  return 0;
}

// ---------------------------------------------------------------------
#ifndef GLPRINTER
int glqwh(int *wh)
{
  if (!wh) return 1;
  CHKPAINTER2
  wh[0] = w->width();
  wh[1] = w->height();
  return 0;
}
#endif

// ---------------------------------------------------------------------
int glsel(void *g)
{
  if (!g) return 1;
#if (!(defined(GLPRINTER) || defined(GLPAINTER)))
  Form *f;
  Child *c=(Child *)g;
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild(c)&&!f->closed) {
#ifdef GLOPENGL
      if ((c->type == "opengl") && c->widget) {
#else
      if (((c->type == "isigraph")||(c->type == "isidraw")) && c->widget) {
#endif
        form = f;
        form->child = c;
        QTwidget = c;
        return 0;
      }
    }
  }
#endif
  return 1;
}

// ---------------------------------------------------------------------
int glsel2(char *g)
{
  if (!g) return 1;
#if (!(defined(GLPRINTER) || defined(GLPAINTER)))
  Child *cc;
  std::string p=std::string(g);
  if (p.size()==0) {
    return 1;
  }
  if ((p[0]=='_')||(p[0]=='-')||(p[0]=='0')||(p[0]=='1')||(p[0]=='2')||(p[0]=='3')||(p[0]=='4')||(p[0]=='5')||(p[0]=='6')||(p[0]=='7')||(p[0]=='8')||(p[0]=='9')) {
    return glsel((void *) c_strtol(p));
  }
  Form *f;
// search current form first
  if (form&&!form->closed) {
    f=form;
    if ((cc=f->id2child(g))) {
#ifdef GLOPENGL
      if ((cc->type == "opengl") && cc->widget) {
#else
      if (((cc->type == "isigraph")||(cc->type == "isidraw")) && cc->widget) {
#endif
        form=f;
        form->child = cc;
        QTwidget = cc;
        return 0;
      }
    }
  }
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if ((cc=f->id2child(g))&&!f->closed) {
#ifdef GLOPENGL
      if ((cc->type == "opengl") && cc->widget) {
#else
      if (((cc->type == "isigraph")||(cc->type == "isidraw")) && cc->widget) {
#endif
        form=f;
        form->child = cc;
        QTwidget = cc;
        return 0;
      }
    }
  }
#endif
  return 1;
}

// ---------------------------------------------------------------------
int glarc(const int *p)
{
  int dy[2];
  CHKPAINTER
  qtarcisi(p,  p + 4, dy);
  w->painter->drawArc(*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glbrush()
{
  CHKPAINTER
  w->brushcolor = QColor(w->color);
  w->brush = QBrush(w->brushcolor);
  w->painter->setBrush(w->brush);
  w->brushnull = 0;
  return 0;
}

// ---------------------------------------------------------------------
int glbrushnull()
{
  CHKPAINTER
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
  CHKPAINTER
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
int glclear2(void *p,int clear)
{
#if defined(GLPRINTER)
  Q_UNUSED(clear);
  Prtobj *w = (Prtobj *)p;
#elif defined(GLPAINTER)
  Drawobj *w = (Drawobj *)p;
#elif 1
  QTWidget2 *w = (QTWidget2 *)p;
#endif
  if (!w) return 1;
  w->color = QColor(0, 0, 0);

  if (glfont0(w,(char *)"profont")) return 1;
  w->orgx = 0;
  w->orgy = 0;
  w->clipped = 0;

  if (w->painter && w->painter->isActive()) w->painter->setPen(QPen(QColor(255, 255, 255),1));
  if (w->painter && w->painter->isActive()) w->painter->setBrush(QBrush(QColor(255, 255, 255)));
#ifndef GLPRINTER
  if (clear && w->painter && w->painter->isActive()) w->painter->drawRect(0, 0, w->width(), w->height());
#endif

  w->pen = QPen(w->color, 1);  // TODO in user space
  w->textpen = QPen(w->pen);
  if (w->painter && w->painter->isActive()) w->painter->setPen(w->pen);
  w->brushcolor = QColor(255, 255, 255);
  w->brush = QBrush(w->brushcolor);
  w->brushnull = 1;
  if (w->painter && w->painter->isActive()) w->painter->setBrush(Qt::NoBrush);
  w->textx = 0;
  w->texty = 0;
  w->textcolor = QColor(w->color);
  return 0;
}

// ---------------------------------------------------------------------
int glclear()
{
  if (FontExtent) {
    delete FontExtent;
    FontExtent=0;
  }
  CHKPAINTER
#if (defined(GLISIGRAPH)||defined(GLPAINTER))
  return glclear2(w,1);
#else
  return glclear2(w,0);
#endif
}

// ---------------------------------------------------------------------
int glclip(const int *p)
{
  CHKPAINTER
  w->clipped = 1;
  w->painter->setClipRect(*(p), *(p + 1), *(p + 2), *(p + 3));
  w->painter->setClipping(true);
  return 0;
}

// ---------------------------------------------------------------------
int glclipreset()
{
  CHKPAINTER
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
  CHKPAINTER2
#if defined(GLISIGRAPH)||defined(GLOPENGL)
  if (-1==a)
    w->unsetCursor();
  else
    w->setCursor((Qt::CursorShape)a);
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glellipse(const int *p)
{
  CHKPAINTER
  w->painter->drawEllipse(*(p), *(p + 1),  *(p + 2),  *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int glfill(const int *p)
{
  CHKPAINTER
  Q_UNUSED(p);
#ifndef GLPRINTER
  w->fill(p);
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glfont_i(const int *p, int len)
{
  char *face = int2utf8(p, len);
  int r = glfont(face);
  free(face);
  return r;
}

// ---------------------------------------------------------------------
int glfont0(void *wid, char *s)
{
  if (!s) return 1;
#if defined(GLPRINTER)
  Prtobj *w = (Prtobj *)wid;
#elif defined(GLPAINTER)
  Drawobj *w = (Drawobj *)wid;
#elif 1
  QTWidget2 *w = (QTWidget2 *)wid;
#endif
  if (!w) return 1;
  Font *fnt = new Font(std::string(s));
  if (fnt->error) {
    delete fnt;
    return 1;
  }
  w->font = fnt;
// also set glfontextent
  if (FontExtent) delete FontExtent;
  FontExtent=new Font(std::string(s));
  return 0;
}

// ---------------------------------------------------------------------
int glfont(char *s)
{
  CHKPAINTER2
  return glfont0(w, s);
}

// ---------------------------------------------------------------------
int glfont2(const int *p, int len)
{
  int size10, degree10, bold, italic, strikeout, underline;
  CHKPAINTER2
  size10 = *(p);
  bold = 1 & *(p + 1);
  italic = 2 & *(p + 1);
  underline = 4 & *(p + 1);
  strikeout = 8 & *(p + 1);
  degree10 = *(p + 2);
  char *face = int2utf8(p + 3, len - 3);
  Font *fnt = new Font(std::string(face), size10, !!bold, !!italic, !!strikeout, !!underline, degree10);
  if (fnt->error) {
    delete fnt;
    return 1;
  }
  w->font = fnt;
// also set glfontextent
  if (FontExtent) delete FontExtent;
  FontExtent=new Font(std::string(face), size10, !!bold, !!italic, !!strikeout, !!underline, degree10);
  return 0;
}

// ---------------------------------------------------------------------
int glfontangle(int a)
{
  CHKPAINTER2
  w->font->angle = a;
  return 0;
}

// ---------------------------------------------------------------------
int glfontextent(char *s)
{
  if (!s) return 1;
  if (!strlen(s)) {
    if (FontExtent) delete FontExtent;
    FontExtent=0;
  } else {
    Font *fnt = new Font(std::string(s));
    if (fnt->error) {
      delete fnt;
      return 1;
    }
    FontExtent=fnt;
  }
  return 0;
}

// ---------------------------------------------------------------------
int gllines(const int *p, int len)
{
  CHKPAINTER
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
  CHKPAINTER
  w->pencolor = QColor(w->color);
  w->pen = QPen(w->pencolor, Max(0.5,p[0]));
  w->pen.setStyle((Qt::PenStyle)p[1]);
  w->painter->setPen(w->pen);
  return 0;
}

// ---------------------------------------------------------------------
int glpie(const int *p)
{
  int dy[2];
  CHKPAINTER
  qtarcisi(p, p + 4, dy);
  w->painter->drawPie(*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glpixel(const int *p)
{
  CHKPAINTER
  w->painter->drawPoint(*(p), *(p + 1));
  return 0;
}

// ---------------------------------------------------------------------
int glpixels2(void *wid, int x,int y,int wi,int h,const uchar *p)
{
  if (!wi || !h || !p) return 1;
#if defined(GLPRINTER)
  Prtobj *w = (Prtobj *)wid;
#elif defined(GLPAINTER)
  Drawobj *w = (Drawobj *)wid;
#elif 1
  QTWidget2 *w = (QTWidget2 *)wid;
#endif
  QImage image = QImage(wi,h,QImage::Format_ARGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*wi*h);
  w->painter->drawImage(x, y, image);
  return 0;
}

// ---------------------------------------------------------------------
int glpixels(const int *p, int len)
{
  if (len<4) return 1;
  if (len!=4+p[2]*Abs(p[3])) return 1;
  CHKPAINTER
  return glpixels2(w, *(p), *(p + 1), *(p + 2), *(p + 3), (uchar *)(p + 4));
}

// ---------------------------------------------------------------------
int glpixelsx(const int *p)
{
  CHKPAINTER
#if defined(_WIN64)||defined(__LP64__)
  Q_UNUSED(p);
  return 1;
#else
  return glpixels2(w, *(p), *(p + 1), *(p + 2), *(p + 3), (uchar *) * (p + 4));
#endif
}

// ---------------------------------------------------------------------
int glpolygon(const int *p, int len)
{
  CHKPAINTER
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
  CHKPAINTER
  w->painter->drawRect(*(p), *(p + 1), *(p + 2), *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int glrgb(const int *p)
{
  CHKPAINTER2
  w->color = QColor(*(p), *(p + 1), *(p + 2));
  return 0;
}

// ---------------------------------------------------------------------
int glrgba(const int *p)
{
  CHKPAINTER2
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
  CHKPAINTER
  if (!w->font) return 1;
  QFontMetrics fm = QFontMetrics((w->font)->font);
  QString qs = QString::fromUtf8(ys);

  w->painter->setPen(w->textpen);
  w->painter->setFont((w->font)->font);
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
  CHKPAINTER
  w->textcolor = QColor(w->color);
  w->textpen = QPen(w->painter->pen());
  w->textpen.setColor(w->textcolor);
  w->textpen.setStyle(Qt::SolidLine);
  return 0;
}

// ---------------------------------------------------------------------
int gltextxy(const int *p)
{
  CHKPAINTER2
  w->textx = *(p);
  w->texty = *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int glwindoworg(const int *p)
{
  CHKPAINTER
  w->painter->translate(*(p), *(p + 1));
  w->orgx += *(p);
  w->orgy += *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int glcmds(const int *ptr, int ncnt)
{
  int cnt;
  int p = 0;
  int rc = 0;

  CHKPAINTER

  while (p < ncnt) {
    cnt = *(ptr + p);
    if (2>cnt) return 1;
    switch (*(ptr + p + 1)) {

    case 2001:		// glarc
      rc = glarc(ptr + p + 2);
      break;

    case 2004:		// glbrush
      rc = glbrush();
      break;

    case 2005:		// glbrushnull
      rc = glbrushnull();
      break;

    case 2062:		//glcapture
      rc = glcapture(*(ptr + p + 2));
      break;

    case 2065:		//glcaret
      rc = glcaret(ptr + p + 2);
      break;

    case 2007:		//glclear
      glclear();
      break;

    case 2078:		//glclip
      rc = glclip(ptr + p + 2);
      break;

    case 2079:		//glclipreset
      rc = glclipreset();
      break;

    case 2069:		//glcursor
      rc = glcursor(*(ptr + p + 2));
      break;

    case 2008:		// glellipse
      rc = glellipse(ptr + p + 2);
      break;

    case 2093:		// glfill
      rc = glfill(ptr + p + 2);
      break;

    case 2012:		// glfont
      rc = glfont_i((ptr + p + 2), cnt-2);
      break;

    case 2312:		// glfont2
      rc = glfont2((ptr + p + 2), cnt-2);
      break;

    case 2342:		// glfontangle
      rc = glfontangle(*(ptr + p + 2));
      break;

    case 2015:		// gllines
      rc = gllines((ptr + p + 2), cnt-2);
      break;

    case 2070:		// glnodblbuf
      rc = glnodblbuf(*(ptr + p + 2));
      break;

    case 2022:		// glpen
      rc = glpen(ptr + p + 2);
      break;

    case 2023:		// glpie
      rc = glpie(ptr + p + 2);
      break;

    case 2024:		// glpixel
      rc = glpixel(ptr + p + 2);
      break;

    case 2076:		// glpixels
      rc = glpixels((ptr + p + 2), cnt-2);
      break;

    case 2075:		// glpixelsx
      rc = glpixelsx(ptr + p + 2);
      break;

    case 2029:		// glpolygon
      rc = glpolygon((ptr + p + 2), cnt-2);
      break;

    case 2031:		// glrect
      rc = glrect(ptr + p + 2);
      break;

    case 2032:		// glrgb
      rc = glrgb(ptr + p + 2);
      break;

    case 2343:		// glrgba
      rc = glrgba(ptr + p + 2);
      break;

    case 2038:		// gltext
      rc = gltext_i(ptr + p + 2, cnt-2);
      break;

    case 2040:		// gltextcolor
      rc = gltextcolor();
      break;

    case 2056:		// gltextxy
      rc = gltextxy(ptr + p + 2);
      break;

    case 2045:		//glwindoworg
      rc = glwindoworg(ptr + p + 2);
      break;

    default:
      return 1;
      break;
    }
    if (rc) return rc;
    p = p + cnt;
  }
  return 0;
}

// ---------------------------------------------------------------------
int glsetlocale(char *c)
{
  CHKPAINTER2
  Q_UNUSED(c);
#if (!(defined(GLPRINTER) || defined(GLPAINTER)))
  QTwidget->locale = std::string(c);
#endif
  return 0;
}
