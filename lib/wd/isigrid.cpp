// Grid control

#include "wd.h"
#include "isigrid.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"
#include "../grid/qgrid.h"

// ---------------------------------------------------------------------
IsiGrid::IsiGrid(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="isigrid";
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (opt.size()==0)
    style="";
  else
    style=opt[0];
  widget=new QGrid(style,this,p);
  widget->setObjectName(qn);
  childStyle(opt);
}

// ---------------------------------------------------------------------
void IsiGrid::set(string p,string v)
{
  ((QGrid *)widget)->set(p,v);
}

// ---------------------------------------------------------------------
string IsiGrid::state()
{
  return ((QGrid *)widget)->state(event);
}
