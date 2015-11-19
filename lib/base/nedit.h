#ifndef NEDIT_H
#define NEDIT_H

#include "bedit.h"

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
  ~Nedit();
  bool pendingReload;

public slots:

private:
  void init_comments();
  void keyPressEvent(QKeyEvent *);
};

#endif
