
#include "cmd.h"
#include "dummy.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

// ---------------------------------------------------------------------
Dummy::Dummy(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dummy";
  widget=0;
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  childStyle(opt);
}

// ---------------------------------------------------------------------
string Dummy::get(string p,string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Dummy::set(string p,string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
string Dummy::state()
{
  return "";
}
