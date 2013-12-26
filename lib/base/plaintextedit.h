#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

// ---------------------------------------------------------------------
class PlainTextEdit : public QPlainTextEdit
{
  Q_OBJECT

public:
  PlainTextEdit(QWidget *parent = 0);
  bool acceptKeyBack;

protected:
  void keyReleaseEvent(QKeyEvent *event);

};

#endif
