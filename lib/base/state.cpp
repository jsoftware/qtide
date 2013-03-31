/* J state */

#include <QApplication>
#include <QDate>
#ifndef ANDROID
#include <QPrinter>
#endif
#include <QSettings>
#include <QTemporaryFile>

#include "base.h"
#include "state.h"
#include "dialog.h"
#include "dlog.h"
#include "jsvr.h"
#include "menu.h"
#include "note.h"
#include "proj.h"
#include "recent.h"
#include "svr.h"
#include "term.h"
#include "../wd/form.h"

#ifdef ANDROID
#include <QAndroidStyle>
extern "C" void javaOnLoad(void *,void *,void *);
#endif

using namespace std;

Config config;
QString LibName;

// ---------------------------------------------------------------------
// copy over configs if necessary
void Config::config_init()
{
  QStringList c;
  QString s,t;
  c << "dirmatch.cfg" << "launch.cfg";
  s=cpath("~addons/ide/qt/config/");
  foreach (QString f,c)
  if ((!cfexist(ConfigPath.filePath(f)) && cfexist(s+f))) {
    t=cfread(s+f);
    cfwrite(ConfigPath.filePath(f),t);
  }

  c.empty();
  c << "base.cfg" << "folders.cfg";
  s=cpath("~system/config/");
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
void Config::init()
{
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

#ifndef ANDROID
  Printer=new QPrinter(QPrinter::HighResolution);
#endif

  Lang = "J";
  LineNos = false;
  LineWrap = false;
  ScriptFilter="*.ijs";

  Rxnna = "\\b";
  Rxnnz = "\\b";

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

  if ("0"==dors("\":4!:0 <'DirTreeX_j_'"))
    DirTreeX=dors("DirTreeX_j_").split(" ",QString::SkipEmptyParts);

  initide();

  Ascii="+"==dors("{.9!:6$0");
  if (Ascii!=(BoxForm==1)) {
    var_cmd("boxdraw_j_ "+QString::number(BoxForm));
    Ascii=!Ascii;
  }

}

// ---------------------------------------------------------------------
// reads and updates qtide.cfg file
void Config::initide()
{
  QString f=ConfigPath.filePath("qtide.cfg");
  QSettings *s=new QSettings(f,QSettings::IniFormat);
  QString h,t,w;

  QString font="monospaced";
  int fontsize=10;
  QFont::Weight fontweight=QFont::Normal;

#ifdef __MACH__
  font="Menlo";
  fontsize=14;
#endif
#ifdef _WIN32
  font="Lucida Console";
#endif

  BoxForm = s->value("Session/BoxForm",0).toInt();
  ConfirmClose = s->value("Session/ConfirmClose",false).toBool();
  ConfirmSave = s->value("Session/ConfirmSave",false).toBool();
  EscClose = s->value("Session/EscClose",true).toBool();
  Font.setFamily(s->value("Session/FontFamily",font).toString());
  Font.setPointSize(s->value("Session/FontSize",fontsize).toInt());

  w=s->value("Session/FontWeight","normal").toString().toLower();
  if (w=="bold") fontweight=QFont::Bold;
  Font.setWeight(fontweight);

  KeepInputLog = s->value("Session/KeepInputLog",true).toBool();
  MaxRecent = s->value("Session/MaxRecent",15).toInt();
  OpenTabAt=s->value("Session/OpenTabAt",0).toInt();
  Snapshots = s->value("Session/Snapshots",true).toInt();
  Snapshotx = s->value("Session/Snapshotx","").toString();

  FifExt = s->value("FindinFiles/Extensions","").toStringList();
  Terminal = s->value("Run/Terminal","").toString();

  t = s->value("Position/Edit","600 100 750 750").toString();
  q2p(t,EditPos);
  t = s->value("Position/Term","0 0 500 600").toString();
  q2p(t,TermPos);

  if (s->allKeys().contains("Session/BoxForm")) return;

  delete s;
  w=(fontweight==QFont::Normal) ? "normal" : "bold";
  QTemporaryFile temp;
  temp.open();
  s=new QSettings(temp.fileName(),QSettings::IniFormat);
  s->setValue("Session/BoxForm",BoxForm);
  s->setValue("Session/ConfirmClose",ConfirmClose);
  s->setValue("Session/ConfirmSave",ConfirmSave);
  s->setValue("Session/EscClose",EscClose);
  s->setValue("Session/FontFamily",Font.family());
  s->setValue("Session/FontSize",Font.pointSize());
  s->setValue("Session/FontWeight",w);
  s->setValue("Session/KeepInputLog",KeepInputLog);
  s->setValue("Session/MaxRecent",MaxRecent);
  s->setValue("Session/OpenTabAt",OpenTabAt);
  s->setValue("Session/Snapshots",Snapshots);
  s->setValue("Session/Snapshotx",Snapshotx);
  s->setValue("FindinFiles/Extensions",FifExt);
  s->setValue("Position/Edit",p2q(EditPos));
  s->setValue("Position/Term",p2q(TermPos));
  s->setValue("Run/Terminal",Terminal);
  s->sync();
  t=cfread(temp.fileName());
  h="# Qt IDE config\n"
    "# This file is read and written by the Qt IDE.\n"
    "# Make changes in the same format as the original.\n"
    "# \n"
    "# BoxForm                      0=linedraw 1=ascii (overrides base cfg)\n"
    "# ConfirmClose=false           confirm session close\n"
    "# ConfirmSave=false            confirm script save\n"
    "# Edit=600 100 750 750         initial edit position\n"
    "# EscClose=true                if Esc will close a window\n"
    "# Extensions=ijs, c cfg...     FIF file extension lists\n"
    "# FontFamily=Menlo             term/edit font family\n"
    "# FontSize=10                  font size\n"
    "# FontWeight=normal            font weight: normal or bold\n"
    "# KeepInputLog=true            if inputlog is preserved\n"
    "# MaxRecent=15                 max number in recent files\n"
    "# OpenTabAt=0                  open tab 0=left,1=insert,2=right\n"
    "# Snapshots=5                  number of project snapshots kept\n"
    "# Snapshotx=                   snapshots exclusion list\n"
    "# Term=0 0 500 600             initial term position\n"
    "# Terminal=mate-terminal       show in terminal command\n"
    ;
  cfwrite(f,h + "\n" + t);
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
  state_init_args(&argc,argv);
  svr_init(argc,argv);
  config.init();
  dlog_init();
  recent.init();
  project.init();
  return true;
}

// ---------------------------------------------------------------------
void state_init_args(int *argc, char *argv[])
{
#ifdef ANDROID
//  config.SingleWin=true;
  config.SingleWin=false;
#else
  config.SingleWin=false;
#endif
  int n=*argc;
  for(int i=1; i<n; i++)
    if (!strcmp(argv[i],"-singlewin")) {
      config.SingleWin=true;
      *argc=n-1;
      for(int j=i+1; j<n; j++)
        argv[j-1]=argv[j];
      return;
    }
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
#ifdef ANDROID
int state_run(int argc, char *argv[],QApplication *app,QString lib,void *vm,void *qtapp,void *qtact)
#else
int state_run(int argc, char *argv[],QApplication *app,QString lib)
#endif
{
#ifdef ANDROID
  javaOnLoad(vm,qtapp,qtact);
  QAndroidStyle *androidStyle = new QAndroidStyle();
  QApplication::setPalette(androidStyle->standardPalette());
  QApplication::setStyle(androidStyle);
#endif
  LibName=lib;
  state_init_resource();
  setlocale(1,"C");
  state_appname();
  term = new Term;
  if (!state_init(argc,argv)) return 1;
  if ((!ShowIde) && Forms.isEmpty()) return 0;
  term->fini();
  return state_fini(app);
}

// ---------------------------------------------------------------------
void var_cmd(QString s)
{
  jcon->cmd(s);
}

// ---------------------------------------------------------------------
QString var_cmdr(QString s)
{
  return jcon->cmdr(s);
}

// ---------------------------------------------------------------------
void var_cmddo(QString s)
{
  jcon->cmddo(s);
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

// ---------------------------------------------------------------------
void var_set(QString s, QString t)
{
  jcon->set(s,t);
}

// ---------------------------------------------------------------------
void immexj(char *s)
{
  sets("inputx_jrx_",QString::fromUtf8(s));
  jcon->immex("0!:100 inputx_jrx_");
}

