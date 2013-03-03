
#include "prtobj.h"

extern "C" int glzclear2 (void *p);

// ---------------------------------------------------------------------
Prtobj::Prtobj()
{
  painter=0;
  fontheight=0;
  initialized=false;
  glzclear2 (this);
}

// ---------------------------------------------------------------------
Prtobj::~Prtobj()
{
  if (initialized && painter) {
    painter->end();
    delete painter;
  }
}

