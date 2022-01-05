
#include <QSettings>

#include "base.h"
#include "proj.h"
#include "svr.h"
#include "snap.h"
#include "state.h"
#include "recent.h"

// using namespace std;

Project project;

QStringList getfolderdefs(QString d);
void project_init();
void setfolder(QString s);

// ---------------------------------------------------------------------
bool Project::closeOK()
{
  return true;
}

// ---------------------------------------------------------------------
void Project::close()
{
  Id.clear();
  Path.clear();
  recent.ProjectOpen=false;
}

// ---------------------------------------------------------------------
QString Project::fullname(QString s)
{
  Q_ASSERT(s.size() > 0);
  if (isroot(s)) return s;
  if (s.at(0) == '~') return cpath(s);
  return Path + "/" + s;
}

// ---------------------------------------------------------------------
QString Project::id2qproj(QString s)
{
  return cpath("~" + s) + "/" + cfsname(s) + config.ProjExt;
}

// ---------------------------------------------------------------------
// initialize most recent project, if possible
void Project::init()
{
  QFile f;
  QString id,lastid,t;
  QStringList s;

  config.ProjInit=true;

  while (recent.Projects.size()>0) {
    id = recent.Projects.at(0).at(0);
    if (cfexist(id2qproj(id)))
      break;
    recent.Projects.removeFirst();
  }

  while (recent.Projects.size()>1) {
    lastid = recent.Projects.at(1).at(0);
    if (cfexist(id2qproj(lastid)))
      break;
    recent.Projects.removeAt(1);
  }

  if (recent.ProjectOpen) {
    if (id.size()) {
      open(id);
      LastId=lastid;
    }
  } else
    recent.ProjectOpen=false;
}

// ---------------------------------------------------------------------
void Project::open(QString id)
{
  recent.ProjectOpen=true;
  if (id==Id) return;
  if (Id.size()>0)
    LastId=Id;
  Id=id;
  Folder=qstaketo(Id,"/");
  Path=cpath("~" + Id);
  if (config.ifGit)
    Git= gitstatus(Path).size() > 0;
  snapshot_tree(Folder);
}

// ---------------------------------------------------------------------
QString Project::projfile()
{
  if (Id.isEmpty()) return "";
  return Path + "/" + cfsname(Path) + config.ProjExt;
}

// ---------------------------------------------------------------------
// J toprojectfolder
QString Project::projectname(QString s)
{
  QString r;
  r=projectname1(s,config.UserFolderKeys,config.UserFolderValues);
  if (r != s) return r;
  return projectname1(s,config.AllFolderKeys,config.AllFolderValues);
}

// ---------------------------------------------------------------------
QString Project::projectname1(QString s,QStringList k,QStringList v)
{
  int i;
  QList<int> n;
  QString f,p;
  for (i=0; i<v.size(); i++)
    if (matchfolder(v.at(i),s)) n.append(i);
  if (n.size() == 0) return s;
  if (n.size() > 1)
    for (i=0; i>n.size(); i++)
      if (k.at(n.at(i)).size() > k.at(n.at(0)).size())
        n.replace(0,n.at(i));
  f=k.at(n.at(0));
  p=cpath("~" + f);
  return f + s.mid(p.size());
}

// ---------------------------------------------------------------------
void Project::save(QStringList tablist)
{
  if (Id.isEmpty()) return;
  QStringList s = tablist;
  s.prepend(Id);
  recent.projectset(s);
}

// ---------------------------------------------------------------------
QStringList Project::source()
{
  QString p=projfile();
  if (p.isEmpty()) return (QStringList)"";
  return cfreadx(p);
}
