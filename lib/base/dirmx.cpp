
#include <QDateTime>
#include <QFileInfo>

#include "base.h"
#include "comp.h"
#include "dirm.h"
#include "eview.h"
#include "state.h"

// ---------------------------------------------------------------------
void Dirm::compareallfiles()
{
  QStringList r;
  for (int i=0; i<Diff.size(); i++) {
    if (i) r.append("");
    r.append(comparefile1(Diff.at(i)));
  }
  textview(r.join("\n"));
}

// ---------------------------------------------------------------------
void Dirm::comparefile()
{
  QString s=dmgetname2();
  if (s.isEmpty()) {
    dminfo("Nothing to compare");
    return;
  }
  QStringList r=comparefile1(s);
  textview(r.join("\n"));
}

// ---------------------------------------------------------------------
QStringList Dirm::comparefile1(QString s)
{
  QStringList r;
  QString t;
  int len,sx,tx;
  sx=Sourcex.size();
  tx=Targetx.size();
  len=2+qMax(sx,tx)+s.size();
  t=fcompare(Source+"/"+s,Target+"/"+s);
  r.append("comparing:");
  r.append(Sourcex+match_fmt1(s,Source,len-sx));
  r.append(Targetx+match_fmt1(s,Target,len-tx));
  r.append(t.mid(1+t.indexOf('\n')));
  return r;
}

// ---------------------------------------------------------------------
void Dirm::comparexdiff()
{
  QString s=dmgetname2();
  if (s.isEmpty()) {
    dminfo("Nothing to compare");
    return;
  }
  xdiff(Source+"/"+s,Target+"/"+s);
}

// ---------------------------------------------------------------------
void Dirm::copyall()
{
  QStringList s=NotInTarget + Diff;
  if (s.isEmpty())
    dminfo("No source files to copy");
  else
    copyfiles(s);
};

// ---------------------------------------------------------------------
void Dirm::copyfile()
{
  QString s=dmgetname1();
  if (s.isEmpty()) {
    dminfo("No file selected");
    return;
  }
  if (!matchhead(Source,s)) {
    dminfo("File not in Source directory");
    return;
  }

  QString w=s.mid(1+Source.size());
  QString t=Target + "/" + w;
  QString p="OK to copy from source to target:\n\n" + w;

  if (queryNY(Title,p))
    cfcopy(s,t);

  Diff.removeOne(w);
  NotInTarget.removeOne(w);
  match_refresh(false);
}

// ---------------------------------------------------------------------
void Dirm::copyfiles(QStringList s)
{
  QString m="OK to copy:\n\n" + s.join("\n");
  if (!queryNY(Title,m)) return;
  foreach (const QString f, s)
    copys2t(f);
  match_refresh(true);
}

// ---------------------------------------------------------------------
void Dirm::copylater()
{
  QStringList r;
  foreach (const QString s, Diff) {
    if (cftime(Source+"/"+s) > cftime(Target+"/"+s))
      r.append(s);
  }
  if (r.isEmpty())
    dminfo("No later source files to copy");
  else
    copyfiles(r);
};

// ---------------------------------------------------------------------
// copy file from source to target directory
void Dirm::copys2t(QString id)
{
  cfcopy(Source + "/" + id, Target + "/" + id);
};

// ---------------------------------------------------------------------
void Dirm::copysource()
{
  if (NotInTarget.isEmpty())
    dminfo("No source files to copy");
  else
    copyfiles(NotInTarget);
};

// ---------------------------------------------------------------------
void Dirm::ignorefile()
{
  QString s=dmgetname1();

  if (s.isEmpty()) {
    dminfo("No file selected");
    return;
  }

  QString w;
  if (matchhead(Source,s))
    w=s.mid(1+Source.size());
  else
    w=s.mid(1+Target.size());

  Diff.removeOne(w);
  NotInSource.removeOne(w);
  NotInTarget.removeOne(w);
  match_refresh(false);
}

// ---------------------------------------------------------------------
void Dirm::matches(bool done)
{
  Found.clear();
  dmread();

  if (Source.isEmpty()) {
    dminfo("Enter the source directory");
    return;
  }
  if (Target.isEmpty()) {
    dminfo("Enter the target directory");
    return;
  }
  if (Source==Target) {
    dminfo("Source and Target are the same");
    return;
  }
  if (match_do())
    match_fmt(done);
}

// ---------------------------------------------------------------------
bool Dirm::match_do()
{
  int i;
  QString filter;
  QStringList dx,dy,nx,ny;
  QFile fx;
  QFile fy;

  NotInSource.clear();
  NotInTarget.clear();
  Diff.clear();

  filter=config.DMTypex.at(TypeInx);
  dx=folder_tree(Source,filter,Subdir);
  dy=folder_tree(Target,filter,Subdir);

  if (dx.isEmpty()&&dy.isEmpty()) {
    dminfo("Both directories are empty");
    return false;
  }

  nx=qsldropeach(1+Source.size(),dx);
  ny=qsldropeach(1+Target.size(),dy);

  for (i=nx.size()-1; i>=0; i--)
    if (!ny.contains(nx.at(i))) {
      NotInTarget.append(nx.at(i));
      nx.removeAt(i);
    }

  for (i=ny.size()-1; i>=0; i--)
    if (!nx.contains(ny.at(i))) {
      NotInSource.append(ny.at(i));
      //ny.removeAt(i);
    }

  for (i=0; i<nx.size(); i++) {
    fx.setFileName(Source+"/"+nx.at(i));
    fy.setFileName(Target+"/"+nx.at(i));
    if (!(fx.size()==fy.size()&&cfread(&fx)==cfread(&fy)))
      Diff.append(nx.at(i));
  }

  return true;
}

// ---------------------------------------------------------------------
void Dirm::match_fmt(bool done)
{
  Q_UNUSED(done);

  int len=0;
  Found.clear();

  foreach (const QString &s, NotInSource)
    len=qMax(len,s.size());
  foreach (const QString &s, NotInTarget)
    len=qMax(len,s.size());
  foreach (const QString &s, Diff)
    len=qMax(len,s.size());
  len+=2;

  if (NotInSource.size()) {
    Found.append("not in source:");
    foreach (const QString &s, NotInSource)
      Found.append(match_fmt1(s,Target,len));
  }

  if (NotInTarget.size()) {
    if (Found.size()) Found.append("");
    Found.append("not in target:");
    foreach (const QString &s, NotInTarget)
      Found.append(match_fmt1(s,Source,len));
  }

  if (Diff.size()) {
    if (Found.size()) Found.append("");
    Found.append("different contents - source,target:");
    foreach (const QString &s, Diff)
      Found.append(match_fmt2(s,len));
    Found.removeLast();

  }

}

// ---------------------------------------------------------------------
QString Dirm::match_fmt1(QString s,QString d,int len)
{
  QString n,p,t;
  QFileInfo f(d+"/"+s);
  n=QString::number(f.size());
  t=f.lastModified().toString(Qt::ISODate);
  t.replace('T',' ');

  p.fill(' ',len-s.size());
  return s+p+t+"  "+n;
}

// ---------------------------------------------------------------------
QStringList Dirm::match_fmt2(QString s,int len)
{
  QStringList r;
  r.append(match_fmt1(s,Source,len));
  r.append(match_fmt1(s,Target,len));
  r.append("");
  return r;
}

// ---------------------------------------------------------------------
// force = force redo from scratch
// otherwise just reformat (e.g. with exclusions...)
void Dirm::match_refresh(int force)
{
  if (!matched) return;
  if (force)
    matches(true);
  else {
    dmread();
    match_fmt(true);
  }
  dmshowfind();
}
