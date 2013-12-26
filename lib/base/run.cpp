#include <QTextBlock>

#include "plaintextedit.h"
#include "base.h"
#include "nedit.h"
#include "note.h"
#include "tedit.h"

using namespace std;

// ---------------------------------------------------------------------
void Note::runline(bool advance, bool show)
{
  Q_UNUSED(show);
  note->saveall();
  Nedit *e = editPage();
  int len = e->blockCount();
  QTextCursor c = e->textCursor();
  QString txt = c.block().text();
  int row = c.blockNumber();

// advance to next line, or if blank, to before next entry
  if (advance) {
    c.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,1);
    if(c.block().text().trimmed().isEmpty()) {
      QTextCursor cnext=c;
      while (len>++row) {
        cnext.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,1);
        if(cnext.block().text().trimmed().size()) break;
        c=cnext;
      }
    }
    c.movePosition(QTextCursor::EndOfBlock,QTextCursor::MoveAnchor,1);
    e->setTextCursor(c);
  }

  tedit->docmdp(txt,true);
}

// ---------------------------------------------------------------------
void Note::runlines(bool all)
{
  note->saveall();
  QString txt;
  Nedit *e = editPage();
  if (all)
    txt=e->toPlainText();
  else
    txt=e->readselected();
  tedit->docmds(txt);
}
