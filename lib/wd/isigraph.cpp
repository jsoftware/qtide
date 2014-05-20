
#include "isigraph.h"
#include "isigraph2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

Child *isigraph=0;

// ---------------------------------------------------------------------
Isigraph::Isigraph(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type = "isigraph";
  Isigraph2 *w= new Isigraph2(this, p);
  widget=(QWidget *) w;
  w->type=type;
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

