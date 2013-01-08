/* J state */

#include <QApplication>
#include <QDate>
#include <QSettings>

#include "base.h"
#include "state.h"
#include "dlog.h"
#include "jsvr.h"
#include "menu.h"
#include "note.h"
#include "proj.h"
#include "recent.h"
#include "svr.h"
#include "term.h"

using namespace std;

Config config;
QString LibName;

// ---------------------------------------------------------------------
// copy over configs from addons if necessary
void Config::config_init()
{
  QStringList c;
  c << "dirmatch.cfg" << "launch.cfg" << "pacman.cfg" << "qtide.cfg";
  QString s=cpath("~addons/ide/qt/config/");
  foreach (QString f,c)
  if ((!cfexist(ConfigPath.filePath(f)) && cfexist(s+f)))
    cfwrite(ConfigPath.filePath(f),cfread(s+f));
}

// ---------------------------------------------------------------------
// AllFolders is everything
// UserFolders is user only
void Config::folder_init()
{
  QString s;
  QStringList k,v;

  k=dors("}.;',',&.>{.\"1 UserFolders_j_").split(",");
  v=dors("}.;',',&.>{:\"1 UserFolders_j_").split(",");

  UserFolderKeys=k;
  UserFolderValues=v;

  k+=dors("}.;',',&.>{.\"1 SystemFolders_j_").split(",");
  v+=dors("}.;',',&.>{:\"1 SystemFolders_j_").split(",");

  AllFolderKeys=k;
  AllFolderValues=v;

  ConfigPath.setPath(cpath("~config"));
  SnapPath.setPath(cpath("~snap"));
  SystemPath.setPath(cpath("~install"));
  TempPath.setPath(cpath("~temp"));
  UserPath.setPath(cpath("~user"));

  createdir(SnapPath);
  createdir(TempPath);
  createdir(UserPath);

}

// ---------------------------------------------------------------------
void Config::init(QString path)
{
  Q_UNUSED(path);

  Q_ASSERT(jcon);

  DefCmt="NB.";
  DefExt=".ijs";
  DefIndent="   ";
  DefTypes << "Scripts ijs"
           << "Text txt"
           << "SourceText ijs,ijt,jproj,txt"
           << "All *";

  FilePatterns="Scripts (*.ijs);;All files (*.*)";
  ProjExt = ".jproj";

#ifdef _WIN32
  ProFont.setFamily("\"MS Sans Serif\" 10");
#else
  ProFont.setFamily("SansSerif 10");
#endif

  LineNos = false;
  LineWrap = false;
  ScriptFilter="*.ijs";

  Rxnna = "\\b";
  Rxnnz = "\\b";

  Ascii="+"==dors("{.9!:6$0");

  NoProfile="0"!=dors("\":~. 4!:0 ;:'SystemFolders_j_ UserFolders_j_'");
  if (NoProfile) {
    noprofile();
    return;
  }

  folder_init();
  config_init();
  launch_init();
  winpos_init();

  if ("0"==dors("\":4!:0 <'XDiff_j_'"))
    XDiff=dors("XDiff_j_");

  QSettings s(ConfigPath.filePath("qtide.cfg"),QSettings::IniFormat);
  QString t;

  ConfirmClose = s.value("Session/ConfirmClose",true).toBool();
  ConfirmSave = s.value("Session/ConfirmSave",true).toBool();
  Font.setFamily(s.value("Session/FontFamily","").toString());
  Font.setPointSize(s.value("Session/FontSize",10).toInt());
  KeepInputLog = s.value("Session/KeepInputLog",true).toBool();
  MaxRecent = s.value("Session/MaxRecent",15).toInt();
  Snapshots = s.value("Session/Snapshots",true).toInt();
  Snapshotx = s.value("Session/Snapshotx","").toString();

  t= s.value("Position/Edit","550 0 500 500").toString();
  q2p(t,EditPos);
  t= s.value("Position/Term","0 0 500 500").toString();
  q2p(t,TermPos);

  FifExt = s.value("FindinFiles/Extensions","").toStringList();
  Terminal = s.value("Run/Terminal","").toString();
}

// ---------------------------------------------------------------------
void Config::noprofile()
{
  ConfirmClose = false;
  ConfirmSave = false;
  Font.setFamily("monospace");
  Font.setPointSize(12);
  q2p("0 0 500 500",TermPos);
  term->menuBar->hide();
}

// ---------------------------------------------------------------------
void Config::toggleascii()
{
  Ascii=!Ascii;
  term->menuBar->viewasciiAct->setChecked(Ascii);
  if(note) {
    note->menuBar->viewasciiAct->setChecked(Ascii);
    if(note2)
      note2->menuBar->viewasciiAct->setChecked(Ascii);
  }
  QString s="0 0$boxdraw_j_ ";
  s+=Ascii?"1":"0";
  jcon->cmd(s);
}

// ---------------------------------------------------------------------
QStringList state_about()
{
  QStringList r;
  QString engine,msg,ver,year;
  engine=dors("9!:14''");
  ver=qstaketo(engine,"/").toUpper();
  year=QString::number(QDate::currentDate().year());
  msg=dors("JVERSION");
  msg=msg + "\n\nCopyright 1994-"+year+" Jsoftware\n";
  msg=msg + "www.jsoftware.com";
  r.append("About " + ver);
  r.append(msg);
  return r;
}

// ---------------------------------------------------------------------
void state_appname()
{
  config.AppName="jqt";
}

// ---------------------------------------------------------------------
int state_fini(QApplication *app)
{
  Q_UNUSED(app);
  return jcon->exec();
}

// ---------------------------------------------------------------------
bool state_init(int argc, char *argv[])
{
  svr_init(argc,argv);
  config.init(cfpath(argv[0]));
  dlog_init();
  recent.init();
  project.init();
  return true;
}

// ---------------------------------------------------------------------
void state_init_resource()
{
  Q_INIT_RESOURCE(lib);
}

// ---------------------------------------------------------------------
void state_quit()
{
  jcon->quit();
}

// ---------------------------------------------------------------------
void state_reinit() {}

// ---------------------------------------------------------------------
int state_run(int argc, char *argv[],QApplication *app,QString lib)
{
  LibName=lib;
  state_init_resource();
  setlocale(1,"C");
  state_appname();
  term = new Term;
  if (!state_init(argc,argv)) return 1;
  term->fini();
  return state_fini(app);
}

// ---------------------------------------------------------------------
void var_cmd(QString s)
{
  jcon->cmd(s);
}

// ---------------------------------------------------------------------
QString var_load(QString s, bool d)
{
  QString r = d ? "loadd" : "load";
  return r + " '" + s + "'";
}

// ---------------------------------------------------------------------
void var_run(QString s)
{
  jcon->immex(s);
}

// ---------------------------------------------------------------------
void var_runs(QString s)
{
  sets("inputx_jrx_",s);
  jcon->immex("0!:101 inputx_jrx_");
}
