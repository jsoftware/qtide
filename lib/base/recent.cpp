#include <iostream>
#include <sstream>
#include <QFile>
#include <QSettings>
#include <QTextStream>

#include "base.h"
#include "util.h"
#include "state.h"
#include "recent.h"
#include "dlog.h"
#include "proj.h"
#include "svr.h"

// using namespace std;

Recent recent;

// ---------------------------------------------------------------------
// add to list on create, open, save
void Recent::filesadd(QString f)
{
  f=cfcase(f);
  Files.removeOne(f);
  Files.prepend(f);
  if (Files.size() > config.MaxRecent)
    Files=Files.mid(0,config.MaxRecent);
  save_recent();
}

// ---------------------------------------------------------------------
// inits for recent.dat and project.dat
void Recent::init()
{
  RecentFile = config.ConfigPath.filePath("recent.dat");
  ProjectFile = config.ConfigPath.filePath("project.dat");

  QSettings s(RecentFile,QSettings::IniFormat);
  DirMatch = qslfcase(s.value("Recent/DirMatch", "").toStringList());
  recentFif = s.value("Recent/Fif", "").toStringList();
  Files = qslfcase(s.value("Recent/Files", "").toStringList());
  ProjectOpen = s.value("Recent/ProjectOpen", "true").toBool();

  QStringList t = cfreadx(ProjectFile);
  QStringList p;
  for (int i=0; i<t.size(); i++) {
    p=t.at(i).split('|',_SkipEmptyParts);
    if (cfexist(project.id2qproj(p[0])))
      Projects.append(p);
  }
}

// ---------------------------------------------------------------------
QStringList Recent::projectget(QString id)
{
  QStringList s;
  s.append(id);
  s.append("-1");
  int n=Projects.size();
  for (int i=0; i<n; i++) {
    if (id == Projects.at(i).first()) {
      s=Projects.at(i);
      break;
    }
  }
  projectset(s);
  return s;
}

// ---------------------------------------------------------------------
void Recent::projectset(QStringList s)
{
  QString id, t;
  id=s.first();
  int n=Projects.size();
  for (int i=0; i<n; i++)
    if (id == Projects.at(i).first()) {
      Projects.removeAt(i);
      break;
    }
  Projects.prepend(s);
  Projects=Projects.mid(0,config.MaxRecent);
  save_project();
}

// ---------------------------------------------------------------------
void Recent::save_project()
{
  QString t;
  for (int i=0; i<Projects.size(); i++)
    t.append(Projects.at(i).join("|") + "\n");
  cfwrite(ProjectFile,t);
}

// ---------------------------------------------------------------------
void Recent::save_recent()
{
  QSettings s(RecentFile,QSettings::IniFormat);
  s.setValue("Recent/DirMatch",DirMatch);
  s.setValue("Recent/Fif",recentFif);
  s.setValue("Recent/Files",Files);
  s.setValue("Recent/ProjectOpen",ProjectOpen);
}
