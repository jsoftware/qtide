// qutil.cpp

#include <limits>
#include <string>
#include <QDebug>

#include "qutil.h"
#include "../base/util.h"

static QVector<int> Vone (QVector<int>() << 1);

// ---------------------------------------------------------------------
int getcellcount(int w, int off, QVector<int> len)
{
  int i=off;
  int m=0;
  int n=len.size();
  while (i<n) {
    m+=len[i++];
    if (m>=w) break;
  }
  return qMin(n,i)-off;
}

// ---------------------------------------------------------------------
int getindexof(int w, QVector<int> p)
{
  int r;
  for (r=1; r<p.size(); r++)
    if (w<=p[r]) break;
  return r-1;
}

// ---------------------------------------------------------------------
bool griderror(QString s)
{
  info ("Grid",s);
  return false;
}

// ---------------------------------------------------------------------
QStringList expandstringlist(QStringList v,QVector<int> s)
{
  if (v.size()==1 || s==Vone) return v;
  int i,j;
  QStringList r;
  for (i=0; i<s.size(); i++) {
    r.append(v[i]);
    for (j=1; j<s[i]; j++)
      r.append("");
  }
  return r;
}

// ---------------------------------------------------------------------
QVector<int> expandvectorint(QVector<int> v,QVector<int> s)
{
  if (v.size()==1 || s==Vone) return v;
  int i,j;
  QVector<int> r;
  for (i=0; i<s.size(); i++) {
    r.append(v[i]);
    for (j=1; j<s[i]; j++)
      r.append(0);
  }
  return r;
}

// ---------------------------------------------------------------------
QVector<int> hdrmergeindex(QVector<int> m)
{
  return vsumscanp(0,m);
}

// ---------------------------------------------------------------------
QVector<int> hdrmergemask(QVector<int> m)
{
  int i,j;
  QVector<int> r;
  for (i=0; i<m.size(); i++)
    if (m[i]==1)
      r.append(1);
    else
      for (j=0; j<m[i]; j++)
        r.append(0);
  return r;
}

// ---------------------------------------------------------------------
QVector<int> qlist2ints(QStringList s)
{
  QVector<int> r;
  for (int i=0; i<s.size(); i++)
    r.append(s[i].toInt());
  return r;
}

// ---------------------------------------------------------------------
int maxscroll(int w,QVector<int> v)
{
  if (v.isEmpty()) return 0;
  int r;
  int n=v.size()-1;
  int m=v[n];
  for (r=n; r>0; r--) {
    m+=v[r-1];
    if (m>w) break;
  }
  return r;
}

// ---------------------------------------------------------------------
QVector<int> mcolmax(int cls,QVector<int> v)
{
  int i,j,p;
  int rws=v.size()/cls;
  QVector<int> r=v.mid(0,cls);
  for (i=1; i<rws; i++) {
    p=i*cls;
    for (j=0; j<cls; j++)
      r[j]=qMax(r[j],v[p+j]);
  }
  return r;
}

// ---------------------------------------------------------------------
QVector<int> mcolsum(int cls,QVector<int> v)
{
  int i,j,p;
  int rws=v.size()/cls;
  QVector<int> r=v.mid(0,cls);
  for (i=1; i<rws; i++) {
    p=i*cls;
    for (j=0; j<cls; j++)
      r[j]=r[j]+v[p+j];
  }
  return r;
}

// ---------------------------------------------------------------------
QVector<int> mrowmax(int cls,QVector<int> v)
{
  int i,j,p;
  int rws=v.size()/cls;
  QVector<int> r(rws,std::numeric_limits<int>::min());
  for (i=0; i<rws; i++) {
    p=i*cls;
    for (j=0; j<cls; j++)
      r[i]=qMax(r[i],v[p+j]);
  }
  return r;
}

// ---------------------------------------------------------------------
QVector<int> mrowsum(int cls,QVector<int> v)
{
  int i,j,p;
  int rws=v.size()/cls;
  QVector<int> r(rws,0);
  for (i=0; i<rws; i++) {
    p=i*cls;
    for (j=0; j<cls; j++)
      r[i]=r[i]+v[p+j];
  }
  return r;
}

// ---------------------------------------------------------------------
int roundint(double d)
{
  return d+0.5;
}

// ---------------------------------------------------------------------
std::string v2s(QVector<int> v)
{
  std::string r;
  int n=v.size();
  if (n==0) return r;
  for (int i=0; i<n; i++)
    r+=i2s(v[i])+" ";
  r.erase(r.size()-1);
  return r;
}

// ---------------------------------------------------------------------
QVector<int> vadd(int b,QVector<int> v)
{
  int n=v.size();
  for (int i=0; i<n; i++)
    v[i]+=b;
  return v;
}

// ---------------------------------------------------------------------
bool validmerge(int cls, QVector<int> m)
{
  int i,j;
  for (i=0; i<m.size(); i++)
    if (m[i]<1)
      for (j=1; j<m[i]; j++)
        if (0==(i+j)%cls)
          return false;
  return true;
}

// ---------------------------------------------------------------------
int viewlength(QVector<int>d, int s, int w)
{
  int r;
  int m=0;
  int n=d.size()-1;
  for (r=s; r<n; r++) {
    m+=d[r];
    if (m>=w) break;
  }
  return 1+r-s;
}

// ---------------------------------------------------------------------
int vindex(int i, QVector<int> v)
{
  if (v.size()==1)
    return v[0];
  else
    return v[i];
}

// ---------------------------------------------------------------------
QVector<int> vmax(int t, QVector<int> v)
{
  if (v.size()==0) return v;
  QVector<int> r=v;
  for (int i=0; i<v.size(); i++)
    r[i]=qMax(t,v[i]);
  return r;
}

// ---------------------------------------------------------------------
QVector<int> vmax(QVector<int> t, QVector<int> v)
{
  if (t.size()==0) return v;
  QVector<int> r=t;
  for (int i=0; i<v.size(); i++)
    r[i]=qMax(r[i],v[i]);
  return r;
}

// ---------------------------------------------------------------------
QVector<int> vmult(int m, QVector<int> v)
{
  int n=v.size();
  QVector<int> r(n);
  for (int i=0; i<n; i++)
    r[i]=m*v[i];
  return r;
}

// ---------------------------------------------------------------------
QVector<int> vmult(QVector<int> u,QVector<int> v)
{
  if (u.size()==1)
    return vmult(u[0],v);
  if (v.size()==1)
    return vmult(v[0],u);
  int n=u.size();
  QVector<int> r(n);
  for (int i=0; i<n; i++)
    r[i]=u[i]*v[i];
  return r;
}

// ---------------------------------------------------------------------
int vsum(int b,QVector<int> v)
{
  int r=b;
  int n=v.size();
  for (int i=0; i<n; i++)
    r+=v[i];
  return r;
}

// ---------------------------------------------------------------------
QVector<int> vshape(int s, QVector<int>v)
{
  int n=v.size();
  QVector<int> r(s);
  for (int i=0; i<s; i++)
    r[i]=v[i%n];
  return r;
}

// ---------------------------------------------------------------------
QVector<int> vsumscan(int b,QVector<int> v)
{
  int n=v.size();
  QVector<int> r(n);
  if (n==0) return r;
  r[0]=b+v[0];
  for (int i=1; i<n; i++)
    r[i]=r[i-1]+v[i];
  return r;
}

// ---------------------------------------------------------------------
QVector<int> vsumscanp(int b,QVector<int> v)
{
  int n=v.size();
  QVector<int> r(n+1);
  r[0]=b;
  for (int i=0; i<n; i++)
    r[i+1]=r[i]+v[i];
  return r;
}

