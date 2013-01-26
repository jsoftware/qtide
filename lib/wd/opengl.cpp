
#include "opengl.h"
#include "opengl2.h"
#include "form.h"

Opengl *opengl=0;

// ---------------------------------------------------------------------
Opengl::Opengl(string s, string p, Form *f) : Child(s,p,f)
{
  type = "opengl";
  Opengl2 *w= new Opengl2(this);
  widget=(QWidget *) w;
  QString qs=s2q(s);
  w->setObjectName(qs);
  opengl = this;
}

// ---------------------------------------------------------------------
void Opengl::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="resize" || event=="initialize" || event=="print")) form=pform;
  opengl=this;
}

// ---------------------------------------------------------------------
void Opengl::setp(string p,string v)
{
  Child::setp(p,v);
}
