#define GLPRINTER
#include "gl2class.h"

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
  if (!wh) return 1;
  CHKPAINTER
  QSizeF size = Printer->paperSize((QPrinter::Unit)unit);
  wh[0] = (float) size.width();
  wh[1] = (float) size.height();
  return 0;
}

// ---------------------------------------------------------------------
int glzqmargins(float *ltrb, int unit)
{
  if (!ltrb) return 1;
  CHKPAINTER
  qreal l,t,r,b;
  Printer->getPageMargins(&l,&t,&r,&b,(QPrinter::Unit)unit);
  ltrb[0] = (float) l;
  ltrb[1] = (float) t;
  ltrb[2] = (float) r;
  ltrb[3] = (float) b;
  return 0;
}

// ---------------------------------------------------------------------
int glzqresolution ()
{
  CHKPAINTER
  return Printer->resolution();
}

/*
enum ColorMode { Color, GrayScale }
enum DuplexMode { DuplexNone, DuplexAuto, DuplexLongSide, DuplexShortSide }
enum Orientation { Portrait, Landscape }
#ifdef QT50
enum OutputFormat { NativeFormat, PdfFormat }
#else
enum OutputFormat { NativeFormat, PdfFormat, PostScriptFormat }
#endif
enum PageOrder { FirstPageFirst, LastPageFirst }
enum PaperSize { A0, A1, A2, A3, ..., Custom }
enum PaperSource { Auto, Cassette, Envelope, EnvelopeManual, ..., SmallFormat }
*/

// ---------------------------------------------------------------------
int glzqcolormode ()
{
  CHKPAINTER
  return Printer->colorMode();
}

// ---------------------------------------------------------------------
int glzqduplexmode ()
{
  CHKPAINTER
  return Printer->duplex();
}

// ---------------------------------------------------------------------
int glzqorientation ()
{
  CHKPAINTER
  return Printer->orientation();
}

// ---------------------------------------------------------------------
int glzqoutputFormat ()
{
  CHKPAINTER
  return Printer->outputFormat();
}

// ---------------------------------------------------------------------
int glzqpageorder ()
{
  CHKPAINTER
  return Printer->pageOrder();
}

// ---------------------------------------------------------------------
int glzqpapersize ()
{
  CHKPAINTER
  return Printer->paperSize();
}

// ---------------------------------------------------------------------
int glzqpapersource ()
{
  CHKPAINTER
  return Printer->paperSource();
}

// ---------------------------------------------------------------------
int glzresolution (int n)
{
  CHKPAINTER
  Printer->setResolution(n);
  return 0;
}

// ---------------------------------------------------------------------
int glzcolormode (int n)
{
  CHKPAINTER
  Printer->setColorMode((QPrinter::ColorMode)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzduplexmode (int n)
{
  CHKPAINTER
  Printer->setDuplex((QPrinter::DuplexMode)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzorientation (int n)
{
  CHKPAINTER
  Printer->setOrientation((QPrinter::Orientation)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzoutputFormat (int n)
{
  CHKPAINTER
  Printer->setOutputFormat((QPrinter::OutputFormat)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpageorder (int n)
{
  CHKPAINTER
  Printer->setPageOrder((QPrinter::PageOrder)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpapersize (int n)
{
  CHKPAINTER
  Printer->setPaperSize((QPrinter::PaperSize)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpapersource (int n)
{
  CHKPAINTER
  Printer->setPaperSource((QPrinter::PaperSource)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzscale (float *xy)
{
  CHKPAINTER
  if (xy) prtobj->painter->scale(xy[0],xy[1]);
  return 0;
}

// ---------------------------------------------------------------------
int glzabortdoc ()
{
  CHKPAINTER
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
  CHKPAINTER
  prtobj->painter->end();
  delete prtobj->painter;
  prtobj->painter=0;
  Printer->setDocName("");
  return 0;
}

// ---------------------------------------------------------------------
int glznewpage ()
{
  CHKPAINTER
  return !Printer->newPage();
}

// ---------------------------------------------------------------------
int glzprinter (char *printername)
{
  if (!printername) return 1;
  CHKPAINTER
  if (printername[0]!='_') {
    Printer->setPrinterName(s2q(printername));
    Printer->setOutputFormat(QPrinter::NativeFormat);
  } else {
    QString p=s2q(printername);
    if (p==s2q("_pdf")) Printer->setOutputFormat(QPrinter::PdfFormat);
#ifndef QT50
    else if (p==s2q("_ps")) Printer->setOutputFormat(QPrinter::PostScriptFormat);
#endif
    else return 1;
  }
  return !Printer->isValid();
}

// ---------------------------------------------------------------------
int glzstartdoc (char *jobname, char *filename)
{
  CHKPAINTER
  if (jobname) Printer->setDocName(s2q(jobname));
  if (filename) Printer->setOutputFileName(s2q(filename));
  if (prtobj->painter) delete prtobj->painter;
  prtobj->painter=new QPainter(Printer);
  if (!prtobj->painter) return 1;
  glzclear2 (prtobj,0);
  return 0;
}

// ---------------------------------------------------------------------
int glzinitprinter ()
{
  if (!Printer) Printer=new QPrinter(QPrinter::HighResolution);
  return (!Printer);
}
