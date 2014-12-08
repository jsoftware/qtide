
// the syntax for messages is:
//   wd 'mb type buttons title message'
//
// type specifies the icon and default behaviour:
//  about
//  info      (default OK button)
//  warn      (default OK button)
//  critical  (default OK button)
//  query     (requires two or three buttons)
//
// if one button, there is no result,
// otherwise the result is the button name (ok, cancel, ...)
//
// buttons are from the set:
//  mb_ok
//  mb_cancel
//  mb_yes
//  mb_no
//  mb_save
//  mb_discard

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <QTextDocument>

#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#else
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "wd.h"
#include "cmd.h"
#include "form.h"
#include "../base/dialog.h"
#include "../base/state.h"

QString mb(string,string);
QString mbabout();
QString mbcolor();
QString mbdir();
QString mbfont();
QString mbinput();
QString mbmsg();
QString mbopen();
QString mbopen1();
QString mbprint(bool);
QString mbprintx(bool);
QString mbsave();

QString fixsep(QString s);

QMessageBox::StandardButton getdefaultbutton();
QMessageBox::StandardButton getonebutton();
QMessageBox::StandardButtons getotherbuttons();
QString getname(int);

static string type;
static QStringList arg;

// ---------------------------------------------------------------------
// c is type, p is parameter, possibly preceded by *
QString mb(string c,string p)
{
  type=c;
  if (type.size()==0) {
    error("missing mb type");
    return "";
  }

  arg=qsplit(p,true);

  if (type=="about")
    return mbabout();
  if (type=="color")
    return mbcolor();
  if (type=="dir")
    return mbdir();
  if (type=="font")
    return mbfont();
  if (type=="input")
    return mbinput();
  if (type=="open")
    return mbopen();
  if (type=="open1")
    return mbopen1();
  if (type=="print") {
    QString s=dlb(s2q(p));
    return mbprint('*'==s.at(0));
  }
  if (type=="printx") {
    QString s=dlb(s2q(p));
    return mbprintx('*'==s.at(0));
  }
  if (type=="save")
    return mbsave();
  if (type=="info"||type=="query"||type=="warn"||type=="critical")
    return mbmsg();
  error("invalid mb type: " + type);
  return "";
}

// ---------------------------------------------------------------------
QString mbmsg()
{
  int r;
  QString t,m;

  QMessageBox::StandardButton button1=getdefaultbutton();
  QMessageBox::StandardButtons buttons=getotherbuttons();

  if (arg.size()==1) {
    t="Message Box";
    m=arg.at(0);
  } else if (arg.size()==2) {
    t=arg.at(0);
    m=arg.at(1);
  } else {
    error("Need title and message: "+q2s(arg.join(" ")));
    return "";
  }

  if (button1==-1) {
    button1=QMessageBox::Ok;
    if (type=="query")
      buttons=QMessageBox::Cancel;
  }
  buttons|=button1;

  if (type=="query") {
    r=QMessageBox::question(getmbparent(),t,m,buttons,button1);
    return getname(r);
  }
  if (type=="critical")
    QMessageBox::critical(getmbparent(),t,m,buttons,button1);
  else if (type=="info")
    QMessageBox::information(getmbparent(),t,m,buttons,button1);
  else if (type=="warn")
    QMessageBox::warning(getmbparent(),t,m,buttons,button1);
  return "";
}

// ---------------------------------------------------------------------
QString mbabout()
{
  if (arg.size()!=2) {
    error("about needs title and text");
    return "";
  }
  QMessageBox::about(getmbparent(),arg.at(0),arg.at(1));
  return "";

}

// ---------------------------------------------------------------------
QString mbcolor()
{
  QColor c;
  int r,g,b;

  if (arg.size()==3) {
    r=c_strtoi(q2s(arg.at(0)));
    g=c_strtoi(q2s(arg.at(1)));
    b=c_strtoi(q2s(arg.at(2)));
    c=QColor(r,g,b);
  } else
    c=Qt::white;
  c=QColorDialog::getColor(c,getmbparent());
  if (!c.isValid()) return "";
  return s2q(i2s(c.red()) + " " + i2s(c.green()) + " " + i2s(c.blue()));
}

// ---------------------------------------------------------------------
QString mbdir()
{
  QString title,dir,fl;
  if (arg.size()!=2) {
    error("dir needs title and directory");
    return "";
  }
  title=arg.at(0);
  dir=arg.at(1);
  fl=QFileDialog::getExistingDirectory(
       getmbparent(),title,dir);
  return fl;
}

// ---------------------------------------------------------------------
QString mbfont()
{
  bool ok;
  QFont font, def;
  QString s;
  def.setStrikeOut(false);
  def.setUnderline(false);
  if (arg.size())
    def.setFamily(arg.at(0));
  if (arg.size()>1)
    def.setPointSize(arg.at(1).toFloat());
  for (int i=2; i<arg.size(); i++) {
    s=arg.at(i);
    if (s=="bold")
      def.setBold(true);
    if (s=="italic")
      def.setItalic(true);
    if (s=="strikeout")
      def.setStrikeOut(true);
    if (s=="underline")
      def.setUnderline(true);
  }
#ifdef __MACH__
  font=QFontDialog::getFont(&ok,def,getmbparent(),QString(), QFontDialog::DontUseNativeDialog);
#else
  font=QFontDialog::getFont(&ok,def,getmbparent());
#endif
  if (!ok) return "";
  return fontspec(font);
}

// ---------------------------------------------------------------------
QString mbinput()
{
  QString type,title,label,text;
  QWidget *w=getmbparent();
  QString r;
  bool ok=true;
  int s=arg.size();
  if (s<3) {
    error("input needs at least: type title label");
    return "";
  }
  type=arg.at(0);
  title=arg.at(1);
  label=arg.at(2);

// mb input double title label value min max decimals
  if (type=="double") {
    if (s != 7) {
      error("input double needs 6 parameters");
      return "";
    }
    double dval=arg.at(3).toDouble();
    double dmin=arg.at(4).toDouble();
    double dmax=arg.at(5).toDouble();
    int ddec=arg.at(6).toInt();
    r= QString::number(QInputDialog::getDouble(w, title, label, dval, dmin, dmax, ddec,&ok));
  }

// mb input int title label value min max step
  else if (type=="int") {
    if (s != 7) {
      error("input int needs 6 parameters");
      return "";
    }
    int ival=arg.at(3).toInt();
    int imin=arg.at(4).toInt();
    int imax=arg.at(5).toInt();
    int istep=arg.at(6).toInt();
    r=QString::number(QInputDialog::getInt(w, title, label, ival,imin,imax,istep,&ok));
  }

//mb input item title label index ifeditable items
  else if (type=="item") {
    if (s != 6) {
      error("input item needs 5 parameters");
      return "";
    }
    int index=arg.at(3).toInt();
    bool ifed=0 != arg.at(4).toInt();
    QStringList items=qsplit(q2s(arg.at(5)));
    r=QInputDialog::getItem(w, title, label, items, index, ifed, &ok);
  }

// mb input text title label value
  else if (type=="text") {
    if (s==3)
      text="";
    else  if (s==4)
      text=arg.at(3);
    else {
      error("input text needs 3 o4 4 parameters");
      return "";
    }
    r= QInputDialog::getText(w, title, label, QLineEdit::Normal, text,&ok);
  }

  else {
    error(q2s("unsupported input type: " + type));
    return "";
  }

  return ok ? r : "";
}

// ---------------------------------------------------------------------
QString mbopen()
{
  QString title,dir,filter;
  QStringList fl;
  if (arg.size()<2) {
    error("open needs title, directory, [filters]");
    return "";
  }
  title=arg.at(0);
  dir=arg.at(1);
  if (arg.size()==3)
    filter=fixsep(arg.at(2));
  fl=QFileDialog::getOpenFileNames(
       getmbparent(),title,dir,filter);
  if (fl.isEmpty())
    return "";
  else return fl.join("\012") + "\012";
}

// ---------------------------------------------------------------------
QString mbopen1()
{
  QString title,dir,filter,fl;
  if (arg.size()<2) {
    error("open1 needs title, directory, [filters]");
    return "";
  }
  title=arg.at(0);
  dir=arg.at(1);
  if (arg.size()==3)
    filter=fixsep(arg.at(2));
  fl=QFileDialog::getOpenFileName(
       getmbparent(),title,dir,filter);
  return fl;
}

// ---------------------------------------------------------------------
QString mbprint(bool iftext)
{
#ifdef QT_NO_PRINTER
  Q_UNUSED(iftext);
  return "";
#else
  QString r="";
  if (arg.size()) {
    QTextDocument *d=0;
    QString s=arg.at(0);
    if (!iftext) {
      if (!cfexist(s)) {
        error("File not found: " + q2s(s));
        return "";
      }
      s=cfread(s);
    }
    d=new QTextDocument(s);
    d->setDefaultFont(config.Font);

#ifdef __MACH__
    QSysInfo qsi;
    if (qsi.MacintoshVersion < QSysInfo::MV_10_7) {
      if (!config.Printer->isValid()) {
        error("Invalid printer: " + q2s(config.Printer->printerName()));
        delete d;
        return "";
      }
      d->print(config.Printer);
      delete d;
      return "";
    }
#endif
    dialogprint(getmbparent(),d);
    delete d;
  } else {
#ifdef __MACH__
    QSysInfo qsi;
    if (qsi.MacintoshVersion < QSysInfo::MV_10_7) {
      r=config.Printer->printerName();
      return r;
    }
#endif
    QPrintDialog *dlg = new QPrintDialog(config.Printer);
    if (dlg->exec() == QDialog::Accepted) {
      switch (config.Printer->outputFormat()) {
      case QPrinter::PdfFormat :
        r="_pdf:" + config.Printer->outputFileName();
        break;
#ifndef QT50
      case QPrinter::PostScriptFormat :
        r="_ps:" + config.Printer->outputFileName();
        break;
#endif
      default :
        r=config.Printer->printerName();
        break;
      }
    }
    delete dlg;
  }
  return r;
#endif
}

// ---------------------------------------------------------------------
// print with no dialog
QString mbprintx(bool iftext)
{
#ifdef QT_NO_PRINTER
  Q_UNUSED(iftext);
#else
  if (arg.size()==0) {
    error("No text given for printx");
    return "";
  }
  QString s=arg.at(0);
  if (!iftext) {
    if (!cfexist(s)) {
      error("File not found: " + q2s(s));
      return "";
    }
    s=cfread(s);
  }
  QTextDocument *d=new QTextDocument(s);
  d->setDefaultFont(config.Font);
  if (!config.Printer->isValid()) {
    error("Invalid printer: " + q2s(config.Printer->printerName()));
    return "";
  }
  d->print(config.Printer);
#endif
  return "";
}

// ---------------------------------------------------------------------
QString mbsave()
{
  QString title,dir,filter,fl;
  if (arg.size()<2) {
    error("save needs title, directory, [filters]");
    return "";
  }
  title=arg.at(0);
  dir=arg.at(1);
  if (arg.size()==3)
    filter=fixsep(arg.at(2));
  fl=QFileDialog::getSaveFileName(
       getmbparent(),title,dir,filter);
  return fl;
}

// ---------------------------------------------------------------------
QString fixsep(QString s)
{
  return s.replace("|",";;");
}

// ---------------------------------------------------------------------
QString getname(int b)
{
  if (b==QMessageBox::Ok)
    return "ok";
  if (b==QMessageBox::Cancel)
    return "cancel";
  if (b==QMessageBox::Yes)
    return "yes";
  if (b==QMessageBox::No)
    return "no";
  if (b==QMessageBox::Save)
    return "save";
  if (b==QMessageBox::Discard)
    return "discard";
  return "unknown button";
}

// ---------------------------------------------------------------------
QMessageBox::StandardButton getonebutton()
{
  if (arg.isEmpty()) return QMessageBox::NoButton;
  QString s=arg.first();
  if (s=="mb_ok")
    return QMessageBox::Ok;
  if (s=="mb_cancel")
    return QMessageBox::Cancel;
  if (s=="mb_yes")
    return QMessageBox::Yes;
  if (s=="mb_no")
    return QMessageBox::No;
  if (s=="mb_save")
    return QMessageBox::Save;
  if (s=="mb_discard")
    return QMessageBox::Discard;
  return QMessageBox::NoButton;
}

// ---------------------------------------------------------------------
QMessageBox::StandardButton getdefaultbutton()
{
  QMessageBox::StandardButton r=getonebutton();
  if (r!=QMessageBox::NoButton)
    arg.removeFirst();
  return r;
}

// ---------------------------------------------------------------------
QMessageBox::StandardButtons getotherbuttons()
{
  QMessageBox::StandardButtons r=QMessageBox::NoButton;
  QMessageBox::StandardButton b;
  while (arg.size()) {
    b=getonebutton();
    if (b==QMessageBox::NoButton)
      return r;
    r|=b;
    arg.removeFirst();
  }
  return r;
}

