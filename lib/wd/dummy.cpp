
#include "wd.h"
#include "dummy.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Dummy::Dummy(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dummy";
  widget=0;
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
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
