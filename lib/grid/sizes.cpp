
#include "grid/wgrid.h"

// ---------------------------------------------------------------------
void WGrid::drawsizes(int sx, int sy, int sw, int sh)
{
  if (sw==0 || sh==0) return;

  int i,j,n,p;

  QFontMetrics fm=QFontMetrics(font,0);

  for (i=0; i<Hrws; i++) {
    p=sx+Ccls*i;
    for (j=0; j<sw; j++) {
      n=p+j;
      if (!Hfm[n]) {
#ifdef QT512
        Hws[n]=fm.horizontalAdvance(HdrX[n]);
#else
        Hws[n]=fm.width(HdrX[n]);
#endif
        Hhs[n]=fm.height();
        Twx[sx+j]=qMax(Twx[sx+j],Hws[n]*vindex(n,HdrMergeMask));
        Hhx[i]=qMax(Hhx[i],Hhs[n]);
        Hfm[n]=true;
      }
    }
  }

  for (i=0; i<Lcls; i++) {
    p=sy+Crws*i;
    for (j=0; j<sh; j++) {
      n=p+j;
      if (!Lfm[n]) {
#ifdef QT512
        Lws[n]=fm.horizontalAdvance(LabX[n]);
#else
        Lws[n]=fm.width(LabX[n]);
#endif
        Lhs[n]=fm.height();
        Lwx[i]=qMax(Lwx[i],Lws[n]);
        Thx[sy+j]=qMax(Thx[sy+j],Lhs[n]);
        Lfm[n]=true;
      }
    }
  }

  for (i=0; i<sh; i++) {
    p=sx+Ccls*(i+sy);
    for (j=0; j<sw; j++) {
      n=p+j;
      if (!Dfm[n]) {
#ifdef QT512
        Dws[n]=fm.horizontalAdvance(Data[n]);
#else
        Dws[n]=fm.width(Data[n]);
#endif
        Dhs[n]=fm.height();
        Twx[sx+j]=qMax(Twx[sx+j],Dws[n]);
        Thx[sy+i]=qMax(Thx[sy+i],Dhs[n]);
        Dfm[n]=true;
      }
    }
  }

  Dwx=Twx;
  Dhx=Thx;
  drawmergesizes();

  Gws=vadd(Mw,Lwx+Dwx);
  Ghs=vadd(Mh,Hhx+Dhx);
  Gwp=vsumscanp(0,Gws);
  Ghp=vsumscanp(0,Ghs);

  Gw=Gwp.last();
  Gh=Ghp.last();
  Hh=Ghp[Hrws];
  Lw=Gwp[Lcls];

  Cws=Gws.mid(Lcls);
  Chs=Ghs.mid(Hrws);
  Cw=Gw-Lw;
  Ch=Gh-Hh;
  Cwp=vadd(-Lw,Gwp.mid(Lcls));
  Chp=vadd(-Hh,Ghp.mid(Hrws));
}

// ---------------------------------------------------------------------
// nonce - merge is done now for all headers,
// and need only be done for visible headers
void WGrid::drawmergesizes()
{
  int h,i,j,n,p,s,w;

  if (HdrMerge==DefMerge) return;
  for (i=0; i<HdrMerge.size(); i++) {
    if (HdrMerge[i]>1) {
      n=HdrMerge[i];
      p=HdrMergeIndex[i];
      w=Hws[p];
      for (j=s=0; j<n; j++)
        s+=Dwx[j];
      if (w>s)
        for (j=0; j<n; j++) {
          if (s>0)
            Dwx[j]=1+(Dwx[j]*w)/s;
          else
            Dwx[j]=1+w/n;
        }
    }
  }

  if (LabMerge==DefMerge) return;
  for (i=0; i<LabMerge.size(); i++) {
    if (LabMerge[i]>1) {
      n=LabMerge[i];
      p=LabMergeIndex[i];
      h=Lhs[p];
      for (j=s=0; j<n; j++)
        s+=Dhx[j];
      if (h>s)
        for (j=0; j<n; j++) {
          if (s>0)
            Dhx[j]=1+(Dhx[j]*h)/s;
          else
            Dhx[j]=1+h/n;
        }
    }
  }
}
