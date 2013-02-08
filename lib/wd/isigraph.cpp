
#include "isigraph.h"
#include "isigraph2.h"
#include "form.h"
#include "pane.h"

Isigraph *isigraph=0;

// ---------------------------------------------------------------------
Isigraph::Isigraph(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type = "isigraph";
  Isigraph2 *w= new Isigraph2(this);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  isigraph = this;
}

// ---------------------------------------------------------------------
void Isigraph::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="print")) form=pform;
  isigraph=this;
}

// ---------------------------------------------------------------------
void Isigraph::setp(string p,string v)
{
  Child::setp(p,v);
}
