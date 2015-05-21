#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class QPrinter;

// ---------------------------------------------------------------------
class PlainTextEdit : public QPlainTextEdit
{
  Q_OBJECT

public:
  PlainTextEdit(QWidget *parent = 0);
  bool acceptKeyBack;

protected:
  void keyReleaseEvent(QKeyEvent *event);

#ifndef QT_NO_PRINTER
public slots:
  void printPreview(QPrinter * printer);
#endif

};

#endif
