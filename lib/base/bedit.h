#ifndef BEDIT_H
#define BEDIT_H

#include "plaintextedit.h"

class QPaintEvent;
class QResizeEvent;
class QScrollBar;
class QSize;
class QTextCursor;
class QWidget;
#ifdef TABCOMPLETION
class QCompleter;
#endif

// ---------------------------------------------------------------------
class Bedit : public PlainTextEditLn
{
  Q_OBJECT

public:
  Bedit(QWidget *parent = 0);
  ~Bedit();

  void home();
  void homeshift();
  QString readselected();
  QString readselect_line(int *pos, int *len);
  QString readselect_text(int *pos, int *len);
  int readcurpos();
  int readtop();
  void selectline(int p);
  void setselect(int p, int len);
  void setcurpos(int pos);
  void settop(int p);
#ifdef TABCOMPLETION
  void setCompleter(QCompleter *c);
  QCompleter *completer() const;
#endif

  int type;

protected:
#ifdef TABCOMPLETION
  void keyPressEvent(QKeyEvent *e);
  void focusInEvent(QFocusEvent *e);
#endif

private slots:
#ifdef TABCOMPLETION
  void insertCompletion(const QString &completion);
#endif

private:
#ifdef TABCOMPLETION
  QString textUnderCursor() const;
  QCompleter *c;
#endif
};

#endif
