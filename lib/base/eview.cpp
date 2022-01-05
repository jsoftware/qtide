
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>

#include "eview.h"
#include "plaintextedit.h"
#include "state.h"
#include "term.h"

// using namespace std;

int TextViewFontSize=0;
bool TextViewNos=false;
QList <TextView *> TextViews;

// ---------------------------------------------------------------------
Eview::Eview(QWidget *parent) : PlainTextEditLn(parent)
{
  Q_UNUSED(parent);
  ensureCursorVisible();
  setLineWrapMode(PlainTextEdit::NoWrap);
  setFont(config.Font);
  type="view";
  showNos=TextViewNos;
}

// ---------------------------------------------------------------------
TextView::TextView(QString t,QString c,QString s)
{
  setAttribute(Qt::WA_DeleteOnClose);
  if (TextViewFontSize==0)
    TextViewFontSize=config.Font.pointSize();
  if (t=="") t="View - Ctrl+H help";
  ev=new Eview(this);
  ev->document()->setPlainText(s);
  ev->moveCursor(QTextCursor::Start);
  QPalette p = ev->palette();
  p.setColor(QPalette::Base, config.ViewBack.color);
  p.setColor(QPalette::Text, config.ViewFore.color);
  p.setColor(QPalette::Highlight, config.ViewSelb.color);
  p.setColor(QPalette::HighlightedText, config.ViewSelt.color);
  ev->setPalette(p);
  zoom(0);
  max=false;

  QVBoxLayout *v=new QVBoxLayout();

  if (c.isEmpty())
    v->setContentsMargins(0,0,0,0);
  else {
    QLabel *cap=new QLabel(c);
    v->addWidget(cap);
  }
  v->addWidget(ev);
  setLayout(v);
  setWindowFlags(Qt::Window);
  setWindowTitle(t);

  QList<int>d=config.winpos_read("View");
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  move(d[0],d[1]);
  resize(qMax(300,d[2]),qMax(300,d[3]));
#endif

  activateWindow();
// following 2 lines avoid spurious blank line number column
  ev->showNos=TextViewNos;
  ev->resizer();
  show();
  ev->setFocus();
}

// ---------------------------------------------------------------------
void TextView::closeall()
{
  TextViews.removeOne(this);
  int len=TextViews.length();
  for (int i=len-1; i>=0; i--)
    TextViews[i]->close();
  closeme();
}

// ---------------------------------------------------------------------
void TextView::closeme()
{
  TextViews.removeOne(this);
  savepos();
  close();

}

// ---------------------------------------------------------------------
void TextView::closeEvent(QCloseEvent *event)
{
  if (TextViews.length() == 1) savepos();
  TextViews.removeOne(this);
  QWidget::closeEvent(event);
}

// ---------------------------------------------------------------------
void TextView::help()
{
  QString s;
  s+="Ctrl+=\tzoom in\n";
  s+="Ctrl+-\tzoom out\n";
  s+="Ctrl+0\tzoom reset\n";
  s+="Ctrl+N\ttoggle line numbers\n";
  s+="Ctrl+Q|Esc\tclose\n";
  s+="Ctrl+Shift+Q\tclose all textviews\n";
  s+="Ctrl+W\ttoggle word wrap\n";
  s+="F11\ttoggle fullscreen\n";
  info("View Shortcuts",s);
}

// ---------------------------------------------------------------------
void TextView::keyPressEvent(QKeyEvent *event)
{
  int key = event->key();
  Qt::KeyboardModifiers mod = app->keyboardModifiers();
  bool ctrl = mod.testFlag(Qt::ControlModifier);
  bool shift = mod.testFlag(Qt::ShiftModifier);
  if (key==Qt::Key_F11)
    togglemax();
  else if (ctrl && key==Qt::Key_Minus)
    zoom(-1);
  else if (ctrl && key==Qt::Key_Equal)
    zoom(1);
  else if (ctrl && key==Qt::Key_0)
    zoom(9);
  else if (ctrl && event->key()==Qt::Key_H)
    help();
  else if (ctrl && event->key()==Qt::Key_N)
    togglenos();
  else if (ctrl && shift && event->key()==Qt::Key_Q)
    closeall();
  else if (ctrl && event->key()==Qt::Key_Q)
    closeme();
  else if (ctrl && event->key()==Qt::Key_W)
    ev->setLineWrapMode((1==ev->lineWrapMode()) ? QPlainTextEdit::NoWrap : QPlainTextEdit::WidgetWidth);
  else QDialog::keyPressEvent(event);
}

// ---------------------------------------------------------------------
void TextView::reject()
{
  savepos();
  QDialog::reject();
}

// ---------------------------------------------------------------------
void TextView::savepos()
{
  config.winpos_save(this,"View");
}

// ---------------------------------------------------------------------
void TextView::togglemax()
{
  max=!max;
  setWindowState(max ? Qt::WindowMaximized : Qt::WindowNoState);
}

// ---------------------------------------------------------------------
void TextView::togglenos()
{
  TextViewNos=!TextViewNos;
  ev->showNos=TextViewNos;
  ev->resizer();
}

// ---------------------------------------------------------------------
void TextView::zoom(int n)
{
  if (n==9)
    TextViewFontSize=config.Font.pointSize();
  else
    TextViewFontSize=qMax(4,TextViewFontSize+n);
  QFont f=ev->font();
  f.setPointSize(TextViewFontSize);
  ev->setFont(f);
  ev->settabwidth();
  ev->update();
}

// ---------------------------------------------------------------------
void textview(QString s)
{
  textview("View",s);
}

// ---------------------------------------------------------------------
void textview(QStringList s)
{
  textview(s.join("\n"));
}

// ---------------------------------------------------------------------
void textview(QString t,QString s)
{
  textview(t,"",s);
}

// ---------------------------------------------------------------------
// title, caption, text
void textview(QString t,QString c,QString s)
{
  TextView *n = new TextView(t,c,s);
  TextViews.append(n);
}

// ---------------------------------------------------------------------
void textview(QString t,QStringList s)
{
  textview(t,s.join("\n"));
}
