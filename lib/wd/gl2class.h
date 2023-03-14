
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
#define CHKPAINTER3 if (!isigraph) return 0; \
  Isigraph2 *w = (Isigraph2 *)isigraph->widget; \
  if (!w) return 0;

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
#define CHKPAINTER3 if (!opengl) return 0; \
  Opengl2 *w = (Opengl2 *)opengl->widget; \
  if (!w) return 0;

#define glclear2 gl_clear2
#define glquery gl_query
#define glquery2 gl_query2

#define glarc gl_arc
#define glbrush gl_brush
#define glbrushnull gl_brushnull
#define glcapture gl_capture
#define glcaret gl_caret
#define glclear gl_clear
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
#define glrect gl_rect
#define glrgb gl_rgb
#define glrgba gl_rgba
#define glsel gl_sel
#define glsel2 gl_sel2
#define gltext gl_text
#define gltextcolor gl_textcolor
#define gltextxy gl_textxy
#define glwaitgl gl_waitgl
#define glwaitnative gl_waitnative
#define glwindoworg gl_windoworg
#define glupdategl gl_updategl
#define glsetlocale gl_setlocale
#define glqhandles gl_qhandles
#define glqwh gl_qwh
#define glqpixelm gl_qpixelm
#define glqpixels gl_qpixels
#define glqextent gl_qextent
#define glqextentw gl_qextentw
#define glqtextmetrics gl_qtextmetrics
#define glqtype gl_qtype

#define glarc_n gl_arc_n
#define glbrush_n gl_brush_n
#define glbrushnull_n gl_brushnull_n
#define glcapture_n gl_capture_n
#define glcaret_n gl_caret_n
#define glclear_n gl_clear_n
#define glclip_n gl_clip_n
#define glclipreset_n gl_clipreset_n
#define glcmds_n gl_cmds_n
#define glcursor_n gl_cursor_n
#define glellipse_n gl_ellipse_n
#define glfill_n gl_fill_n
#define glfont_n gl_font_n
#define glfont2_n gl_font2_n
#define glfontangle_n gl_fontangle_n
#define glfontextent_n gl_fontextent_n
#define gllines_n gl_lines_n
#define glnodblbuf_n gl_nodblbuf_n
#define glpaint_n gl_paint_n
#define glpaintx_n gl_paintx_n
#define glpen_n gl_pen_n
#define glpie_n gl_pie_n
#define glpixel_n gl_pixel_n
#define glpixels_n gl_pixels_n
#define glpixelsx_n gl_pixelsx_n
#define glpolygon_n gl_polygon_n
#define glrect_n gl_rect_n
#define glrgb_n gl_rgb_n
#define glrgba_n gl_rgba_n
#define glsel_n gl_sel_n
#define glsel2_n gl_sel2_n
#define gltext_n gl_text_n
#define gltextcolor_n gl_textcolor_n
#define gltextxy_n gl_textxy_n
#define glwaitgl_n gl_waitgl_n
#define glwaitnative_n gl_waitnative_n
#define glwindoworg_n gl_windoworg_n
#define glupdategl_n gl_updategl_n
#define glsetlocale_n gl_setlocale_n
#define glqhandles_n gl_qhandles_n
#define glqwh_n gl_qwh_n
#define glqpixelm_n gl_qpixelm_n
#define glqpixels_n gl_qpixels_n
#define glqextent_n gl_qextent_n
#define glqextentw_n gl_qextentw_n
#define glqtextmetrics_n gl_qtextmetrics_n
#define glwaitgl_n gl_waitgl_n
#define glwaitnative_n gl_waitnative_n
#define glqtype_n gl_qtype_n

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
#define CHKPAINTER3 if (!Printer) return 0; \
  Prtobj *w=prtobj; \
  if(!w) return 0;

#define glclear2 glzclear2
#define glquery glzquery
#define glquery2 glzquery2

#define glarc glzarc
#define glbrush glzbrush
#define glbrushnull glzbrushnull
#define glclear glzclear
#define glclip glzclip
#define glclipreset glzclipreset
#define glcmds glzcmds
#define glellipse glzellipse
#define glfill glzfill
#define glfont glzfont
#define glfont2 glzfont2
#define glfontangle glzfontangle
#define glfontextent glzfontextent
#define gllines glzlines
#define glnodblbuf glznodblbuf
#define glpen glzpen
#define glpie glzpie
#define glpixel glzpixel
#define glpixels glzpixels
#define glpixelsx glzpixelsx
#define glpolygon glzpolygon
#define glrect glzrect
#define glrgb glzrgb
#define glrgba glzrgba
#define gltext glztext
#define gltextcolor glztextcolor
#define gltextxy glztextxy
#define glwindoworg glzwindoworg
#define glresolution glzresolution
#define glcolormode glzcolormode
#define glduplexmode glzduplexmode
#define glorientation glzorientation
#define gloutputformat glzoutputformat
#define glpageorder glzpageorder
#define glpapersize glzpapersize
#define glpapersource glzpapersource
#define glscale glzscale
#define glabortdoc glzabortdoc
#define glenddoc glzenddoc
#define glnewpage glznewpage
#define glprinter glzprinter
#define glstartdoc glzstartdoc
#define glinitprinter glzinitprinter
#define glqresolution glzqresolution
#define glqcolormode glzqcolormode
#define glqduplexmode glzqduplexmode
#define glqorientation glzqorientation
#define glqoutputformat glzqoutputformat
#define glqpageorder glzqpageorder
#define glqpapersize glzqpapersize
#define glqpapersource glzqpapersource
#define glqwh glzqwh
#define glqmargins glzqmargins
#define glqextent glzqextent
#define glqextentw glzqextentw
#define glqtextmetrics glzqtextmetrics
#define glcapture glzcapture
#define glcaret glzcaret
#define glcursor glzcursor
#define glqtype glzqtype
#define glpaint glzpaint
#define glpaintx glzpaintx
#define glsetlocale glzsetlocale

#define glarc_n glzarc_n
#define glbrush_n glzbrush_n
#define glbrushnull_n glzbrushnull_n
#define glclear_n glzclear_n
#define glclip_n glzclip_n
#define glclipreset_n glzclipreset_n
#define glcmds_n glzcmds_n
#define glellipse_n glzellipse_n
#define glfill_n glzfill_n
#define glfont_n glzfont_n
#define glfont2_n glzfont2_n
#define glfontangle_n glzfontangle_n
#define glfontextent_n glzfontextent_n
#define gllines_n glzlines_n
#define glnodblbuf_n glznodblbuf_n
#define glpen_n glzpen_n
#define glpie_n glzpie_n
#define glpixel_n glzpixel_n
#define glpixels_n glzpixels_n
#define glpixelsx_n glzpixelsx_n
#define glpolygon_n glzpolygon_n
#define glrect_n glzrect_n
#define glrgb_n glzrgb_n
#define glrgba_n glzrgba_n
#define gltext_n glztext_n
#define gltextcolor_n glztextcolor_n
#define gltextxy_n glztextxy_n
#define glwindoworg_n glzwindoworg_n
#define glresolution_n glzresolution_n
#define glcolormode_n glzcolormode_n
#define glduplexmode_n glzduplexmode_n
#define glorientation_n glzorientation_n
#define gloutputformat_n glzoutputformat_n
#define glpageorder_n glzpageorder_n
#define glpapersize_n glzpapersize_n
#define glpapersource_n glzpapersource_n
#define glscale_n glzscale_n
#define glabortdoc_n glzabortdoc_n
#define glenddoc_n glzenddoc_n
#define glnewpage_n glznewpage_n
#define glprinter_n glzprinter_n
#define glstartdoc_n glzstartdoc_n
#define glinitprinter_n glzinitprinter_n
#define glqresolution_n glzqresolution_n
#define glqcolormode_n glzqcolormode_n
#define glqduplexmode_n glzqduplexmode_n
#define glqorientation_n glzqorientation_n
#define glqoutputformat_n glzqoutputformat_n
#define glqpageorder_n glzqpageorder_n
#define glqpapersize_n glzqpapersize_n
#define glqpapersource_n glzqpapersource_n
#define glqwh_n glzqwh_n
#define glqmargins_n glzqmargins_n
#define glqextent_n glzqextent_n
#define glqextentw_n glzqextentw_n
#define glqtextmetrics_n glzqtextmetrics_n
#define glcapture_n glzcapture_n
#define glcaret_n glzcaret_n
#define glcursor_n glzcursor_n
#define glqtype_n glzqtype_n
#define glpaint_n glzpaint_n
#define glpaintx_n glzpaintx_n
#define glsetlocale_n glzsetlocale_n

#endif

extern QList<Form *>Forms;

static int glfont_i(const int *p, int len);
static int glfont0(void *wid, char *s);
static int glfont_i(const int *p, int len);
static int glfontextent_i(const int *p, int len);
static int glpixels2(void *wid, int x,int y,int wi,int h,const uchar *p);
static int gltext_i(const int *p, int len);

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
  if (w->type=="isigraph" && w->insidepaint) {
    w->pendingpaint = true;
    return 0;
  }
  w->update();
#elif defined(GLOPENGL)
  if (w->insidepaint) {
    w->pendingpaint = true;
    return 0;
  }
  w->updateGL();
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glpaintx()
{
  CHKPAINTER2
#if defined(GLISIGRAPH)
  if (w->type=="isigraph" && w->insidepaint) {
    w->pendingpaint = true;
    return 0;
  }
  w->repaint();
#elif defined(GLOPENGL)
  if (w->insidepaint) {
    w->pendingpaint = true;
    return 0;
  }
  w->updateGL();
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glqhandles(void **p)
{
  if (!p) return 1;
  CHKPAINTER2
#if defined(GLPRINTER)
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
#else
  type[0] = 0;
#endif
  return 0;
}

#ifndef GLPRINTER
// ---------------------------------------------------------------------
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
#if !defined(GLPRINTER)
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
#if !defined(GLPRINTER)
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
  CHKPAINTER3
#if defined(GLISIGRAPH)
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
static int glfont_i(const int *p, int len)
{
  char *face = int2utf8(p, len);
  int r = glfont(face);
  free(face);
  return r;
}

// ---------------------------------------------------------------------
static int glfont0(void *wid, char *s)
{
  if (!s) return 1;
#if defined(GLPRINTER)
  Prtobj *w = (Prtobj *)wid;
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
static int glfontextent_i(const int *p, int len)
{
  char *s = int2utf8(p, len);
  int r = glfontextent(s);
  free(s);
  return r;
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
static int glpixels2(void *wid, int x,int y,int wi,int h,const uchar *p)
{
  if (!wi || !h || !p) return 1;
#if defined(GLPRINTER)
  Prtobj *w = (Prtobj *)wid;
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
static int glsetlocale_i(const int *p, int len)
{
  char *s = int2utf8(p, len);
  int r = glsetlocale(s);
  free(s);
  return r;
}

// ---------------------------------------------------------------------
int glsetlocale(char *c)
{
  CHKPAINTER2
  Q_UNUSED(c);
#if !defined(GLPRINTER)
  QTwidget->locale = std::string(c);
#endif
  return 0;
}

// ---------------------------------------------------------------------
int glcmds(const int *ptr, int ncnt)
{
  int cnt;
  int p = 0;
  int rc = 0;
  char *st1;
#if defined(GLPRINTER)
  char *st2;
  float xy[2];
  int pn1,pn2;
#endif

  if (2>ncnt) return 1;  // not enough argument
  int t=*(ptr + p + 1);
  cnt = *(ptr + p);
  if ( (cnt==ncnt)  && (t==glclear_n) ) {
    CHKPAINTER3 return rc = glclear();
  } else if ( (cnt==ncnt)  && ( (t==glpaint_n) || (t==glpaintx_n) || (t==glcursor_n) || (t==glfont_n) ||
                                (t==glfont2_n) || (t==glfontangle_n) || (t==glrgb_n) || (t==glrgba_n) ||
                                (t==gltextxy_n) || (t==glsetlocale_n) )) {
    CHKPAINTER2
  } else {
    CHKPAINTER
  }

  while (p < ncnt) {
    cnt = *(ptr + p);
    if (2>cnt) return 1;
    switch (*(ptr + p + 1)) {

    case glpaint_n:
      rc = glpaint();
      break;

    case glpaintx_n:
      rc = glpaintx();
      break;

    case glarc_n:
      rc = glarc(ptr + p + 2);
      break;

    case glbrush_n:
      rc = glbrush();
      break;

    case glbrushnull_n:
      rc = glbrushnull();
      break;

    case glcapture_n:
      rc = glcapture(*(ptr + p + 2));
      break;

    case glcaret_n:
      rc = glcaret(ptr + p + 2);
      break;

    case glclear_n:
      glclear();
      break;

    case glclip_n:
      rc = glclip(ptr + p + 2);
      break;

    case glclipreset_n:
      rc = glclipreset();
      break;

    case glcursor_n:
      rc = glcursor(*(ptr + p + 2));
      break;

    case glellipse_n:
      rc = glellipse(ptr + p + 2);
      break;

    case glfill_n:
      rc = glfill(ptr + p + 2);
      break;

    case glfont_n:
      rc = glfont_i((ptr + p + 2), cnt-2);
      break;

    case glfont2_n:
      rc = glfont2((ptr + p + 2), cnt-2);
      break;

    case glfontangle_n:
      rc = glfontangle(*(ptr + p + 2));
      break;

    case gllines_n:
      rc = gllines((ptr + p + 2), cnt-2);
      break;

    case glnodblbuf_n:
      rc = glnodblbuf(*(ptr + p + 2));
      break;

    case glpen_n:
      rc = glpen(ptr + p + 2);
      break;

    case glpie_n:
      rc = glpie(ptr + p + 2);
      break;

    case glpixel_n:
      rc = glpixel(ptr + p + 2);
      break;

    case glpixels_n:
      rc = glpixels((ptr + p + 2), cnt-2);
      break;

    case glpixelsx_n:
      rc = glpixelsx(ptr + p + 2);
      break;

    case glpolygon_n:
      rc = glpolygon((ptr + p + 2), cnt-2);
      break;

    case glrect_n:
      rc = glrect(ptr + p + 2);
      break;

    case glrgb_n:
      rc = glrgb(ptr + p + 2);
      break;

    case glrgba_n:
      rc = glrgba(ptr + p + 2);
      break;

    case gltext_n:
      rc = gltext_i(ptr + p + 2, cnt-2);
      break;

    case gltextcolor_n:
      rc = gltextcolor();
      break;

    case gltextxy_n:
      rc = gltextxy(ptr + p + 2);
      break;

    case glwindoworg_n:
      rc = glwindoworg(ptr + p + 2);
      break;

    case glfontextent_n:
      rc = glfontextent_i((ptr + p + 2), cnt-2);
      break;

    case glsetlocale_n:
      rc = glsetlocale_i((ptr + p + 2), cnt-2);
      break;

#if defined(GLPRINTER)

    case glzresolution_n:
      if (1!=cnt-2) return 1;
      rc = glzresolution(*(ptr + p + 2));
      break;

    case glzcolormode_n:
      if (1!=cnt-2) return 1;
      rc = glzcolormode(*(ptr + p + 2));
      break;

    case glzduplexmode_n:
      if (1!=cnt-2) return 1;
      rc = glzduplexmode(*(ptr + p + 2));
      break;

    case glzorientation_n:
      if (1!=cnt-2) return 1;
      rc = glzorientation(*(ptr + p + 2));
      break;

    case glzoutputformat_n:
      if (1!=cnt-2) return 1;
      rc = glzoutputformat(*(ptr + p + 2));
      break;

    case glzpageorder_n:
      if (1!=cnt-2) return 1;
      rc = glzpageorder(*(ptr + p + 2));
      break;

    case glzpapersize_n:
      if (1!=cnt-2) return 1;
      rc = glzpapersize(*(ptr + p + 2));
      break;

    case glzpapersource_n:
      if (1!=cnt-2) return 1;
      rc = glzpapersource(*(ptr + p + 2));
      break;

    case glzscale_n:
      if (2!=cnt-2) return 1;
      xy[0] = *(ptr + p + 2)/1000.0;
      xy[1] = *(ptr + p + 2 + 1)/1000.0;
      rc = glzscale(xy);
      break;

    case glzabortdoc_n:
      if (0!=cnt-2) return 1;
      rc = glzabortdoc();
      break;

    case glzenddoc_n:
      if (0!=cnt-2) return 1;
      rc = glzenddoc();
      break;

    case glznewpage_n:
      if (0!=cnt-2) return 1;
      rc = glznewpage();
      break;

    case glzprinter_n:
      st1 = int2utf8((ptr + p + 2), cnt-2);
      rc = glzprinter(st1);
      free(st1);
      break;

    case glzstartdoc_n:
      if (2>cnt-2) return 1;
      pn1 = *(ptr + p + 2);
      if (pn1>cnt-2-1) return 1;
      st1 = int2utf8((ptr + p + 2 + 1), pn1);
      pn2 = *(ptr + p + 2 + 1 + pn1);
      if (pn2>cnt-2-1-pn1-1) return 1;
      st2 = int2utf8((ptr + p + 2 + 1 + pn1), pn2);
      rc = glzstartdoc(st1, st2);
      free(st1);
      free(st2);
      break;

    case glzinitprinter_n:
      if (0!=cnt-2) return 1;
      rc = glzinitprinter();
      break;
#else
    case glsel2_n:
      st1 = int2utf8((ptr + p + 2), cnt-2);
      rc = glsel2(st1);
      free(st1);
      break;
#endif

    default:
      qDebug() << "glcmds missing " << (*(ptr + p + 1));
      return 1;
      break;
    }
    if (rc) return rc;
    p = p + cnt;
  }
  return 0;
}

int glquery(int t, int* ptr, int ncnt)
{
  int rc = 0;
  if(t==glqtype_n) {
    intresult.resize(1);
    if(!(rc=glqtype(&intresult[0]))) {
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
#if !defined(GLPRINTER)
  } else if(t==glqhandles_n) {
    longresult.resize(2);
    if(!(rc=glqhandles((void**)&longresult[0]))) {
      resulttype = 2;
      rc = -1;
    } else longresult.clear();
  } else if(t==glqwh_n) {
    intresult.resize(2);
    if(!(rc=glqwh(&intresult[0]))) {
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
#else
  } else if(t==glqwh_n) {
    if(1!=ncnt) {
      qDebug() << "argument not 1 integer";
      return rc = 1;
    }
    float wh[2];
    if(!(rc=glzqwh(wh,*ptr))) {
      intresult.resize(2);
      intresult[0] = 1000.0 * wh[0];
      intresult[1] = 1000.0 * wh[1];
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
#endif
  } else if(t==glqtextmetrics_n) {
    intresult.resize(7);
    if(!(rc=glqtextmetrics(&intresult[0]))) {
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
  } else if(t==glqextent_n) {
    char *s=int2utf8(ptr,ncnt);
    intresult.resize(2);
    if(!(rc=glqextent(s,&intresult[0]))) {
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
    free(s);
  } else if(t==glqextentw_n) {
    char *s=int2utf8(ptr,ncnt);
    QStringList n=(QString::fromUtf8(s)).split("\n",_KeepEmptyParts);
    intresult.resize(n.size());
    if(!(rc=glqextent(s,&intresult[0]))) {
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
    free(s);
#if !defined(GLPRINTER)
  } else if(t==glqpixels_n) {
    if(4!=ncnt) {
      qDebug() << "argument not 4 integers";
      return rc = 1;
    }
    intresult.resize(*(ptr+2) * *(ptr+3));
    if(!(rc=glqpixels(ptr,&intresult[0]))) {
      resulttype = 1;
//      backward compatible return as rank-1 array
//        resultshape[0]=*(ptr+2); resultshape[1]=*(ptr+3);
      rc = -1;
    } else intresult.clear();
  } else if(t==glqpixelm_n) {
    if(4!=ncnt) {
      qDebug() << "argument not 4 integers";
      return rc = 1;
    }
    intresult.resize(*(ptr+2) * *(ptr+3));
    int shp[2];
    if(!(rc=glqpixelm(ptr,shp,&intresult[0]))) {
      intresult.resize(shp[0]*shp[1]);
      resultshape[0]=shp[0];
      resultshape[1]=shp[1];
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
#endif
  } else {
    qDebug() << "glq..." << t << ncnt;
    exit(0);
    rc = 1;
  }
  return rc;
}

int glquery2(int t, int* ptr, int ncnt)
{
#if defined(GLPRINTER)
  int rc = 0;
  if(t==glzqresolution_n) {
    intresult.resize(1);
    intresult[0]=glzqresolution();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqcolormode_n) {
    intresult.resize(1);
    intresult[0]=glzqcolormode();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqduplexmode_n) {
    intresult.resize(1);
    intresult[0]=glzqduplexmode();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqorientation_n) {
    intresult.resize(1);
    intresult[0]=glzqorientation();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqoutputformat_n) {
    intresult.resize(1);
    intresult[0]=glzqoutputformat();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqpageorder_n) {
    intresult.resize(1);
    intresult[0]=glzqpageorder();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqpapersize_n) {
    intresult.resize(1);
    intresult[0]=glzqpapersize();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqpapersource_n) {
    intresult.resize(1);
    intresult[0]=glzqpapersource();
    resulttype = 1;
    rc = -1;
  } else if(t==glzqmargins_n) {
    if(1!=ncnt) {
      qDebug() << "argument not 1 integer";
      return rc = 1;
    }
    float f4[4];
    intresult.resize(4);
    if(!(rc=glzqmargins(f4,*ptr))) {
      intresult[0] = f4[0] * 1000.0;
      intresult[1] = f4[1] * 1000.0;
      intresult[2] = f4[2] * 1000.0;
      intresult[3] = f4[3] * 1000.0;
      resulttype = 1;
      rc = -1;
    } else intresult.clear();
  } else {
    qDebug() << "glq..." << t << ncnt;
    exit(0);
    rc = 1;
  }
  return rc;
#else
  Q_UNUSED(t);
  Q_UNUSED(ptr);
  Q_UNUSED(ncnt);
  return 0;
#endif
}
