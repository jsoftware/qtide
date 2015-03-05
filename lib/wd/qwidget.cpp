// QWidget control

#include "wd.h"
#include "qwidget.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
QWidgex::QWidgex(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="qwidget";
  widget=new QWidget;
}

// ---------------------------------------------------------------------
string QWidgex::get(string p,string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void QWidgex::set(string p,string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
string QWidgex::state()
{
  return "";
}
