
// the syntax for messages is:
//   wd 'mb type buttons title message'
//
// type specifies the icon and default behaviour:
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
#include <QMessageBox>

#include "wd.h"
#include "cmd.h"

QString mbcolor();
QString mbdir();
QString mbfont();
QString mbinfo(QString);
QString mbmsg();
QString mbopen();
QString mbsave();

QString type;
QStringList arg;

QMessageBox::StandardButton getdefaultbutton();
QMessageBox::StandardButton getonebutton();
QMessageBox::StandardButtons getotherbuttons();
QString getname(int);

// ---------------------------------------------------------------------
QString mb(string p)
{
  arg=qsplit(p);
  if (arg.size()<1) return mbinfo("missing mb type");

  type=arg.first();
  arg.removeFirst();
  if (type=="color")
    return mbcolor();
  if (type=="dir")
    return mbdir();
  if (type=="font")
    return mbfont();
  if (type=="open")
    return mbopen();
  if (type=="save")
    return mbsave();
  if (type=="info"||type=="query"||type=="warn"||type=="critical")
    return mbmsg();
  return mbinfo("invalid mb type: " + type);
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
    return mbinfo ("Need title and message: "+arg.join(" "));
  }

  if (button1==-1) {
    button1=QMessageBox::Ok;
    if (type=="query")
      buttons=QMessageBox::Cancel;
  }
  buttons|=button1;

  if (type=="query") {
    r=QMessageBox::question(QApplication::focusWidget(),t,m,buttons,button1);
    return getname(r);
  }
  if (type=="critical")
    QMessageBox::critical(QApplication::focusWidget(),t,m,buttons,button1);
  else if (type=="info")
    QMessageBox::information(QApplication::focusWidget(),t,m,buttons,button1);
  else if (type=="warn")
    QMessageBox::warning(QApplication::focusWidget(),t,m,buttons,button1);
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
  c=QColorDialog::getColor(c,QApplication::focusWidget());
  if (!c.isValid()) return "";
  return s2q(i2s(c.red()) + " " + i2s(c.green()) + " " + i2s(c.blue()));
}

// ---------------------------------------------------------------------
QString mbdir()
{
  QString title,dir;
  if (arg.size()!=2)
    return mbinfo("dir needs title and directory");
  title=arg.at(0);
  dir=arg.at(1);
  return QFileDialog::getExistingDirectory(
           QApplication::focusWidget(),title,dir);
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
  font=QFontDialog::getFont(&ok,def,QApplication::focusWidget());
  if (!ok) return "";
  QString r;
  r="\"" + font.family() + "\" " + QString::number(font.pointSize());
  if (font.bold()) r+=" bold";
  if (font.italic()) r+=" italic";
  if (font.strikeOut()) r+=" strikeout";
  if (font.underline()) r+=" underline";
  return r;
}

// ---------------------------------------------------------------------
QString mbopen()
{
  QString title,dir,filter;
  if (arg.size()<2)
    return mbinfo("open needs title, directory, [filters]");
  title=arg.at(0);
  dir=arg.at(1);
  if (arg.size()==3)
    filter=arg.at(2);
  return QFileDialog::getOpenFileName(
           QApplication::focusWidget(),title,dir,filter);
}

// ---------------------------------------------------------------------
QString mbsave()
{
  QString title,dir,filter;
  if (arg.size()<2)
    return mbinfo("save needs title, directory, [filters]");
  title=arg.at(0);
  dir=arg.at(1);
  if (arg.size()==3)
    filter=arg.at(2);
  return QFileDialog::getSaveFileName(
           QApplication::focusWidget(),title,dir,filter);
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

// ---------------------------------------------------------------------
QString mbinfo(QString s)
{
  info("Message Box",s);
  return "";
}
