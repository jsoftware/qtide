
#include <assert.h>
#include "qgrid.h"
#include "wgrid.h"
#include "../base/jsvr.h"

// ---------------------------------------------------------------------
QVector<int> axismerge(QList<int> a)
{
  int i,s=1;
  int n=a.size();

  QVector<int> r;
  QVector<int> p(n);

  p[n-1]=1;
  for (i=n-1; i>0; i--)
    p[i-1]=p[i]*AxisShape[a[i]];
  for (i=0; i<n; i++) {
    s*=AxisShape[a[i]];
    r+=QVector<int>(s,p[i]);
  }
  return r;
}

// ---------------------------------------------------------------------
QStringList axisrep(QStringList a,int p)
{
  QStringList r;
  for (int i=0; i<p; i++)
    r+=a;
  return r;
}

// ---------------------------------------------------------------------
QVector<int> axisreps(QList<int> a)
{
  QVector<int> r;
  int n=a.size()-1;
  r.append(1);
  for (int i=0; i<n; i++)
    r.append(r[i]*AxisShape[a[i]]);
  return r;
}

// ---------------------------------------------------------------------
int axislength(QList<int> a)
{
  int r=AxisShape[a[0]];
  for (int i=1; i<a.size(); i++)
    r*=AxisShape[a[i]];
  return r;
}

// ---------------------------------------------------------------------
void WGrid::getgrid()
{
  int i;
  I len, *vec=0;
  QString arg;
  QList<int> ndx;
  QStringList data,hdr,hdrx,lab,labx;
  QVector<int> hdrmerge,labmerge;

  int rws=axislength(AxisRows);
  QVector<int>rwscopy=axismerge(AxisRows);
  QVector<int>rwsreps=axisreps(AxisRows);

  int cls=axislength(AxisCols);
  QVector<int>clscopy=axismerge(AxisCols);
  QVector<int>clsreps=axisreps(AxisCols);

  foreach (i, AxisSlice)
  ndx.append(AxisIndex[i]);
  arg=intlist2qs(AxisRows)+ ";" +  intlist2qs(AxisCols) + ";";
  if (AxisSlice.isEmpty())
    arg+="'';'';";
  else
    arg+=intlist2qs(AxisSlice) + ";" +  intlist2qs(ndx) + ";";
  arg+=intlist2qs(AxisShape);
  assert(doriv(q2s("gridindex_jqtide_ "+arg),&vec,&len));
  int ilen=(int)len;
  assert(ilen==rws*cls);
  for (i=0; i<ilen; i++)
    data.append(CellData[vec[i]]);

  for (i=0; i<AxisRows.size(); i++) {
    lab+=axisrep(AxisLabels[AxisRows[i]],rwsreps[i]);
    labmerge=rwscopy;
  }

  for (i=0; i<AxisCols.size(); i++) {
    hdr+=axisrep(AxisLabels[AxisCols[i]],clsreps[i]);
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
}
