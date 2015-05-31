
#include "isidraw.h"
#include "isigraph2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

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
  f->isigraph=this;
}

// ---------------------------------------------------------------------
void Isidraw::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="print")) form=pform;
  form->isigraph=this;
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
