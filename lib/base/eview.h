#ifndef EVIEW_H
#define EVIEW_H

#include <QDialog>
#include "plaintextedit.h"

// ---------------------------------------------------------------------
class Eview : public PlainTextEdit
{
  Q_OBJECT

public:
  Eview(QWidget *parent = 0);

private slots:
  void highlightCurrentLine();

};

// ---------------------------------------------------------------------
class TextView : public QDialog
{
  Q_OBJECT

public:
  TextView(QString t,QString h,QString s);

private:
  void reject();
  void savepos();
  void keyPressEvent(QKeyEvent *e);

  Eview *ev;

};

void textview(QString t,QString s);
void textview(QString t,QStringList s);
void textview(QString t,QString c,QString s);
void textview(QString s);
void textview(QStringList s);


#endif

