#ifndef NEDIT_H
#define NEDIT_H

#include "bedit.h"

class High;
class QFile;

class Nedit : public Bedit

{
  Q_OBJECT

public:
  Nedit();
  QString getcomment();
  QFile *file;
  QString fname;
  bool saved;
  QString sname;
  QString text;

public slots:

private:
  void keyPressEvent(QKeyEvent *);
  void init_comments();
  High *high;
};

#endif
