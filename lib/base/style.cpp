/* font styles */

#include <QApplication>
#include <QSettings>
#include <QTemporaryFile>

#include "base.h"
#include "state.h"

// ---------------------------------------------------------------------
// reads and updates style.cfg file
void Config::initstyle()
{
  QString f=ConfigPath.filePath("style.cfg");
  QSettings *s=new QSettings(f,QSettings::IniFormat);
  QString h,t,w;

  EditFore.set(s->value("Edit/fore","0 0 0").toString());
  EditBack.set(s->value("Edit/back","255 255 255").toString());
  EditHigh.set(s->value("Edit/high","240 240 232").toString());

  TermFore.set(s->value("Term/fore","0 0 0").toString());
  TermBack.set(s->value("Term/back","255 255 255").toString());
  TermHigh.set(s->value("Term/high","240 240 232").toString());

  QString def=TermFore.read();

  adverbStyle.set(s->value("Class/adverb",def).toString());
  commentStyle.set(s->value("Class/comment",def).toString());
  //CommentMulti.set(s->value("Class/",def).toString());
  conjunctionStyle.set(s->value("Class/conjunction",def).toString());
  controlStyle.set(s->value("Class/control",def).toString());
  functionStyle.set(s->value("Class/function",def).toString());
  nounStyle.set(s->value("Class/noun",def).toString());
  noundefStyle.set(s->value("Class/noundef",def).toString());
  numberStyle.set(s->value("Class/number",def).toString());
  stringStyle.set(s->value("Class/string",def).toString());
  verbStyle.set(s->value("Class/verb",def).toString());

  //if (s->allKeys().contains("Edit/high")) return;

  delete s;
  QTemporaryFile temp;
  temp.open();
  s=new QSettings(temp.fileName(),QSettings::IniFormat);
  s->setValue("Edit/fore",EditFore.read());
  s->setValue("Edit/back",EditBack.read());
  s->setValue("Edit/high",EditHigh.read());
  s->setValue("Term/fore",TermFore.read());
  s->setValue("Term/back",TermBack.read());
  s->setValue("Term/high",TermHigh.read());

  s->setValue("Class/adverb",adverbStyle.read());
  s->setValue("Class/comment",commentStyle.read());
  s->setValue("Class/conjunction",conjunctionStyle.read());
  s->setValue("Class/control",controlStyle.read());
  s->setValue("Class/function",functionStyle.read());
  s->setValue("Class/noun",nounStyle.read());
  s->setValue("Class/noundef",noundefStyle.read());
  s->setValue("Class/number",numberStyle.read());
  s->setValue("Class/string",stringStyle.read());
  s->setValue("Class/verb",verbStyle.read());

  s->sync();
  t=cfread(temp.fileName());
  h="# Qt Styles config\n"
    "# This file is read and written by the Qt IDE.\n"
    "# Make changes in the same format as the original.\n"
    "# \n"
    "# Each style has rgb color values (0-255), optionally\n"
    "# followed by bold or italic, e.g.\n"
    "#  verb=0 128 128 bold\n"
    "#\n"
    "# Edit/Term definitions are:\n"
    "#  fore = text color\n"
    "#  back = default background color\n"
    "#  high = selected background color\n"
    ;
  cfwrite(f,h + "\n" + t);
}

// ---------------------------------------------------------------------
Style::Style()
{
  color=QColor(0,0,0);

  italic=false;
  weight=QFont::Normal;
}

// ---------------------------------------------------------------------
QString Style::read()
{
  QString r=QString::number(color.red()) + " "
            + QString::number(color.green()) + " "
            + QString::number(color.blue());

  if (weight==QFont::Bold)
    r+=" bold";

  if (italic)
    r+=" italic";
  return r;
}

// ---------------------------------------------------------------------
void Style::set(QString s)
{
  QStringList p=s.toLower().split(" ",QString::SkipEmptyParts);

  if (p.contains("bold")) {
    weight=QFont::Bold;
    p.removeAll("bold");
  } else
    weight=QFont::Normal;

  italic=p.contains("italic");
  p.removeAll("italic");

  if (p.size()!=3) {
    info ("Style","Invalid style setting: " + s);
    return;
  }
  color=QColor(p.at(0).toInt(),p.at(1).toInt(),p.at(2).toInt());
}
