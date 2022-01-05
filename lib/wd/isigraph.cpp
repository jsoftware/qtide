
#include "cmd.h"
#include "form.h"
#include "isigraph2.h"
#include "isigraph.h"
#include "pane.h"

extern Form *form;
extern Child *isigraph;

// ---------------------------------------------------------------------
Isigraph::Isigraph(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="isigraph";
  Isigraph2 *w= new Isigraph2(this);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);
  isigraph = this;
}

// ---------------------------------------------------------------------
void Isigraph::setform()
{
  if (!widget) return;
  if (!(event=="paint" || event=="resize" || event=="print")) Child::setform();
  isigraph=this;
}

// ---------------------------------------------------------------------
std::string Isigraph::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void Isigraph::set(std::string p,std::string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Isigraph::state()
{
  return "";
}
