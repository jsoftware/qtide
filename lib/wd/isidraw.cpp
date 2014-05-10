
#include "isidraw.h"
#include "isigraph2.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Isidraw::Isidraw(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type = "isidraw";
  Isigraph2 *w= new Isigraph2(this, p);
  widget=(QWidget *) w;
  w->type=type;
  QString qn=s2q(n);
  w->setObjectName(qn);
  isigraph = this;
}

// ---------------------------------------------------------------------
void Isidraw::set(string p,string v)
{
  if (!widget) return;
  Isigraph2 *w=(Isigraph2*) widget;
  QStringList opt=qsplit(v);

  if (p=="timer") {
    if (opt.isEmpty()) {
      error("set timer requires 1 number: " + p);
      return;
    }
    w->setTimer(c_strtoi(q2s(opt.at(0))));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Isidraw::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="print" || event=="timer")) form=pform;
  isigraph=(Child *)this;
}
