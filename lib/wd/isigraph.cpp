
#include "isigraph.h"
#include "isigraph2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Isigraph::Isigraph(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type = "isigraph";
  Isigraph2 *w= new Isigraph2(this, p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  childStyle(opt);
  f->isigraph = this;
}

// ---------------------------------------------------------------------
Isigraph::~Isigraph()
{
  if (widget) delete (Isigraph2 *)widget;
  widget=0;
}

// ---------------------------------------------------------------------
void Isigraph::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="print")) form=pform;
  form->isigraph=this;
}

