
#include <assert.h>
#include "qgrid.h"
#include "wgrid.h"
#include "../base/jsvr.h"

// ---------------------------------------------------------------------
bool WGrid::getgrid()
{
  if (p->AxisLabels.isEmpty() ||  p->AxisNames.isEmpty() ||
      p->AxisOrder.isEmpty() ||  p->AxisShape.isEmpty() ||
      p->CellData.isEmpty() ) return false;

  int i;
  I len, *vec=0;
  QString arg;
  QList<int> ndx;
  QStringList data,hdr,hdrx,lab,labx;
  QVector<int> hdrmerge,labmerge;

  int rws=axislength(p->AxisRows);
  QVector<int>rwscopy=axismerge(p->AxisRows);
  QVector<int>rwsreps=axisreps(p->AxisRows);

  int cls=axislength(p->AxisCols);
  QVector<int>clscopy=axismerge(p->AxisCols);
  QVector<int>clsreps=axisreps(p->AxisCols);

  foreach (i, p->AxisSlice)
    ndx.append(p->AxisIndex[i]);
  arg=intlist2qs(p->AxisRows)+ ";" +  intlist2qs(p->AxisCols) + ";";
  if (p->AxisSlice.isEmpty())
    arg+="'';'';";
  else
    arg+=intlist2qs(p->AxisSlice) + ";" +  intlist2qs(ndx) + ";";
  arg+=intlist2qs(p->AxisShape);
  if (!doriv(q2s("gridindex_jqtide_ "+arg),&vec,&len))
    return false;
  int ilen=(int)len;
  assert(ilen==rws*cls);
  for (i=0;
       i<ilen;
       i++)
    data.append(p->CellData[vec[i]]);

  for (i=0;
       i<p->AxisRows.size();
       i++) {
    lab+=axisrep(p->AxisLabels[p->AxisRows[i]],rwsreps[i]);
    labmerge=rwscopy;
  }

  for (i=0;
       i<p->AxisCols.size();
       i++) {
    hdr+=axisrep(p->AxisLabels[p->AxisCols[i]],clsreps[i]);
    hdrmerge=clscopy;
  }

  Ccls=cls;
  Crws=rws;
  Hdr=hdr;
  HdrMerge=hdrmerge;
  HdrX=expandstringlist(Hdr,HdrMerge);

  Lab=lab;
  LabMerge=labmerge;
  LabX=expandstringlist(Lab,LabMerge);

  Data=data;
  return true;
}

// ---------------------------------------------------------------------
int WGrid::axislength(QList<int> a)
{
  int r=p->AxisShape[a[0]];
  for (int i=1; i<a.size(); i++)
    r*=p->AxisShape[a[i]];
  return r;
}

// ---------------------------------------------------------------------
QVector<int> WGrid::axismerge(QList<int> a)
{
  int i,s=1;
  int n=a.size();

  QVector<int> r;
  QVector<int> t(n);

  t[n-1]=1;
  for (i=n-1; i>0; i--)
    t[i-1]=t[i]*p->AxisShape[a[i]];
  for (i=0; i<n; i++) {
    s*=p->AxisShape[a[i]];
    r+=QVector<int>(s,t[i]);
  }
  return r;
}

// ---------------------------------------------------------------------
QStringList WGrid::axisrep(QStringList a,int p)
{
  QStringList r;
  for (int i=0; i<p; i++)
    r+=a;
  return r;
}

// ---------------------------------------------------------------------
QVector<int> WGrid::axisreps(QList<int> a)
{
  QVector<int> r;
  int n=a.size()-1;
  r.append(1);
  for (int i=0; i<n; i++)
    r.append(r[i]*p->AxisShape[a[i]]);
  return r;
}
