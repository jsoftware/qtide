
#include "grid/wgrid.h"

// ---------------------------------------------------------------------
void WGrid::drawheaders()
{
  if (Hh==0) return;

  painter=new QPainter(pmhdr);

  drawheaderback();
  drawheadertext();
  drawheaderlines();
  drawheadermerge();

  delete painter;
}

// ---------------------------------------------------------------------
void WGrid::drawheaderback()
{
  painter->setPen(QPen(HdrBack,1));
  painter->setBrush(QBrush(HdrBack));
  painter->drawRect(0,0,Vw,Hh);
}

// ---------------------------------------------------------------------
void WGrid::drawheaderlines()
{
  int i;

  painter->setPen(QPen(GridLight,1));
  for (i=0; i<Hrws; i++)
    painter->drawLine(0,Ghp[i]+1,Vw,Ghp[i]+1);
  for (i=0; i<Vcls; i++)
    painter->drawLine(Vwp[i]+1,0,Vwp[i]+1,Hh);

  painter->setPen(QPen(GridLines,1));
  for (i=0; i<=Hrws; i++)
    painter->drawLine(0,Ghp[i],Vw,Ghp[i]);
  for (i=0; i<=Vcls; i++)
    painter->drawLine(Vwp[i],0,Vwp[i],Hh);
}

// ---------------------------------------------------------------------
void WGrid::drawheadermerge()
{
  if (HdrMerge==DefMerge) return;

  int c,h,i,n,p,r,t,x,w,y;
  QFontMetrics fm=QFontMetrics(font,0);
  int my=My+fm.ascent();

  painter->setBrush(QBrush(HdrBack));

  for (i=0; i<HdrMerge.size(); i++) {
    if (HdrMerge[i]>1) {
      n=HdrMerge[i];
      p=HdrMergeIndex[i];
      r=p/Ccls;
      c=p-r*Ccls;

      if (Scls>=c+n || Scls+Vcls<=c) continue;

      x=Cwp[c]-Cwp[Scls];
      t=Cwp[c+n]-Cwp[Scls];
      w=t-x;
      y=Ghp[r];
      h=Ghs[r];

      painter->setPen(QPen(GridLines,1));
      painter->drawRect(x,y,w,h);
      painter->setPen(QPen(GridLight,1));
      painter->drawLine(x+1,y+1,t-1,y+1);
      painter->drawLine(x+1,y+1,x+1,y+h-1);
      painter->setPen(QPen(CellText,1));

      switch (vindex(p,HdrAlignX)) {
      case 0:
        painter->drawText(Mx+x,my+y,HdrX[p]);
        break;
      case 1:
        painter->drawText((x+t-Hws[p])/2,my+y,HdrX[p]);
        break;
      case 2:
        painter->drawText(t-Hws[p]-Mx,my+y,HdrX[p]);
        break;
      }
    }
  }
}

// ---------------------------------------------------------------------
void WGrid::drawheadertext()
{
  int i,j,n,p;
  QFontMetrics fm=QFontMetrics(font,0);
  painter->setPen(QPen(CellText,1));
  int my=My+fm.ascent();

  for (i=0; i<Hrws; i++) {
    p=i*Ccls;
    for (j=0; j<Vcls; j++) {
      n=Scls+p+j;
      if (0==vindex(n,HdrMergeMask)) continue;
      switch (vindex(n,HdrAlignX)) {
      case 0:
        painter->drawText(Mx+Vwp[j],my+Ghp[i],HdrX[n]);
        break;
      case 1:
        painter->drawText((Vwp[j]+Vwp[j+1]-Hws[n])/2,my+Ghp[i],HdrX[n]);
        break;
      case 2:
        painter->drawText(Vwp[j+1]-Hws[n]-Mx,my+Ghp[i],HdrX[n]);
        break;
      }
    }
  }
}
