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
