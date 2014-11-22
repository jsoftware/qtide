
#include "grid/wgrid.h"

// ---------------------------------------------------------------------
void WGrid::drawcells()
{
  painter=new QPainter(pmcell);

  if (0<Crws && 0<Ccls) {
    drawcellback();
    drawcelltext();
    drawcelllines();
  }

  delete painter;
}

// ---------------------------------------------------------------------
void WGrid::drawcellback()
{
  painter->setBrush(QBrush(CellBack));
  painter->drawRect(0,0,Vw,Vh);

  painter->setBrush(QBrush(CellBackAlt));
  for (int i=1; i<Vrws; i+=2)
    painter->drawRect(0,Vhp[i],Vw,Vhs[i]);
}

// ---------------------------------------------------------------------
void WGrid::drawcelllines()
{
  int i;

  painter->setPen(QPen(GridLines,1));
  for (i=0; i<=Vrws; i++)
    painter->drawLine(0,Vhp[i],Vw,Vhp[i]);
  for (i=0; i<=Vcls; i++)
    painter->drawLine(Vwp[i],0,Vwp[i],Vh);
}

// ---------------------------------------------------------------------
void WGrid::drawcelltext()
{
  int i,j,n,p;
  QFontMetrics fm=QFontMetrics(font,0);
  painter->setPen(QPen(CellText,1));

  int my=My+fm.ascent();

  for (i=0; i<Vrws; i++) {
    p=Ccls*(i+Srws);
    for (j=0; j<Vcls; j++) {
      n=Scls+p+j;
      switch (vindex(n,DataAlign)) {
      case 0:
        painter->drawText(Mx+Vwp[j],my+Vhp[i],Data[n]);
        break;
      case 1:
        painter->drawText((Vwp[j]+Vwp[j+1]-Dws[n])/2,my+Vhp[i],Data[n]);
        break;
      case 2:
        painter->drawText(Vwp[j+1]-Dws[n]-Mx,my+Vhp[i],Data[n]);
        break;
      }
    }
  }
}
