// Grid control

#include "cmd.h"
#include "form.h"
#include "isigrid.h"
#include "pane.h"
#include "wd.h"
#include "../grid/qgrid.h"

// ---------------------------------------------------------------------
IsiGrid::IsiGrid(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="isigrid";
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"cube")) return;
  if (opt.size()==0)
    style="";
  else
    style=opt[0];
  widget=new QGrid(style,this,p);
  widget->setObjectName(qn);
  childStyle(opt);
}

// ---------------------------------------------------------------------
std::string IsiGrid::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void IsiGrid::set(std::string p,std::string v)
{
  ((QGrid *)widget)->set(p,v);
}

// ---------------------------------------------------------------------
std::string IsiGrid::state()
{
  return ((QGrid *)widget)->state(event);
}
