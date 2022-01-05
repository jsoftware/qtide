// compare

#include "base.h"
#include "comp.h"

/*
 * x,y,XY = original and combined std::stringlists
 * AX = indices of x in XY
 * X  = items of x not yet accounted for
 * NX = indices of x not yet accounted for
 * SX = indices of x not matched
*/

// using namespace std;

bool qvcontainseach(QVector<int>v,QVector<int>w);
QVector<int> qvlastseq(QVector<int>v,QVector<int>w);
int qvremseq(QVector<int>x,QVector<int>nx,QVector<int>p,QVector<int>*s);

// ---------------------------------------------------------------------
QString Compare::comp(QStringList x,QStringList y)
{
  int i,m,n;
  QStringList r,rx,ry;

  XY=x+y;
  n=x.size();
  if (n==0) return "empty left argument";
  X.resize(n);
  for (i=0; i<n; i++)
    X.replace(i,XY.indexOf(x.at(i)));
  AX=X;
  NX.resize(n);
  for (i=0; i<n; i++)
    NX.replace(i,i);

  n=y.size();
  if (n==0) return "empty right argument";
  Y.resize(n);
  for (i=0; i<n; i++)
    Y.replace(i,XY.indexOf(y.at(i)));
  AY=Y;
  NY.resize(n);
  for (i=0; i<n; i++)
    NY.replace(i,i);

  while (compend()) complcs();

#ifdef QT57
  std::sort(SX.begin(), SX.end());
  std::sort(SY.begin(), SY.end());
#else
  qSort(SX);
  qSort(SY);
#endif

  for (i=0; i<SX.size(); i++) {
    n=SX.at(i);
    rx.append("0 [" + QString::number(n) + "] " + XY.at(AX.at(n)));
  }

  for (i=0; i<SY.size(); i++) {
    n=SY.at(i);
    ry.append("1 [" + QString::number(n) + "] " + XY.at(AY.at(n)));
  }

  for (i=0; i<SX.size(); i++)
    SX.replace(i,SX.at(i)*2);

  for (i=0; i<SY.size(); i++)
    SY.replace(i,1+SY.at(i)*2);

  SX=SX+SY;
#ifdef QT57
  std::sort(SX.begin(), SX.end());
#else
  qSort(SX);
#endif
  m=n=0;
  for (i=0; i<SX.size(); i++)
    if (SX.at(i)%2)
      r.append(ry.at(n++));
    else
      r.append(rx.at(m++));
  return r.join("\n");
}

// ---------------------------------------------------------------------
// repeats until no change:
// remove same head and tails
// remove different lines
// return true if continue with lcs
bool Compare::compend()
{
  int i,m,n,t;
  int ox=0,oy=0;
  int nx=X.size(),ny=Y.size();
  QVector<int> p;

  while (ox != nx || oy != ny) {
    ox=nx;
    oy=ny;
    t=qMin(nx,ny);
    if (t == 0) break;

// remove head and tail matches
    for (m=0; m<t; m++)
      if (X.at(m) != Y.at(m)) break;
    X = X.mid(m);
    Y = Y.mid(m);
    NX = NX.mid(m);
    NY = NY.mid(m);
    nx=X.size();
    ny=Y.size();

    t -= m;
    for (n=0; n<t; n++)
      if (X.at(nx-1-n) != Y.at(ny-1-n)) break;
    if (n) {
      X = X.mid(0,nx-n);
      Y = Y.mid(0,ny-n);
      NX = NX.mid(0,nx-n);
      NY = NY.mid(0,ny-n);
    }

// remove lines not in both
    p.clear();
    for (i=0; i<X.size(); i++)
      if (!Y.contains(X.at(i))) {
        p.append(i);
        SX.append(NX.at(i));
      }
    for (i=p.size(); i>0; i--) {
      X.remove(p.at(i-1));
      NX.remove(p.at(i-1));
    }
    p.clear();
    for (i=0; i<Y.size(); i++)
      if (!X.contains(Y.at(i))) {
        p.append(i);
        SY.append(NY.at(i));
      }
    for (i=p.size(); i>0; i--) {
      Y.remove(p.at(i-1));
      NY.remove(p.at(i-1));
    }

    nx=X.size(),ny=Y.size();
  }

  if (nx != 0 && ny != 0) return true;

  SX = SX + NX;
  SY = SY + NY;
  return false;
}

// ---------------------------------------------------------------------
void Compare::complcs()
{
  int m,n;
  QVector<int> w;

  w=seqlcs(X,Y);
  m=qvremseq(X,NX,w,&SX);
  X=X.mid(m);
  NX=NX.mid(m);

  n=qvremseq(Y,NY,w,&SY);
  if ((m==0) && (n==0)) n++;
  Y=Y.mid(n);
  NY=NY.mid(n);
}

// ---------------------------------------------------------------------
QVector<int> Compare::seqlcs(QVector<int> x,QVector<int> y)
{
  int i,j;
  int mx=100;
  QVector<int> r;

  int m=std::min(mx,(int)x.size());
  int n=std::min(mx,(int)y.size());

  x = x.mid(0,m);
  y = y.mid(0,n);

  if (m < n) {
    std::swap(x,y);
    std::swap(m,n);
  }

  std::vector< std::vector<int> > c(m+1, std::vector<int>(n+1,0));

  for (i=0; i<m; ++i)
    for (j=0; j<n; ++j)
      if (x.at(i) == y.at(j))
        c[i+1][j+1] = c[i][j] + 1;
      else
        c[i+1][j+1] = std::max(c[i+1][j], c[i][j+1]);

  i = c[m][n];
  r.resize(i);
  while (m > 0 && n > 0) {
    if (x.at(m-1) == y.at(n-1) && n--)
      r.replace(--i,x.at(--m));
    else
      c[m][n-1] >= c[m-1][n] ? n-- : m--;
  }

  return r;
}

// ---------------------------------------------------------------------
QString compare(QStringList s,QStringList t)
{
  Compare n;
  return n.comp(s,t);
}

// ---------------------------------------------------------------------
QString fcompare(QString s, QString t)
{
  Compare n;
  QString r=n.comp(cfreads(s),cfreads(t));
  QString p="comparing:\n";
  return p + r;
}

// ---------------------------------------------------------------------
// qvector contains on list
bool qvcontainseach(QVector<int>v,QVector<int>w)
{
  for (int i=0; i<w.size(); i++)
    if (!v.contains(w.at(i))) return false;
  return true;
}

// ---------------------------------------------------------------------
// get positions of lcs sequence in vector
// starts from front with last value
// that could form part of the lcs
QVector<int> qvlastseq(QVector<int>v,QVector<int>w)
{
  int i,n=w.size();
  QVector<int> r;
  r.resize(n);

  int p=v.indexOf(w.first());
  if (n==1) {
    r.replace(0,p);
    return r;
  }

  while (true) {
    i=v.indexOf(w.first(),p+1);
    if (i<0 || ! qvcontainseach(v.mid(i),w)) break;
    p=i;
  }

  r.replace(0,p);
  for (i=1; i<n; i++)
    r.replace(i,v.indexOf(w.at(i),r.at(i-1)));

  return r;
}

// ---------------------------------------------------------------------
// return drop amount on x,nx
int qvremseq(QVector<int>x,QVector<int>nx,QVector<int>w,QVector<int>*s)
{
  int e,i;
  QVector<int> p;
  p=qvlastseq(x,w);
  e=p.last();
  for (i=0; i<e; i++)
    if (!p.contains(i))
      s->append(nx.at(i));
  return e+1;
}
