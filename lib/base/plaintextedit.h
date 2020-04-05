#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QKeyEvent>

class LineNumberArea;
class QPrinter;
class QWidget;
class QSyntaxHighlighter;

// ---------------------------------------------------------------------
class PlainTextEdit : public QPlainTextEdit
{
  Q_OBJECT

public:
  PlainTextEdit(QWidget *parent = 0);
  QSyntaxHighlighter* (*highlighter)(QTextDocument *d);

#ifndef QT_NO_PRINTER
public slots:
  void print(QPrinter  *printer);
  void printPreview(QPrinter *printer);
#endif

};

// ---------------------------------------------------------------------
class PlainTextEditLn : public PlainTextEdit
{
  Q_OBJECT

public:
  PlainTextEditLn(QWidget *parent = 0);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();
  void resizer();
  void settabwidth();
  bool showlineNumbers();

  bool showNos;
  QString type;

public slots:
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &, int);
  void updateLineNumberAreaWidth(int newBlockCount);

protected:
  void resizeEvent(QResizeEvent *event);

private:
  QWidget *lineNumberArea;

};

// ---------------------------------------------------------------------
class LineNumberArea : public QWidget
{
public:
  LineNumberArea(PlainTextEditLn *editor) : QWidget(editor)
  {
    edit = editor;
  }

  QSize sizeHint() const
  {
    return QSize(edit->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event)
  {
    edit->lineNumberAreaPaintEvent(event);
  }

private:
  PlainTextEditLn *edit;
};

#endif

