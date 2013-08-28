#ifndef BEDIT_H
#define BEDIT_H

#include <QPlainTextEdit>

class QPaintEvent;
class QResizeEvent;
class QScrollBar;
class QSize;
class QWidget;

class LineNumberArea;

// ---------------------------------------------------------------------
class Bedit : public QPlainTextEdit
{
  Q_OBJECT

public:
  Bedit(QWidget *parent = 0);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();
  QString readhelptext(int);
  QString readselected();
  QString readselect_line(int *pos, int *len);
  QString readselect_text(int *pos, int *len);
  int readcurpos();
  int readtop();
  void resizer();
  void selectline(int p);
  void setselect(int p, int len);
  void setcurpos(int pos);
  void settop(int p);

  int type;

protected:
  void resizeEvent(QResizeEvent *event);

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &, int);

private:
  QWidget *lineNumberArea;
};

// ---------------------------------------------------------------------
class LineNumberArea : public QWidget
{
public:
  LineNumberArea(Bedit *editor) : QWidget(editor) {
    edit = editor;
  }

  QSize sizeHint() const {
    return QSize(edit->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) {
    edit->lineNumberAreaPaintEvent(event);
  }

private:
  Bedit *edit;
};

#endif
