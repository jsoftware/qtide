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
  void append_smoutput(QString s);
  QString getprompt();
  void insert(QString s);
  void removeprompt();
  void setprompt();
  void setresized(int);
  void promptreplace(QString t);

  QScrollBar *hScroll;
  int ifResized, Tw, Th;
  QString prompt;

public slots:
  void docmdp(QString t, bool show, bool same);
  void docmds(QString t, bool show);
  void docmdx(QString t);
  void itemActivated(QListWidgetItem *);
  void loadscript(QString s,bool show);
  void runall(QString s);

private slots:
#ifdef QT_OS_ANDROID
  void backButtonTimer();
#endif

private:
  void docmd(QString t);
  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);
  void enter();
#ifdef QT_OS_ANDROID
  int backButtonPressed;
#endif
  void togglemode();
};

extern Tedit *tedit;

#endif
