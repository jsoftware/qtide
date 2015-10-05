#include <QApplication>

#include "base.h"
#include "nedit.h"
#include "state.h"
#include "term.h"

using namespace std;

QMap<QString,QString> Comments;

// ---------------------------------------------------------------------
Nedit::Nedit()
{
  type=1;
  ensureCursorVisible();
  setLineWrapMode(config.LineWrap ?
                  PlainTextEdit::WidgetWidth : PlainTextEdit::NoWrap);
  setFont(config.Font);
  QPalette p = palette();
  p.setColor(QPalette::Base, config.EditBack.color);
  p.setColor(QPalette::Text, config.EditFore.color);
  p.setColor(QPalette::Highlight, config.EditSelb.color);
  p.setColor(QPalette::HighlightedText, config.EditSelt.color);
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
  Comments["jproj"]="NB.";
  Comments["k"]="/";
  Comments["q"]="/";
  Comments["R"]="#";
  Comments["sh"]="#";
  Comments["tex"]="#";
}

// ---------------------------------------------------------------------
void Nedit::keyPressEvent(QKeyEvent *e)
{
  Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();
  bool shift = mod.testFlag(Qt::ShiftModifier);
  bool ctrl = mod.testFlag(Qt::ControlModifier);
  int key = e->key();
  if (key==Qt::Key_Home && (ctrl==false) && shift==false)
    home();
  else
    Bedit::keyPressEvent(e);
}

// ---------------------------------------------------------------------
Nedit::~Nedit()
{
  delete file;
}
