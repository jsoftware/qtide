
#include "opengl.h"
#include "opengl2.h"
#include "form.h"
#include "pane.h"

Opengl *opengl=0;

// ---------------------------------------------------------------------
Opengl::Opengl(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type = "opengl";
  Opengl2 *w= new Opengl2(this);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  opengl = this;
}

// ---------------------------------------------------------------------
void Opengl::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="paintz" || event=="resize" || event=="initialize" || event=="print")) form=pform;
  opengl=this;
}

// ---------------------------------------------------------------------
void Opengl::setp(string p,string v)
{
  Child::setp(p,v);
}
