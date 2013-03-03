#ifndef PRTOBJ_H
#define PRTOBJ_H

#include <QPainter>

#include "font.h"

// ---------------------------------------------------------------------
class Prtobj
{

public:
  Prtobj();
  ~Prtobj();

  QBrush brush;
  Font *font;
  QPen pen;
  QPen textpen;
  QColor color;
  QColor pencolor;
  QColor brushcolor;
  QColor textcolor;

  QPainter *painter;

  int fontheight;

  bool antialiased;
  bool transformed;
  bool initialized;

  int brushnull;
  int clipped, textx, texty, orgx, orgy;

};

#endif
