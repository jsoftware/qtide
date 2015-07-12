
#include "cmd.h"
#include "form.h"
#include "isigraph2.h"
#include "isigraph.h"
#include "pane.h"

// ---------------------------------------------------------------------
Isigraph::Isigraph(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="isigraph";
  Isigraph2 *w= new Isigraph2(this);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);
  f->isigraph = this;
}

// ---------------------------------------------------------------------
void Isigraph::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="print")) form=pform;
  form->isigraph=this;
}

// ---------------------------------------------------------------------
string Isigraph::get(string p,string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Isigraph::set(string p,string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
string Isigraph::state()
{
  return "";
}
