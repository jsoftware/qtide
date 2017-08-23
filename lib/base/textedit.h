#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextBlock>
#include <QTextEdit>
#include <QKeyEvent>

class QPrinter;

// ---------------------------------------------------------------------
class TextEdit : public QTextEdit
{
  Q_OBJECT

public:
  TextEdit(QWidget *parent = 0);

  void appendHtml(const QString &);
  void appendPlainText(const QString &);
  QTextBlock firstVisibleBlock();
  void home();
  void homeshift();
  QString readselected();
  int readcurpos();
  int readtop();
  void setselect(int p, int len);
  void setcurpos(int pos);
  int type;

#ifndef QT_NO_PRINTER
public slots:
  void printPreview(QPrinter *printer);
#endif

private:
  void newblock();
  QTextBlockFormat blockfmt;
  QTextCharFormat charfmt;

private slots:
  void highlightCurrentLine();

};

#endif
