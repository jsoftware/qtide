
#include "isigraph.h"
#include "isigraph2.h"
#include "form.h"

Isigraph *isigraph=0;

// ---------------------------------------------------------------------
Isigraph::Isigraph(string s, string p, Form *f) : Child(s,p,f)
{
  type = "isigraph";
  Isigraph2 *w= new Isigraph2(this);
  widget=(QWidget*) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
  isigraph = this;
}

// ---------------------------------------------------------------------
void Isigraph::setform()
{
  if (event=="paint" || event=="print") return;
  form=pform;
}

// ---------------------------------------------------------------------
void Isigraph::setp(string p,string v)
{
  Child::setp(p,v);
}
