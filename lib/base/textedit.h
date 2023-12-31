#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextBlock>
#include <QTextEdit>
#include <QKeyEvent>

class QPrinter;
class QSyntaxHighlighter;

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
  void settabwidth();
  void setselect(int p, int len);
  void setcurpos(int pos);
  int type;

  QSyntaxHighlighter* (*highlighter)(QTextDocument *d);

#ifdef Q_OS_ANDROID
  QTextCursor cu0;
#endif

#ifndef QT_NO_PRINTER
public slots:
  void print(QPrinter *printer);
  void printPreview(QPrinter *printer);
#endif

#ifdef Q_OS_ANDROID
public slots:
  void copy();
  void cut();
#endif

private:
  void newblock();
  QTextBlockFormat blockfmt;
  QTextCharFormat charfmt;

private slots:
  void highlightCurrentLine();

};

#endif
