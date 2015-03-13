
#include "isidraw.h"
#include "isigraph2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Isidraw::Isidraw(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="isidraw";
  Isigraph2 *w= new Isigraph2(this, p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  f->isigraph=this;
}

// ---------------------------------------------------------------------
Isidraw::~Isidraw()
{
  if (widget) delete (Isigraph2 *)widget;
  widget=0;
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
