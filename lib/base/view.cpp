
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include "plaintextedit.h"
#include "state.h"
#include "view.h"
#include "term.h"

using namespace std;

// ---------------------------------------------------------------------
Eview::Eview(QWidget *parent) : PlainTextEdit(parent)
{
  Q_UNUSED(parent);
  ensureCursorVisible();
  setLineWrapMode(PlainTextEdit::NoWrap);
  setFont(config.Font);
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
  highlightCurrentLine();
}

// ---------------------------------------------------------------------
void Eview::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;
    QColor lineColor = QColor(240,240,232);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }
  setExtraSelections(extraSelections);
}

// ---------------------------------------------------------------------
TextView::TextView(QString t,QString c,QString s)
{
  ev=new Eview(this);
  ev->document()->setPlainText(s);
  ev->moveCursor(QTextCursor::Start);

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
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  resize(600,500);
#endif
  activateWindow();
  show();
  ev->setFocus();
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
void TextView::keyPressEvent(QKeyEvent *event)
{
  Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();
  bool ctrl = mod.testFlag(Qt::ControlModifier);
  if (ctrl && event->key()==Qt::Key_W)
    ev->setLineWrapMode((1==ev->lineWrapMode()) ? QPlainTextEdit::NoWrap : QPlainTextEdit::WidgetWidth);
  else QDialog::keyPressEvent(event);
}

// ---------------------------------------------------------------------
void TextView::keyReleaseEvent(QKeyEvent *event)
{
#ifdef QT_OS_ANDROID
  if (event->key()==Qt::Key_Back) {
    reject();
  } else QDialog::keyReleaseEvent(event);
#else
  QDialog::keyReleaseEvent(event);
#endif
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
  new TextView(t,c,s);
}

// ---------------------------------------------------------------------
void textview(QString t,QStringList s)
{
  textview(t,s.join("\n"));
}
