
#include "wd.h"
#include "dummy.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Dummy::Dummy(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dummy";
  widget=0;
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + q2s(unopt.join(" ")));
    return;
  }
  childStyle(opt);
}
