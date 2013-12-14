#ifndef VIEW_H
#define VIEW_H

#include <QDialog>
#include <QPlainTextEdit>

// ---------------------------------------------------------------------
class Eview : public QPlainTextEdit
{
  Q_OBJECT

public:
  Eview();

private slots:
  void highlightCurrentLine();

};

// ---------------------------------------------------------------------
class TextView : public QDialog
{

  Q_OBJECT

public:
  TextView(QString t,QString s);

private slots:

private:
  void reject();
  void savepos();
  void keyReleaseEvent(QKeyEvent *e);

};

void textview(QString t,QString s);
void textview(QString t,QStringList s);
void textview(QString s);
void textview(QStringList s);

#endif

