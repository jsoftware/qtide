
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#else
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#endif
#endif
#include <QTextDocument>
#ifdef QT68
#include <QStyleHints>  // Detect dark/light style
#include <QToolBar> // Override QT legacy icons with Phosphor Icons
#endif

#include "base.h"
#include "dialog.h"
#include "note.h"
#include "proj.h"
#include "state.h"
#include "svr.h"
#include "bedit.h"
#include "tedit.h"

#ifndef QT_NO_PRINTER
QPrinter *Printer;
#endif

// ---------------------------------------------------------------------
QString dialogdirectory(QWidget *w,QString t,QString p)
{
  QFileDialog *d=new QFileDialog(w,t,p);
  d->setFileMode(QFileDialog::Directory);
  d->setOption(QFileDialog::ShowDirsOnly);
#ifndef NMDIALOG
  if (d->exec())
    return d->selectedFiles().at(0);
  else
    return "";
#else
  d->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
  d->show();
  return "";
#endif
}

// ---------------------------------------------------------------------
void dialogfont(QWidget *w)
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok,config.Font,w);
  if (font==config.Font) return;
  config.Font=font;
  if (ok) fontset(font);
}

// ---------------------------------------------------------------------
QString dialogfileopen(QWidget *w,QString t)
{
  QString d;
  if (t=="open")
    d=getfilepath();
  else if (t=="addons")
    d=config.AddonsPath.absolutePath();
  else if (t=="current")
    d=jcon->cmdr("1!:43''");
  else if (t=="home")
    d=config.HomePath.absolutePath();
  else if (t=="last") {
    d=cfpath(config.LastFileOpen);
    if (d.isEmpty())
      d=config.TempPath.absolutePath();
  } else if (t=="system")
    d=config.SystemPath.absolutePath();
  else if (t=="temp")
    d=config.TempPath.absolutePath();
  else if (t=="user")
    d=config.UserPath.absolutePath();
  else if (config.AppName=="jqt")
    d=config.SystemPath.absolutePath();
  else
    d=cpath("~qhome");
  return QFileDialog::getOpenFileName(w,t,d,config.FilePatterns);
}

#ifndef QT_NO_PRINTDIALOG
// ---------------------------------------------------------------------
void dialogprint(QWidget *w,QTextDocument *d)
{
  QPrintDialog *dlg = new QPrintDialog(config.Printer, w);
  dlg->setOptions(
    QAbstractPrintDialog::PrintCurrentPage|
    QAbstractPrintDialog::PrintSelection|
    QAbstractPrintDialog::PrintPageRange|
    QAbstractPrintDialog::PrintShowPageSize|
    QAbstractPrintDialog::PrintCollateCopies);
  dlg->setWindowTitle("Print Document");
  if (dlg->exec() != QDialog::Accepted)
    return;
  if (d) {
#ifdef QT50
    d->print((QPagedPaintDevice *)config.Printer);
#else
    d->print(config.Printer);
#endif
  }
  delete dlg;
  config.Printer->setPrintRange(QPrinter::AllPages);
}

// ---------------------------------------------------------------------
void dialogprint(QWidget *w,QWidget *d)
{
  QPrintDialog *dlg = new QPrintDialog(config.Printer, w);
  dlg->setOptions(
    QAbstractPrintDialog::PrintCurrentPage|
    QAbstractPrintDialog::PrintSelection|
    QAbstractPrintDialog::PrintPageRange|
    QAbstractPrintDialog::PrintShowPageSize|
    QAbstractPrintDialog::PrintCollateCopies);
  dlg->setWindowTitle("Print Document");
  if (dlg->exec() != QDialog::Accepted)
    return;
  if (d) {
    if (d==tedit) {
      if (config.Printer->printRange()==(QPrinter::Selection))
        tedit->print(config.Printer);
      else
        tedit->printPreview(config.Printer);
    } else {
      PlainTextEdit *p=(PlainTextEdit *)d;
      if (config.Printer->printRange()==(QPrinter::Selection))
        p->print(config.Printer);
      else
        p->printPreview(config.Printer);
    }
  }

  delete dlg;
  config.Printer->setPrintRange(QPrinter::AllPages);
}

// ---------------------------------------------------------------------
void dialogprintpreview(QWidget *w,QWidget *d)
{
  if (!d) return;
  QPrintPreviewDialog *dlg = new QPrintPreviewDialog(config.Printer, w);
  dlg->setWindowTitle("Preview Document");

#ifdef QT68
  QString actionIconText;
  QString iconResourceCoreName;
  QString styleSuffix;
  QList<QToolBar *> toolbarlist = dlg->findChildren<QToolBar *>();
  if(!toolbarlist.isEmpty()) { // Ensure QT dialog has expected toolbar
    // Define colors depending on dark/light style
    if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
      styleSuffix = "dark";
#if defined(_WIN32)
      toolbarlist.first()->setStyleSheet(
        "QToolButton:checked, QToolButton:selected {"
        "background-color: #555555;"
        "border: 1px solid #555555;"
        "border-radius: 6px;"
        "}"
      );
#endif
    } else {
      styleSuffix = "light";
#if defined(_WIN32)
      toolbarlist.first()->setStyleSheet(
        "QToolButton:checked, QToolButton:selected {"
        "background-color: #cccccc;"
        "border: 1px solid #cccccc;"
        "border-radius: 6px;"
        "}"
      );
#endif
    }
    toolbarlist.first()->setIconSize(QSize(24,24));
    for (auto actionHolder : toolbarlist.first()->actions()) {
      // Override QT legacy icons after checking they exist as expected
      actionIconText = actionHolder->iconText();
      if (actionIconText == "Fit width") {
        iconResourceCoreName = "arrows-horizontal";
      } else if (actionIconText == "Fit page") {
        iconResourceCoreName = "arrows-out";
      } else if (actionIconText == "Zoom out") {
        iconResourceCoreName = "magnifying-glass-minus";
      } else if (actionIconText == "Zoom in") {
        iconResourceCoreName = "magnifying-glass-plus";
      } else if (actionIconText == "Portrait") {
        iconResourceCoreName = "identification-badge";
      } else if (actionIconText == "Landscape") {
        iconResourceCoreName = "image";
      } else if (actionIconText == "First page") {
        iconResourceCoreName = "caret-double-left";
      } else if (actionIconText == "Previous page") {
        iconResourceCoreName = "caret-left";
      } else if (actionIconText == "Next page") {
        iconResourceCoreName = "caret-right";
      } else if (actionIconText == "Last page") {
        iconResourceCoreName = "caret-double-right";
      } else if (actionIconText == "Show single page") {
        iconResourceCoreName = "file-text";
      } else if (actionIconText == "Show facing pages") {
        iconResourceCoreName = "book-open-text";
      } else if (actionIconText == "Show overview of all pages") {
        iconResourceCoreName = "squares-four";
      } else if (actionIconText == "Page setup") {
        iconResourceCoreName = "faders-horizontal";
      } else if (actionIconText == "Print") {
        iconResourceCoreName = "printer";
      }
      actionHolder->setIcon(QIcon(":/images/" + iconResourceCoreName + "-bold-" + styleSuffix + ".svg"));
    }
  }
#endif
  if (d==tedit)
    QObject::connect(dlg,SIGNAL(paintRequested(QPrinter *)),((TextEdit *)d),SLOT(printPreview(QPrinter *)));
  else
    QObject::connect(dlg,SIGNAL(paintRequested(QPrinter *)),((PlainTextEdit *)d),SLOT(printPreview(QPrinter *)));
  dlg->exec();
  delete dlg;
  config.Printer->setPrintRange(QPrinter::AllPages);
}
#endif

// ---------------------------------------------------------------------
QString dialogsaveas(QWidget *w,QString t,QString p)
{
  return QFileDialog::getSaveFileName(w,t,p,config.FilePatterns);
}

// ---------------------------------------------------------------------
QString getfilepath()
{
  if (project.Id.isEmpty())
    return config.TempPath.absolutePath();
  else
    return project.Path;
};

// ---------------------------------------------------------------------
QString getprojectpath()
{
  if (!project.Id.isEmpty())
    return project.Path;
  QString r;
  if (config.UserFolderKeys.contains("Projects"))
    r=cpath("~Projects");
  else if (config.UserFolderKeys.contains("User"))
    r=cpath("~User");
  else if (!config.UserFolderKeys.isEmpty())
    r= config.UserFolderValues.at(0);
  else
    r=config.UserPath.absolutePath();
  return r+"/";
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void printpreview(QPrinter * printer, QTextDocument *d)
{
#ifdef QT50
  d->documentLayout()->setPaintDevice((QPagedPaintDevice *)printer);
  d->setPageSize(QSizeF(printer->pageRect(QPrinter::DevicePixel).size()));
  d->print((QPagedPaintDevice *)printer);
#else
  d->documentLayout()->setPaintDevice(printer);
  d->setPageSize(QSizeF(printer->pageRect().size()));
  d->print(printer);
#endif
}
#endif
