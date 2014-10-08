
#include "prtobj.h"

extern "C" int glzclear2 (void *p,int clear);

// ---------------------------------------------------------------------
Prtobj::Prtobj()
{
  initialized=false;
  painter=0;
  font=0;
  glzclear2 (this,0);
}

// ---------------------------------------------------------------------
Prtobj::~Prtobj()
{
  if (initialized && painter) {
    painter->end();
    delete painter;
  }
}
