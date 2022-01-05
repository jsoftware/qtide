
#include "cmd.h"
#include "dummy.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

// ---------------------------------------------------------------------
Dummy::Dummy(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dummy";
  widget=0;
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  childStyle(opt);
}

// ---------------------------------------------------------------------
std::string Dummy::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Dummy::set(std::string p,std::string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Dummy::state()
{
  return "";
}
