// pdf.cpp
//
#include "cmd.h"
#include "form.h"
//#include "pane.h"
#include "wd.h"
#include "../base/state.h"

#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>

//QString pdfFilePath = "path_to_pdf_file"; // Set the file path for the PDF
//QPdfWriter writer(pdfFilePath);
//writer.setPageSize(QPageSize(QPageSize::Letter)); // Set the page size
//writer.setPageMargins(QMarginsF(0.75, 0.25, 0.75, 0.25), QPageLayout::Inch); // Set page margins
//writer.setPageOrientation(QPageLayout::Portrait); // Set page orientation
//doc.setDefaultStyleSheet("table {border: 1px solid black}"); // Set default styles
//doc.setHtml(htmlContent); // Set HTML content
//doc.drawContents(&painter); // Draw the contents to the PDF

//QPdfWriter writer("output.pdf");
//writer.setPageSize(QPageSize::A4);
//QPainter painter(&writer);
//painter.drawText(100, 100, "Hello, PDF!");
//painter.end();

//QPagedPaintDevice::PdfVersion QPdfWriter::pdfVersion() const
//Returns the PDF version for this writer. The default is PdfVersion_1_4.

//See also setPdfVersion().

extern int rc;

std::string pdf(std::string p);

static std::string pdfclose();
static std::string pdfnew();
static std::string pdfsave();
static std::string pdfset();

static QStringList arg;
static Cmd cmd;

static QString filename;
static QTextDocument *doc;
static QPainter painter;
static QPdfWriter *writer;

static void createPdf();

// ---------------------------------------------------------------------
std::string pdf(std::string p)
{
  if (p.size() == 0) {
    error("missing pdf command");
    return "";
  }

  cmd.init(p);
  std::string type=cmd.getid();

  if (type=="close") return pdfclose();
  if (type=="save") return pdfsave();

  arg=cmd.qsplits();

  if (arg.size() == 0) {
    error("missing pdf command parameter");
    return "";
  }

  //qDebug() << "type,arg" << s2q(type) << arg[0];

  if (type=="new") return pdfnew();
  if (type=="set") return pdfset();
  return "";
}

// ---------------------------------------------------------------------
// needed?
std::string pdfclose()
{
  filename="";
  return "";
}

// ---------------------------------------------------------------------
std::string pdfnew()
{
  if (arg.count() != 1) {
    error("expecting filename argument to pdf new");
    return "";
  }
  filename=arg[0];
  doc = new QTextDocument();
  writer = new QPdfWriter(filename);
  writer->setCreator("Unknown Soldier");
  writer->setPageSize(QPageSize(QPageSize::Letter));
  writer->setPageMargins(QMargins(0.75,0.25,0.75,0.25));
  writer->setPageOrientation(QPageLayout::Portrait);
  return "";
}

// ---------------------------------------------------------------------
std::string pdfsave()
{
  createPdf();
  QPainter painter(writer);
  //painter.translate(0,0);
  doc->drawContents(&painter);
  //painter.end();
  return "";
}

// ---------------------------------------------------------------------
std::string pdfset()
{
  //qDebug() << "setting" << arg[0];
  doc->setHtml(arg[0]);
  return "";
}

// ---------------------------------------------------------------------
void createPdf()
{
  qDebug() <<     "createPdf";
  filename = "/home/chris/temp/output.pdf";
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "could not open file for writing";
    return;
  }
  QPdfWriter pdfWriter(&file);
  pdfWriter.setPageSize(QPageSize(QPageSize::A4));

  QPainter painter(&pdfWriter);
  painter.setPen(Qt::black);


  QTextDocument doc;
  doc.setHtml("<h1>Hello, PDF!</h1><p>This is <b>bold</b> and <i>italic</i> text.</p>");
  doc.drawContents(&painter);

  painter.end();
}
