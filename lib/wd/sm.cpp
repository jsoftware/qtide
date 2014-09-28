
#include <QApplication>

#include "wd.h"
#include "../base/bedit.h"
#include "../base/note.h"
#include "../base/tedit.h"
#include "../base/term.h"

extern int rc;

string smget(string);
string smread();
string smerror(string);

// ---------------------------------------------------------------------
// c is type, p is parameter
string sm(string c,string p)
{
  rc=-2;
  if (c=="get")
    return smget(p);
  rc=0;
  if (c=="act")
    term->smact();
  else if (c=="prompt")
    term->smprompt(s2q(p));
  else
    return smerror("unrecognized sm command: " + c);
  return "";
}

// ---------------------------------------------------------------------
// get window
// p is active|edit|term
string smget(string p)
{
  Bedit *edit, *win;
  QWidget *w=QApplication::focusWidget();

  if (note)
    edit=(Bedit *)note->editPage();
  else
    edit=0;

  if (p.size()==0)
    return smerror("sm get parameter should be one of active|edit|term");
  else if (p=="term")
    win=tedit;
  else if (p=="edit") {
    if (note)
      win=edit;
    else
      return smerror("No active edit window");
  } else if (p=="active") {
    if (w==(QWidget*)tedit)
      win=tedit;
    else if (note && w==(QWidget*)edit)
      win=edit;
    else
      return smerror("No active window");
  } else
    return smerror("unrecognized sm command: get " + p);

  QTextCursor c=win->textCursor();
  int b=c.selectionStart();
  int e=c.selectionEnd();
  string r;
  r+=spair("text",win->toPlainText());
  r+=spair("select",QString::number(b)+" "+QString::number(e));
  return r;
}

// ---------------------------------------------------------------------
string smerror(string p)
{
  rc=1;
  return p;
}
