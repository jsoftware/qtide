
#include "grid/wgrid.h"

// ---------------------------------------------------------------------
void WGrid::drawlabels()
{
  if (Lw==0) return;

  painter=new QPainter(pmlab);

  drawlabelback();
  drawlabeltext();
  drawlabellines();
  drawlabelmerge();

  delete painter;
}

// ---------------------------------------------------------------------
void WGrid::drawlabelback()
{
  painter->setPen(QPen(HdrBack,1));
  painter->setBrush(QBrush(HdrBack));
  painter->drawRect(0,0,Lw,Vh);
}

// ---------------------------------------------------------------------
void WGrid::drawlabellines()
{
  int i;

  painter->setPen(QPen(GridLight,1));
  for (i=0; i<Lcls; i++)
    painter->drawLine(Gwp[i]+1,0,Gwp[i]+1,Vh);
  for (i=0; i<Vrws; i++)
    painter->drawLine(0,Vhp[i]+1,Lw,Vhp[i]+1);

  painter->setPen(QPen(GridLines,1));
  for (i=0; i<=Lcls; i++)
    painter->drawLine(Gwp[i],0,Gwp[i],Vh);
  for (i=0; i<=Vrws; i++)
    painter->drawLine(0,Vhp[i],Lw,Vhp[i]);
}

// ---------------------------------------------------------------------
void WGrid::drawlabelmerge()
{
  int c,h,i,n,p,r,s,t,w,x,y;
  QFontMetrics fm=QFontMetrics(font,0);
  int my=My+fm.ascent();

  painter->setBrush(QBrush(HdrBack));

  for (i=0; i<LabMerge.size(); i++) {
    if (LabMerge[i]>1) {
      n=LabMerge[i];
      p=LabMergeIndex[i];
      c=p/Crws;
      r=p-c*Crws;

      if (Srws>=r+n || Srws+Vrws<=r) continue;

      x=Gwp[c];
      t=Gwp[c+1];
      w=Gws[c];
      y=Chp[r]-Chp[Srws];
      s=Chp[r+n]-Chp[Srws];
      h=s-y;

      painter->setPen(QPen(GridLines,1));
      painter->drawRect(x,y,w,h);
      painter->setPen(QPen(GridLight,1));
      painter->drawLine(x+1,y+1,t-1,y+1);
      painter->drawLine(x+1,y+1,x+1,y+h-1);
      painter->setPen(QPen(CellText,1));

      switch (vindex(p,LabAlignX)) {
      case 0:
        painter->drawText(Mx+x,my+y,LabX[p]);
        break;
      case 1:
        painter->drawText((x+t-Lws[p])/2,my+y,LabX[p]);
        break;
      case 2:
        painter->drawText(t-Lws[p]-Mx,my+y,LabX[p]);
        break;
      }
    }
  }
}

// ---------------------------------------------------------------------
void WGrid::drawlabeltext()
{
  int i,j,n,p;
  QFontMetrics fm=QFontMetrics(font,0);
  painter->setPen(QPen(CellText,1));
  int my=My+fm.ascent();

  for (i=0; i<Lcls; i++) {
    p=i*Crws;
    for (j=0; j<Vrws; j++) {
      n=Srws+p+j;
      if (0==vindex(n,LabMergeMask)) continue;
      switch (vindex(n,LabAlign)) {
      case 0:
        painter->drawText(Mx+Gwp[i],my+Vhp[j],LabX[n]);
        break;
      case 1:
        painter->drawText((Gwp[i]+Gwp[i+1]-Lws[n])/2,my+Vhp[j],LabX[n]);
        break;
      case 2:
        painter->drawText(Gwp[i+1]-Lws[n]-Mx,my+Vhp[j],LabX[n]);
        break;
      }
    }
  }
}
