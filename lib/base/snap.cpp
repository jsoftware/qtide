#include <QDate>

#include "base.h"
#include "snap.h"
#include "note.h"
#include "proj.h"
#include "state.h"
#include "recent.h"

// using namespace std;

QStringList SnapTrees;

bool snapfcopy(QString from,QString to);
bool snapshot1(bool,QString,QString);
bool ss_erase(QString p,QString s);
QStringList ss_files(QString d);
bool ss_make(QString from,QString to);
bool ss_match(QString p, QString q);

// ---------------------------------------------------------------------
bool snapfcopy(QString from,QString to)
{
  QFile f(from);
  return f.copy(to);
}

// ---------------------------------------------------------------------
QString snapgetpath(QString p)
{
  QString r = snappath(p);
  QDir d;
  QFile *f;
  if (!d.exists(r)) {
    d.mkpath(r);
    f = new QFile(r + "/dir.txt");
    cfwrite(f,p);
  }
  return r;
}

// ---------------------------------------------------------------------
QString snappath(QString p)
{
  return config.SnapPath.filePath(".snp/" + getsha1(p));
}

// ---------------------------------------------------------------------
bool snaprmdir(QString p)
{
  if (!matchhead(config.SnapPath.absolutePath(),p)) return false;
  return cfrmdir(p);
}

// ---------------------------------------------------------------------
void snapshot(bool force, QString path)
{
  if (config.Snapshots==0) return;
  snapshot1(force,ss_date(),path);
}

// ---------------------------------------------------------------------
bool snapshot1(bool force, QString today, QString path)
{
  QString p=snapgetpath(path);
  if (p.size() == 0) return true;
  p += "/";
  QString pfx = p + "s" + today;
  QString seq = "001";
  QDir qdir(p);
  QStringList d;
  QStringList f;
  bool donetoday=false;

  f.append("s*");
  d=qdir.entryList(f);

  if (d.size()) {
    d.sort();
    donetoday = d.last().mid(1,6) == today;
  }

// force snap
  if (force) {
    if (d.size()>0 && ss_match(p + d.last(),path)) {
      ss_info("Last snapshot matches current project.");
      return true;
    }
    if (donetoday) {
      QString t(QString("000%1").arg(1+d.last().right(3).toInt()));
      seq=t.right(3);
    }
    ss_make(path,pfx + seq);
    ss_info("New snapshot: " + cfsname(pfx + seq));

  }
// auto snap
  else {
    if (donetoday) return true;
    if (d.size()>0 && ss_match(p + d.last(),path)) return true;
    ss_make(path,pfx + seq);
  }

  for (int i=0; i<d.size()-config.Snapshots; i++)
    ss_erase(p,d.at(i));

  return true;
}

// ---------------------------------------------------------------------
void snapshot_tree(QString folder)
{
  if (config.Snapshots == 0 || folder.size() == 0) return;
  if (SnapTrees.contains(folder)) return;
  QString p=cpath("~" + folder);
  QStringList folders=project_tree(p);
  QString t=ss_date();
  for (int i=0; i<folders.size(); i++)
    snapshot1(false,t,p + "/" + folders.at(i));
  SnapTrees.append(folder);
}

// ---------------------------------------------------------------------
QString ss_date()
{
  return QDate::currentDate().toString("yyMMdd");
}

// ---------------------------------------------------------------------
bool ss_erase(QString p,QString s)
{
  return snaprmdir(p + s);
}

// ---------------------------------------------------------------------
QStringList ss_files(QString s)
{
  QStringList y=cflist(s,"");

  if (config.Snapshotx.isEmpty()) return y;

  QStringList x=cflist(s,config.Snapshotx);
  QStringList r;
  foreach (QString s,y)
    if (!x.contains(s))
      r << s;
  return r;
}

// ---------------------------------------------------------------------
void ss_info(QString s)
{
  info("Snapshot",s);
}

// ---------------------------------------------------------------------
QStringList ss_list(QString p)
{
  QDir d(p);
  QStringList t=d.entryList(QStringList() << "s*");
  t.sort();
  return qslreverse(t);
}

// ---------------------------------------------------------------------
// ss_make - make a snapshot
bool ss_make(QString from, QString to)
{
  from += "/";
  to += "/";

  if (!ss_mkdir(to)) return false;
  QStringList f=ss_files(from);
  for (int i=0; i<f.size(); i++)
    snapfcopy(from+f.at(i),to+f.at(i));
  return true;
}

// ---------------------------------------------------------------------
bool ss_match(QString p, QString q)
{
  QStringList pf=ss_files(p);
  QStringList qf=ss_files(q);
  if (pf.size() != qf.size()) return false;
  pf.sort();
  qf.sort();
  QFile *pn, *qn;
  bool match;
  for (int i=0; i<pf.size(); i++) {
    pn=new QFile(p + "/" + pf.at(i));
    qn=new QFile(q + "/" + pf.at(i));
    match = (cfread(pn) == cfread(qn));
    delete pn;
    delete qn;
    if (!match) return false;
  }
  return true;
}

// ---------------------------------------------------------------------
bool ss_mkdir(QString s)
{
  QDir d(s);
  if (d.exists() || d.mkpath(s))
    return true;
  info("Snap","Unable to create snapshot directory: " + s);
  return false;
}
