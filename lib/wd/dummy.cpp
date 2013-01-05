
#include "wd.h"
#include "dummy.h"
#include "form.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Dummy::Dummy(string s, string p, Form *f) : Child(s,p,f)
{
  type="dummy";
  widget=0;
}

