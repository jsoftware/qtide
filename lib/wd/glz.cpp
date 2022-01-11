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
  CHKPAINTER2
  QRectF size = Printer->pageRect((QPrinter::Unit)unit);
  wh[0] = (float) size.width();
  wh[1] = (float) size.height();
  return 0;
}

// ---------------------------------------------------------------------
int glzqmargins(float *ltrb, int unit)
{
  if (!ltrb) return 1;
  CHKPAINTER2
  QMarginsF m=Printer->pageLayout().margins((QPageLayout::Unit)unit);
  ltrb[0] = (float) m.left();
  ltrb[1] = (float) m.top();
  ltrb[2] = (float) m.right();
  ltrb[3] = (float) m.bottom();
  return 0;
}

// ---------------------------------------------------------------------
int glzqresolution ()
{
  CHKPAINTER2
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
  CHKPAINTER2
  return Printer->colorMode();
}

// ---------------------------------------------------------------------
int glzqduplexmode ()
{
  CHKPAINTER2
  return Printer->duplex();
}

// ---------------------------------------------------------------------
int glzqorientation ()
{
  CHKPAINTER2
  return Printer->pageLayout().orientation();
}

// ---------------------------------------------------------------------
int glzqoutputFormat ()
{
  CHKPAINTER2
  return Printer->outputFormat();
}

// ---------------------------------------------------------------------
int glzqpageorder ()
{
  CHKPAINTER2
  return Printer->pageOrder();
}

// ---------------------------------------------------------------------
int glzqpapersize ()
{
  CHKPAINTER2
  return Printer->pageLayout().pageSize().id();
}

// ---------------------------------------------------------------------
int glzqpapersource ()
{
  CHKPAINTER2
  return Printer->paperSource();
}

// ---------------------------------------------------------------------
int glzresolution (int n)
{
  CHKPAINTER2
  Printer->setResolution(n);
  return 0;
}

// ---------------------------------------------------------------------
int glzcolormode (int n)
{
  CHKPAINTER2
  Printer->setColorMode((QPrinter::ColorMode)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzduplexmode (int n)
{
  CHKPAINTER2
  Printer->setDuplex((QPrinter::DuplexMode)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzorientation (int n)
{
  CHKPAINTER2
  Printer->setPageOrientation((QPageLayout::Orientation)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzoutputFormat (int n)
{
  CHKPAINTER2
  Printer->setOutputFormat((QPrinter::OutputFormat)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpageorder (int n)
{
  CHKPAINTER2
  Printer->setPageOrder((QPrinter::PageOrder)n);
  return 0;
}

// ---------------------------------------------------------------------
int glzpapersize (int n)
{
  CHKPAINTER2
  Printer->setPageSize(QPageSize((QPageSize::PageSizeId)n));
  return 0;
}

// ---------------------------------------------------------------------
int glzpapersource (int n)
{
  CHKPAINTER2
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
  CHKPAINTER2
  Printer->abort();
  if ((!prtobj) || !prtobj->painter) return 0;
  if (prtobj->painter) {
    prtobj->painter->end();
    delete prtobj->painter;
    prtobj->painter=0;
  }
  Printer->setDocName("");
  return 0;
}

// ---------------------------------------------------------------------
int glzenddoc ()
{
  CHKPAINTER2
  if ((!prtobj) || !prtobj->painter) return 0;
  if (prtobj->painter) {
    prtobj->painter->end();
    delete prtobj->painter;
    prtobj->painter=0;
  }
  Printer->setDocName("");
  return 0;
}

// ---------------------------------------------------------------------
int glznewpage ()
{
  CHKPAINTER
  if (Printer->newPage()) {
    glzclear2 (prtobj,0);
    return 0;
  } else return 1;
}

// ---------------------------------------------------------------------
int glzprinter (char *printername)
{
  if (!printername) return 1;
  CHKPAINTER2
  if (prtobj->painter) {
    prtobj->painter->end();
    delete prtobj->painter;
    prtobj->painter=0;
  }
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
  CHKPAINTER2
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
  Printer=new QPrinter(QPrinter::HighResolution);
  return !Printer;
}
