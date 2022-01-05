
#include <QListWidget>

#include "base.h"
#include "fif.h"
#include "state.h"
#include "utils.h"

// using namespace std;

// ---------------------------------------------------------------------
void Fif::search()
{

  QStringList r;
  if (Search.isEmpty()) {
    finfo("No search defined");
    return;
  }
  r=searchdo();
  found->clear();
  found->addItems(r);

  if (!ifResults) {
#ifndef SMALL_SCREEN
    QSize s=size();
    resize(s.width(),qMax(Pos[3],s.height()+100));
#endif
    found->show();
    ifResults=true;
  }
}

// ---------------------------------------------------------------------
QStringList Fif::searchdo()
{
  QString file,txt;
  QStringList r;

  QString p=cpath(Path);
  QStringList files=folder_tree(p,Type,Subdir);

  if (files.isEmpty())
    return r;

  if (Matchcase)
    what=Search;
  else
    what=Search.toLower();

  ifRegex=Assign || Name || Regex;
  if (ifRegex) {
    if (Assign)
      pat.setPattern(config.Rxnna+what+rxassign(config.DefExt,false));
    else if (Name)
      pat.setPattern(config.Rxnna+what+config.Rxnnz);
    else
      pat.setPattern(what);
  }

  for (int i=0; i<files.size(); i++)
    r.append(searchfile(files.at(i)));

  int len=p.size()+1;
  for (int i=0; i<r.size(); i++)
    r.replace(i,r.at(i).mid(len));

  return r;
}

// ---------------------------------------------------------------------
QStringList Fif::searchfile(QString file)
{
  int n,p=0;
  QList<int> hit;

  QString dat,txt;
  QStringList r;

  dat=txt=cfread(file);
  if (txt.isEmpty()) return r;

  if (!Matchcase)
    txt=txt.toLower();

  if (Regex)
    hit=searchfilex(txt);
  else {
    if (ifRegex) {
      while (0<=(n=txt.indexOf(pat,p))) {
        hit.append(n);
        p=n+1;
      }
    } else {
      while (0<=(n=txt.indexOf(what,p))) {
        hit.append(n);
        p=n+1;
      }
    }
  }
  if (hit.size()==0) return r;
  return searchformat(file,hit,dat);
}

// ---------------------------------------------------------------------
// search files line by line for regex
QList<int> Fif::searchfilex(QString txt)
{
  int i,m,n,p=0;
  QList<int> r;
  QStringList s=txt.split('\n');

  for (i=0; i<s.size(); i++) {
    m=0;
    while (0<=(n=s.at(i).indexOf(pat,m))) {
      r.append(p+n);
      m=n+1;
    }
    p+=s.at(i).size()+1;
  }
  return r;
}

// ---------------------------------------------------------------------
QStringList Fif::searchformat(QString file,QList<int>hit, QString txt)
{
  QStringList r;

  if (Fileonly) {
    r.append(file+":"+QString::number(hit.size()));
    return r;
  }

  hit=lineindex(hit,txt);
  hit=removedups(hit);
  QStringList lines=txt.split("\n");
  foreach (int i,hit)
    r.append(QString::number(i) + ": " + lines.at(i));
  r=qslprependeach(file+":",r);
  return r;
}

// ---------------------------------------------------------------------
// convert hits into lines
QList<int> Fif::lineindex(const QList<int> hit,const QString txt)
{
  int i,p=0;
  QList<int> r;
  QList<int> end=lineends(txt);
  for (i=0; i<hit.size(); i++) {
    while (end.at(p)<hit.at(i))
      p++;
    r.append(p);
  }
  return r;
}

// ---------------------------------------------------------------------
// get text file line ends
QList<int> Fif::lineends(const QString txt)
{
  int n,p=0;
  QList<int>r;
  while (0<=(n=txt.indexOf('\n',p))) {
    r.append(n);
    p=n+1;
  }
  r.append(txt.size());
  return r;
}

// ---------------------------------------------------------------------
// remove dups from sorted list
QList<int> Fif::removedups(const QList<int> hit)
{
  int p,q;
  QList<int> r;
  if (hit.size()==0) return r;
  p=hit.at(0);
  r.append(p);
  for (int i=1; i<hit.size(); i++) {
    q=hit.at(i);
    if (p!=q) r.append((p=q));
  }
  return r;
}
