#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <cmath>
#endif

#include <QPrinter>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef Max
#define Max(a, b) ((a) < (b) ? (b) : (a))
#endif

#include "wd.h"
#include "font.h"
#include "glz.h"
#include "prtobj.h"

extern QPrinter *Printer;
Prtobj *prtobj=new Prtobj();

static int glzfont0 (Prtobj *prtobj, char *s);

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
int glzqextent(char *s,int *wh)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (prtobj->font)->font );
  *(wh) = fm.width( QString::fromUtf8 (s));
  *(wh+1) = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glzqextentw(char *s,int *w)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  QStringList n=(QString::fromUtf8 (s)).split("\n",QString::SkipEmptyParts);
  QFontMetrics fm = QFontMetrics ( (prtobj->font)->font );
  for (int i=0; i<n.size(); i++) {
    *(w + i) = fm.width ( n.at(i));
  }
  return 0;
}

// ---------------------------------------------------------------------
// Height, Ascent, Descent, InternalLeading, ExternalLeading, AverageCharWidth, MaxCharWidth
int glzqtextmetrics(int *tm)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  QFontMetrics fm = QFontMetrics ( (prtobj->font)->font );
  *(tm) = fm.height();
  *(tm+1) = fm.ascent();
  *(tm+2) = fm.descent();
  *(tm+3) = fm.leading();
  *(tm+4) = 0;
  *(tm+5) = fm.averageCharWidth();
  *(tm+6) = fm.maxWidth();

  return 0;
}

/*
QPrinter::Millimeter   0
QPrinter::Point        1
QPrinter::Inch         2
QPrinter::Pica         3
QPrinter::Didot        4
QPrinter::Cicero       5
QPrinter::DevicePixel  6
*/

// ---------------------------------------------------------------------
int glzqwh(float *wh, int unit)
{
  if (!(wh && Printer)) return 1;
  QSizeF size = Printer->paperSize((QPrinter::Unit)unit);
  wh[0] = (float) size.width();
  wh[1] = (float) size.height();
  return 0;
}

// ---------------------------------------------------------------------
int glzarc (const int *p)
{
  int dy[2];
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  qtarcisi (p,  p + 4, dy);
  prtobj->painter->drawArc (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glzbrush ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->brushcolor = QColor (prtobj->color);
  prtobj->brush = QBrush (prtobj->brushcolor);
  prtobj->painter->setBrush(prtobj->brush);
  prtobj->brushnull = 0;
  return 0;
}

// ---------------------------------------------------------------------
int glzbrushnull ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->brushnull = 1;
  prtobj->painter->setBrush(Qt::NoBrush);
  return 0;
}

// ---------------------------------------------------------------------
int glzclear2 (void *p)
{
  Prtobj *prtobj = (Prtobj *) p;
  if ((!Printer) || !Printer->isValid()) return 1;
  if (!prtobj) return 1;
  prtobj->color = QColor (0, 0, 0);
  prtobj->orgx = 0;
  prtobj->orgy = 0;
  prtobj->clipped = 0;
  prtobj->textx = 0;
  prtobj->texty = 0;
  prtobj->brushnull = 1;

// messed up here, but ProFont should be part of config...
#ifdef _WIN32
  glzfont0(prtobj,(char *)"\"MS Sans Serif\" 10");
#else
#ifdef __MACH__
  glzfont0(prtobj,(char *)"\"Lucida Grande\" 10");
#else
  glzfont0(prtobj,(char *)"SansSerif 10");
#endif
#endif
  prtobj->pen = QPen (prtobj->color, 1); // TODO in user space
  prtobj->textpen = QPen (prtobj->pen);
  prtobj->brushcolor = QColor (255, 255, 255);
  prtobj->brush.setColor(prtobj->brushcolor);
  prtobj->textcolor = QColor(prtobj->color);

  if (!prtobj->painter) return 0;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->painter->setBrush(Qt::NoBrush);
  prtobj->painter->setWorldMatrixEnabled (true);
  prtobj->painter->translate (-prtobj->orgx, -prtobj->orgy);
  prtobj->painter->setClipping (false);

  return 0;
}

// ---------------------------------------------------------------------
int glzclear ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  return glzclear2 (prtobj);
}

// ---------------------------------------------------------------------
int glzclip (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->clipped = 1;
  prtobj->painter->setClipRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  prtobj->painter->setClipping (true);
  return 0;
}

// ---------------------------------------------------------------------
int glzclipreset ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  if (prtobj->clipped) {
    prtobj->painter->setClipping (false);
    prtobj->clipped = 0;
  }
  return 0;
}

// ---------------------------------------------------------------------
int glzellipse (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->painter->drawEllipse (*(p), *(p + 1),  *(p + 2),  *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
static int glzfont_i (const int *p, int len)
{
  char *face = int2utf8 (p, len);
  int r = glzfont (face);
  free (face);
  return r;
}

// ---------------------------------------------------------------------
static int glzfont0 (Prtobj *prtobj, char *s)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->font = new Font (string(s));
  prtobj->painter->setFont ((prtobj->font)->font);
  QFontMetrics fm = QFontMetrics ( (prtobj->font)->font );
  prtobj->fontheight = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glzfont (char *s)
{
  return glzfont0 (prtobj, s);
}

// ---------------------------------------------------------------------
int glzfont2 (const int *p, int len)
{
  int size10, degree10, bold, italic, strikeout, underline;
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  size10 = *(p);
  bold = 1 & *(p + 1);
  italic = 2 & *(p + 1);
  underline = 4 & *(p + 1);
  strikeout = 8 & *(p + 1);
  degree10 = *(p + 2);
  char *face = int2utf8 (p + 3, len - 3);
  prtobj->font = new Font (string(face), size10, !!bold, !!italic, !!strikeout, !!underline, degree10);
  prtobj->painter->setFont ((prtobj->font)->font);
  QFontMetrics fm = QFontMetrics ( (prtobj->font)->font );
  prtobj->fontheight = fm.height();
  return 0;
}

// ---------------------------------------------------------------------
int glzfontangle (int a)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->font->angle = a;
  return 0;
}

// ---------------------------------------------------------------------
int glzlines (const int *p, int len)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  prtobj->painter->drawPolyline (pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int glznodblbuf (int a)
{
// TODO
  Q_UNUSED(a);
  return 0;
}

// ---------------------------------------------------------------------
int glzpen (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->pencolor = QColor (prtobj->color);
  prtobj->pen = QPen (prtobj->pencolor, Max (0.5, *(p))); // TODO in user space
  prtobj->painter->setPen(prtobj->pen);
  return 0;
}

// ---------------------------------------------------------------------
int glzpie (const int *p)
{
  int dy[2];
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  qtarcisi (p, p + 4, dy);
  prtobj->painter->drawPie (*(p), *(p + 1), *(p + 2), *(p + 3), dy[0], dy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glzpixel (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->painter->drawPoint (*(p), *(p + 1));
  return 0;
}

// ---------------------------------------------------------------------
static int glzpixels2(int x,int y,int w,int h,const uchar *p)
{
  if (w==0||h==0) return 0;
  QImage image = QImage(w,h,QImage::Format_RGB32);
  const uchar *t=image.bits();
  memcpy((uchar *)t,p,4*w*h);

  prtobj->painter->drawImage(x, y, image);

  return 0;
}

// ---------------------------------------------------------------------
int glzpixels(const int *p, int len)
{
  Q_UNUSED(len);
  if ((!Printer) || !Printer->isValid()) return 1;

  return glzpixels2 (*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *)(p + 4));
}

// ---------------------------------------------------------------------
int glzpixelsx (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
#if defined(_WIN64)||defined(__LP64__)
  Q_UNUSED(p);
  return 1;
#else
  return glzpixels2 (*(p), *(p + 1), *(p + 2), *(p + 3), (uchar *) * (p + 4));
#endif
}

// ---------------------------------------------------------------------
int glzpolygon (const int *p, int len)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  int c = len / 2;
  if (0 == c) return 0;
  QPoint *pt= new QPoint[c];
  for (int i = 0; i < c; i++) pt[i] = QPoint (*(p + 2 * i), *(p + 1 + 2 * i));
  prtobj->painter->drawPolygon (pt,c);
  delete [] pt;
  return 0;
}

// ---------------------------------------------------------------------
int glzrect (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->painter->drawRect (*(p), *(p + 1), *(p + 2), *(p + 3));
  return 0;
}

// ---------------------------------------------------------------------
int glzrgb (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  prtobj->color = QColor (*(p), *(p + 1), *(p + 2));
  return 0;
}

// ---------------------------------------------------------------------
static int glztext_i (const int *p, int len)
{
  char *ys = int2utf8 ((p), len);
  int r = glztext (ys);
  free (ys);
  return r;
}

// ---------------------------------------------------------------------
int glztext (char *ys)
{
  Q_UNUSED(ys);
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->painter->setPen(prtobj->textpen);
  if (0 == prtobj->font->angle)
    prtobj->painter->drawText ( prtobj->textx, prtobj->texty + prtobj->fontheight, QString::fromUtf8 (ys));
  else {
    prtobj->painter->save ();
    prtobj->painter->translate ( prtobj->textx, prtobj->texty );
    prtobj->painter->rotate ( - prtobj->font->angle/10 );
    prtobj->painter->drawText ( 0, prtobj->fontheight, QString::fromUtf8 (ys));
    prtobj->painter->restore ();
  }
  prtobj->painter->setPen(prtobj->pen);
  return 0;
}

// ---------------------------------------------------------------------
int glztextcolor ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->textcolor = QColor (prtobj->color);
  prtobj->textpen = QPen (prtobj->painter->pen());
  prtobj->textpen.setColor (prtobj->textcolor);
  return 0;
}

// ---------------------------------------------------------------------
int glztextxy (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  prtobj->textx = *(p);
  prtobj->texty = *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int glzwindoworg (const int *p)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;
  prtobj->painter->translate (*(p), *(p + 1));
  prtobj->orgx += *(p);
  prtobj->orgy += *(p + 1);
  return 0;
}

// ---------------------------------------------------------------------
int glzcmds (const int *ptr, int ncnt)
{
  int cnt;
  int p = 0;
  int errcnt = 0;

  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (!prtobj->painter->isActive()) return 1;

  while (p < ncnt) {
    cnt = *(ptr + p);
    switch (*(ptr + p + 1)) {

    case 2001:		// glarc
      glzarc (ptr + p + 2);
      break;

    case 2004:		// glbrush
      glzbrush ();
      break;

    case 2005:		// glbrushnull
      glzbrushnull ();
      break;

    case 2007:		//glclear
      glzclear ();
      break;

    case 2078:		//glclip
      glzclip (ptr + p + 2);
      break;

    case 2079:		//glclipreset
      glzclipreset ();
      break;

    case 2008:		// glellipse
      glzellipse (ptr + p + 2);
      break;

    case 2012:		// glfont
      glzfont_i ((ptr + p + 2), cnt-2);
      break;

    case 2312:		// glfont2
      glzfont2 ((ptr + p + 2), cnt-2);
      break;

    case 2342:		//glfontangle
      glzfontangle (*(ptr + p + 2));
      break;

    case 2015:		// gllines
      glzlines ((ptr + p + 2), cnt-2);
      break;

    case 2070:		// glnodblbuf
      glznodblbuf (*(ptr + p + 2));
      break;

    case 2022:		// glpen
      glzpen (ptr + p + 2);
      break;

    case 2023:		// glpie
      glzpie (ptr + p + 2);
      break;

    case 2024:		// glpixel
      glzpixel (ptr + p + 2);
      break;

    case 2076:		// glpixels
      glzpixels ((ptr + p + 2), cnt-2);
      break;

    case 2075:		// glpixelsx
      glzpixelsx (ptr + p + 2);
      break;

    case 2029:		// glpolygon
      glzpolygon ((ptr + p + 2), cnt-2);
      break;

    case 2031:		// glrect
      glzrect (ptr + p + 2);
      break;

    case 2032:		// glrgb
      glzrgb (ptr + p + 2);
      break;

    case 2038:		// gltext
      glztext_i (ptr + p + 2, cnt-2);
      break;

    case 2040:		// gltextcolor
      glztextcolor ();
      break;

    case 2056:		// gltextxy
      glztextxy (ptr + p + 2);
      break;

    case 2045:		//glwindoworg
      glzwindoworg (ptr + p + 2);
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
int glzqresolution ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->resolution();
}

/*
enum ColorMode { Color, GrayScale }
enum DuplexMode { DuplexNone, DuplexAuto, DuplexLongSide, DuplexShortSide }
enum Orientation { Portrait, Landscape }
enum OutputFormat { NativeFormat, PdfFormat, PostScriptFormat }
enum PageOrder { FirstPageFirst, LastPageFirst }
enum PaperSize { A0, A1, A2, A3, ..., Custom }
enum PaperSource { Auto, Cassette, Envelope, EnvelopeManual, ..., SmallFormat }
*/

// ---------------------------------------------------------------------
int glzqcolormode ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->colorMode();
}

// ---------------------------------------------------------------------
int glzqduplexmode ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->duplex();
}

// ---------------------------------------------------------------------
int glzqorientation ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->orientation();
}

// ---------------------------------------------------------------------
int glzqoutputFormat ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->outputFormat();
}

// ---------------------------------------------------------------------
int glzqpageorder ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->pageOrder();
}

// ---------------------------------------------------------------------
int glzqpapersize ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->paperSize();
}

// ---------------------------------------------------------------------
int glzqpapersource ()
{
  if ((!Printer) || !Printer->isValid()) return 0;
  return Printer->paperSource();
}

// ---------------------------------------------------------------------
int glzresolution (int n)
{
  if ((!Printer) || !Printer->isValid()) return 0;
  Printer->setResolution(n);
  return Printer->resolution();
}

// ---------------------------------------------------------------------
int glzcolormode (int n)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->setColorMode((QPrinter::ColorMode)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzduplexmode (int n)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->setDuplex((QPrinter::DuplexMode)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzorientation (int n)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->setOrientation((QPrinter::Orientation)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzoutputFormat (int n)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->setOutputFormat((QPrinter::OutputFormat)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpageorder (int n)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->setPageOrder((QPrinter::PageOrder)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpapersize (int n)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->setPaperSize((QPrinter::PaperSize)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpapersource (int n)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->setPaperSource((QPrinter::PaperSource)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzscale (float *xy)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  if (xy) prtobj->painter->scale(xy[0],xy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glzabortdoc ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  Printer->abort();
  if ((!prtobj) || !prtobj->painter) return 0;
  prtobj->painter->end();
  delete prtobj->painter;
  prtobj->painter=0;
  Printer->setDocName("");
  return 0;
}

// ---------------------------------------------------------------------
int glzenddoc ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  prtobj->painter->end();
  delete prtobj->painter;
  prtobj->painter=0;
  Printer->setDocName("");
  return 0;
}

// ---------------------------------------------------------------------
int glznewpage ()
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if ((!prtobj) || !prtobj->painter) return 1;
  return !Printer->newPage();
}

// ---------------------------------------------------------------------
int glzprinter (char *printername)
{
  if (!printername) return 1;
  if (printername[0]!='_') {
    Printer->setPrinterName(s2q(printername));
    Printer->setOutputFormat(QPrinter::NativeFormat);
  } else {
    QString p=s2q(printername);
    if (p==s2q("_pdf")) Printer->setOutputFormat(QPrinter::PdfFormat);
    else if (p==s2q("_ps")) Printer->setOutputFormat(QPrinter::PostScriptFormat);
    else return 1;
  }
  return !Printer->isValid();
}

// ---------------------------------------------------------------------
int glzstartdoc (char *jobname, char *filename)
{
  if ((!Printer) || !Printer->isValid()) return 1;
  if (jobname) Printer->setDocName(s2q(jobname));
  if (filename) Printer->setOutputFileName(s2q(filename));
  if (prtobj->painter) delete prtobj->painter;
  prtobj->painter=new QPainter(Printer);
  if (!prtobj->painter) return 1;
  glzclear2 (prtobj);
  return 0;
}
