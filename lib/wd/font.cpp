#include <QApplication>
#include <QStringList>
#include <QString>
#include <QDebug>

#include "cmd.h"
#include "font.h"
#include "wd.h"
#include "../base/state.h"

Font *FontExtent=0;

// ---------------------------------------------------------------------
Font::Font(std::string s, float pointsize)
{
  error=false;
  angle=0;
  if (s=="fixfont") {
    font=config.Font;
    return;
  } else if (s=="profont") {
    font=app->font();
    return;
  }
  QString face, sizestyle;
  int bold=0,italic=0,strikeout=0,underline=0;
  float size=0;
  QStringList ss=qsplit(s);
  if (ss.length()>0) {
    face = ss[0];
    if (ss.length()>1) {
      for (int j = 1; j < ss.length(); j++) {
        if (ss[j]=="bold") bold = 1;
        else if (ss[j]=="italic") italic = 1;
        else if (ss[j]=="underline") underline = 1;
        else if (ss[j]=="strikeout") strikeout = 1;
        else if (ss[j].mid(0,5)=="angle") angle = c_strtoi(q2s(ss[j].mid(5)));
        else {
          size = (float) c_strtod(q2s(ss[j]));
          if (0==size) {
            error=true;
            break;
          };
        }
      }
    }
  }
  if (-1.0!=pointsize) size=pointsize;
//  qDebug() << "font: " + face + ",size=" + QString::number(size) + ",bold=" + QString::number(bold) + ",italic=" + QString::number(italic) + ",strikeout=" + QString::number(strikeout) + ",underline=" + QString::number(underline) + ",angle=" + QString::number(angle) ;
  font = QFont (face);
  if (0.0!=size) font.setPointSizeF((size>0.0)?size:-size);
  font.setBold(bold);
  font.setItalic(italic);
  font.setStrikeOut(strikeout);
  font.setUnderline(underline);
}

// ---------------------------------------------------------------------
Font::Font(std::string s,int size10, bool bold, bool italic, bool strikeout, bool underline, int angle10)
{
  error=false;
  angle=angle10;
  QString face = s2q(remquotes(s));
  font = QFont (face);
  font.setPointSizeF(size10/10.0);
  font.setBold(bold);
  font.setItalic(italic);
  font.setStrikeOut(strikeout);
  font.setUnderline(underline);
}
