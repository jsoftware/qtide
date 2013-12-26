
#include <QVBoxLayout>
#include "plaintextedit.h"
#include "state.h"
#include "view.h"
#include "term.h"

using namespace std;

// ---------------------------------------------------------------------
Eview::Eview()
{
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
TextView::TextView(QString t,QString s)
{
  QVBoxLayout *v=new QVBoxLayout();
  v->setContentsMargins(0,0,0,0);
  Eview *e=new Eview();
  e->document()->setPlainText(s);
  e->moveCursor(QTextCursor::Start);
  v->addWidget(e);
  setLayout(v);
  setWindowTitle(t);
#ifdef SMALL_SCREEN
  move(0,0);
  resize(term->width(),term->height());
#else
  resize(600,500);
#endif
  activateWindow();
  show();
  e->setFocus();
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
void TextView::keyReleaseEvent(QKeyEvent *event)
{
#ifdef Q_OS_ANDROID
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
  new TextView(t,s);
}

// ---------------------------------------------------------------------
void textview(QString t,QStringList s)
{
  textview(t,s.join("\n"));
}
