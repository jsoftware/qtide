#ifndef STYLE_H
#define STYLE_H

#include <QColor>
#include <QFont>

class Style
{
public:
  Style();

  void set(QString);
  QString read();

  QColor color;
  bool italic;
  QFont::Weight weight;

};
#endif
