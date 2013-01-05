#ifndef TEDIT_H
#define TEDIT_H

#include <QListWidgetItem>
#include "bedit.h"

class Tedit : public Bedit
{
  Q_OBJECT

public:
  Tedit();

  void append(QString s);
  QString getprompt();
  void insert(QString s);
  void setprompt();
  void setresized(int);
  void promptreplace(QString t);

  QScrollBar *hScroll;
  int ifResized, Tw, Th;
  QString prompt;

public slots:
  void docmdp(QString t, bool show);
  void docmds(QString t);
  void docmdx(QString t);
  void itemActivated(QListWidgetItem *);
  void loadscript(QString s,bool show);

private:
  void docmd(QString t);
  void keyPressEvent(QKeyEvent *);
  void enter();
  void togglemode();
};

extern Tedit *tedit;

#endif
