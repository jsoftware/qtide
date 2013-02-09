
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
}
