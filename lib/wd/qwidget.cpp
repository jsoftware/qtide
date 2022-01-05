// QWidget control

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "qwidget.h"
#include "wd.h"

// ---------------------------------------------------------------------
QWidgex::QWidgex(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="qwidget";
  widget=new QWidget;
}

// ---------------------------------------------------------------------
std::string QWidgex::get(std::string p,std::string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void QWidgex::set(std::string p,std::string v)
{
  Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string QWidgex::state()
{
  return "";
}
