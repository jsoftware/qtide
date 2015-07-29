
#include "cmd.h"
#include "form.h"
#include "isidraw.h"
#include "isigraph2.h"
#include "pane.h"

extern Form *form;

// ---------------------------------------------------------------------
Isidraw::Isidraw(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="isidraw";
  Isigraph2 *w= new Isigraph2(this);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);
  isigraph=this;
}

// ---------------------------------------------------------------------
void Isidraw::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="resize" || event=="print")) Child::setform();
  isigraph=this;
}

// ---------------------------------------------------------------------
string Isidraw::get(string p,string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Isidraw::set(string p,string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
string Isidraw::state()
{
  return "";
}
