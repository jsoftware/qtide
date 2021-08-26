#ifndef EVIEW_H
#define EVIEW_H

#include <QDialog>
#include "plaintextedit.h"

// ---------------------------------------------------------------------
class Eview : public PlainTextEditLn
{
  Q_OBJECT

public:
  Eview(QWidget *parent = 0);
};

// ---------------------------------------------------------------------
class TextView : public QDialog
{
  Q_OBJECT

public:
  TextView(QString t,QString h,QString s);

private:
  void closeall();
  void closeme();
  void closeEvent(QCloseEvent *event);
  void help();
  void keyPressEvent(QKeyEvent *e);
  void reject();
  void savepos();
  void togglemax();
  void togglenos();
  void zoom(int);

  Eview *ev;
  bool max;
};

void textview(QString t,QString s);
void textview(QString t,QStringList s);
void textview(QString t,QString c,QString s);
void textview(QString s);
void textview(QStringList s);

#endif

