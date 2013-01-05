
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include "state.h"
#include "view.h"

using namespace std;

// ---------------------------------------------------------------------
Eview::Eview()
{
  ensureCursorVisible();
  setLineWrapMode(QPlainTextEdit::NoWrap);
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
  resize(600,500);
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
