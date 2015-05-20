
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

#include "base.h"
#include "dialog.h"
#include "note.h"
#include "plaintextedit.h"
#include "proj.h"
#include "state.h"

#ifndef QT_NO_PRINTER
QPrinter *Printer;
#endif

// ---------------------------------------------------------------------
QString dialogdirectory(QWidget *w,QString t,QString p)
{
  QFileDialog *d=new QFileDialog(w,t,p);
  d->setFileMode(QFileDialog::Directory);
  d->setOption(QFileDialog::ShowDirsOnly);
  if (d->exec())
    return d->selectedFiles().at(0);
  else
    return "";
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
  else if (t=="system")
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
#ifdef QT48
    QAbstractPrintDialog::PrintCurrentPage|
#endif
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
void dialogprint(QWidget *w,PlainTextEdit *d)
{
  QPrintDialog *dlg = new QPrintDialog(config.Printer, w);
  dlg->setOptions(
#ifdef QT48
    QAbstractPrintDialog::PrintCurrentPage|
#endif
    QAbstractPrintDialog::PrintSelection|
    QAbstractPrintDialog::PrintPageRange|
    QAbstractPrintDialog::PrintShowPageSize|
    QAbstractPrintDialog::PrintCollateCopies);
  dlg->setWindowTitle("Print Document");
  if (dlg->exec() != QDialog::Accepted)
    return;
  if (d) {
    QTextDocument *dd;
    dd=d->document()->clone();
#ifdef QT50
    dd->documentLayout()->setPaintDevice((QPagedPaintDevice *)config.Printer);
    dd->setPageSize(QSizeF(config.Printer->pageRect().size()));
    dd->print((QPagedPaintDevice *)config.Printer);
#else
    dd->documentLayout()->setPaintDevice(config.Printer);
    dd->setPageSize(QSizeF(config.Printer->pageRect().size()));
    dd->print(config.Printer);
#endif
    delete dd;
  }
  delete dlg;
  config.Printer->setPrintRange(QPrinter::AllPages);
}
// ---------------------------------------------------------------------
void dialogprintpreview(QWidget *w,PlainTextEdit *d)
{
  if (!d) return;
  QPrintPreviewDialog *dlg = new QPrintPreviewDialog(config.Printer, w);
  dlg->setWindowTitle("Preview Document");
  QObject::connect(dlg,SIGNAL(paintRequested(QPrinter *)),d,SLOT(print(QPrinter *)));
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
    r=cpath ("~Projects");
  else if (config.UserFolderKeys.contains("User"))
    r=cpath ("~User");
  else if (!config.UserFolderKeys.isEmpty())
    r= config.UserFolderValues.at(0);
  else
    r=config.UserPath.absolutePath();
  return r+"/";
}
