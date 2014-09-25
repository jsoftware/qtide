#ifndef FONT_H
#define FONT_H

#include <string>
#include <QFont>

using namespace std;

// ---------------------------------------------------------------------
class Font : public QObject
{
  Q_OBJECT

public:
  Font(string s, float pointsize=-1.0);
  Font(string s,int size10, bool bold, bool italic, bool strikeout, bool underline, int angle10);
  QFont font;
  int angle;

private:

};

extern Font *FontExtent;

#endif
