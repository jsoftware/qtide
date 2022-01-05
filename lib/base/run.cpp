#include <QTextBlock>

#include "plaintextedit.h"
#include "base.h"
#include "jsvr.h"
#include "nedit.h"
#include "note.h"
#include "tedit.h"
#include "term.h"

// using namespace std;

// ---------------------------------------------------------------------
bool Note::isMultiline(QString txt)
{
  sets("inputx_jrx_",q2s(txt));
  return dorb("ismultiline_jqtide_ inputx_jrx_");
}

// ---------------------------------------------------------------------
void Note::runline(bool advance, bool show)
{
  note->saveall();
  Nedit *e = editPage();
  int len = e->blockCount();
  QTextCursor c = e->textCursor();
  QString txt = c.block().text();
  int row = c.blockNumber();
  bool multi=isMultiline(txt);
  if (multi) {
    QString s;
    while (len>++row) {
      c.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,1);
      s=c.block().text();
      txt+="\n" + s;
      if (s==")") break;
    }
  }
// advance to next line, or if blank, to before next entry
  if (advance) {
    c.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,1);
    c.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor,1);
    e->setTextCursor(c);
    if (c.block().text().trimmed().isEmpty()) {
      QTextCursor cnext=c;
      while (len>++row) {
        cnext.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,1);
        if (cnext.block().text().trimmed().size()) break;
        c=cnext;
      }
    }
    c.movePosition(QTextCursor::EndOfBlock,QTextCursor::MoveAnchor,1);
    e->setTextCursor(c);
  }

  if (multi)
    tedit->docmds(txt,true,show);
  else
    tedit->docmdp(txt,true,show);
}

// ---------------------------------------------------------------------
void Note::runlines(bool all, bool show)
{
  note->saveall();
  QString txt;
  Nedit *e = editPage();
  if (all) {
    if (note->saveall())
      tedit->runall(e->fname, show);
  } else {
    txt=e->readselected();
    tedit->docmds(txt, show);
  }
}

// ---------------------------------------------------------------------
void Term::runlines()
{
  tedit->docmds(tedit->readselected(), true);
}
