
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>

#include "base.h"
#include "dialog.h"
#include "note.h"
#include "proj.h"
#include "state.h"
#include "tedit.h"

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
  QFont font = QFontDialog::getFont(&ok,config.Font, w);
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

// ---------------------------------------------------------------------
void dialogprint(QWidget *w,QTextDocument *d)
{
  QPrinter printer(QPrinter::HighResolution);
  QPrintDialog *dlg = new QPrintDialog(&printer, w);
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
  d->print(&printer);
  delete dlg;
}

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
  if (project.Id.isEmpty())
    return config.UserPath.absolutePath();
  else
    return project.Path;
};
