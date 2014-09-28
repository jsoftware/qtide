
#include "wd.h"
#include "../base/bedit.h"
#include "../base/note.h"
#include "../base/tedit.h"
#include "../base/term.h"

extern int rc;

QString smget(string);
QString smread();
QString smunknown(string);

// ---------------------------------------------------------------------
// c is type, p is parameter
QString sm(string c,string p)
{
  if (c=="get")
    return smget(p);
  rc=0;
  if (c=="act")
    term->smact();
  else if (c=="prompt")
    term->smprompt(s2q(p));
  else
    return smunknown(c);
  return "";
}

// ---------------------------------------------------------------------
// get active window
QString smget(string p)
{
  Bedit *Win;

  rc=-1;
  if (term->isActiveWindow())
    Win=tedit;
  else
    Win=(Bedit *)note->editPage();

  if (p=="select") {
    int bgn,end;
    QTextCursor c=Win->textCursor();
    bgn=c.selectionStart();
    end=c.selectionEnd();
    return QString::number(bgn)+" "+QString::number(end);
  }

  if (p=="text")
    return Win->toPlainText();

  return smunknown(p);
}

// ---------------------------------------------------------------------
QString smunknown(string p)
{
  rc=-1;
  return "unrecognized sm command: " + s2q(p);
}
