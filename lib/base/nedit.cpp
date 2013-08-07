#include <QApplication>

#include "base.h"
#include "nedit.h"
#include "state.h"
#include "term.h"
#include <QPrinter>

using namespace std;

QMap<QString,QString> Comments;

// ---------------------------------------------------------------------
Nedit::Nedit()
{
  type=1;
  ensureCursorVisible();
  setLineWrapMode(config.LineWrap ?
                  QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
  setFont(config.Font);
  QPalette p = palette();
  p.setColor(QPalette::Active, QPalette::Base, config.EditBack.color);
  p.setColor(QPalette::Inactive, QPalette::Base, config.EditBack.color);
  p.setColor(QPalette::Text, config.EditFore.color);
  setPalette(p);
  highlight(document());
}

// ---------------------------------------------------------------------
QString Nedit::getcomment()
{
  if (Comments.isEmpty()) init_comments();
  QString f=file->fileName();
  int n=f.lastIndexOf('.');
  if (n==-1) return "";
  return Comments[f.mid(n+1)];
}

// ---------------------------------------------------------------------
void Nedit::init_comments()
{
  Comments["ijs"]="NB.";
  Comments["ijt"]="NB.";
  Comments["k"]="/";
  Comments["q"]="/";
  Comments["R"]="#";
  Comments["sh"]="#";
  Comments["tex"]="#";
}

// ---------------------------------------------------------------------
void Nedit::keyPressEvent(QKeyEvent *e)
{
  QPlainTextEdit::keyPressEvent(e);
}
