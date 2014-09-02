
#include "grid/wgrid.h"

// ---------------------------------------------------------------------
void WGrid::drawtop()
{
  if (Hh==0 || Lw==0) return;

  painter=new QPainter(pmtop);

  painter->setPen(QPen(HdrBack,1));
  painter->setBrush(QBrush(HdrBack));
  painter->drawRect(0,0,Lw,Hh);

  int i;
  painter->setPen(QPen(GridLight,1));
  for (i=0; i<Hrws; i++)
    painter->drawLine(0,Ghp[i]+1,Lw,Ghp[i]+1);
  for (i=0; i<Lcls; i++)
    painter->drawLine(Gwp[i]+1,0,Gwp[i]+1,Hh);

  painter->setPen(QPen(GridLines,1));
  for (i=0; i<=Hrws; i++)
    painter->drawLine(0,Ghp[i],Lw,Ghp[i]);
  for (i=0; i<=Lcls; i++)
    painter->drawLine(Gwp[i],0,Gwp[i],Hh);

  delete painter;
}
