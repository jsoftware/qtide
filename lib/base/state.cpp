/* J state */

#include <QApplication>
#include <QDate>
#include <QFont>
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#else
#include <QPrinter>
#endif
#endif
#include <QSettings>
#include <QSyntaxHighlighter>
#include <QTemporaryFile>

#include <locale.h>

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
#include "../high/high.h"
#include "../wd/form.h"

#ifdef QT_OS_ANDROID
#include <jni.h>
extern "C" void javaOnLoad(JavaVM * vm, JNIEnv * env);
QString AndroidPackage;
extern "C" void android_getdisplaymetrics(double * dmetrics);
int androidVfuncPos=2;
#endif
#if !(defined(QT_NO_QUICKVIEW1)&&defined(QT_NO_QUICKVIEW2)&&defined(QT_NO_QUICKWIDGET))
#include "qmlje.h"
QmlJE qmlje;
#endif
#ifndef QT_NO_QUICKVIEW2
extern void regQmlJE();
#endif

extern bool FHS;

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

  k=s2q(dors("'',}.;',',&.>{.\"1 UserFolders_j_")).split(",");
  v=s2q(dors("'',}.;',',&.>{:\"1 UserFolders_j_")).split(",");

  UserFolderKeys=k;
  UserFolderValues=v;

  k+=s2q(dors("'',}.;',',&.>{.\"1 SystemFolders_j_")).split(",");
  v+=s2q(dors("'',}.;',',&.>{:\"1 SystemFolders_j_")).split(",");

  AllFolderKeys=k;
  AllFolderValues=v;

  BinPath.setPath(cpath("~bin"));
  ConfigPath.setPath(cpath("~config"));
  SnapPath.setPath(cpath("~snap"));
  SystemPath.setPath(cpath("~install"));
  TempPath.setPath(cpath("~temp"));
  UserPath.setPath(cpath("~user"));

// ??? should them be created for root user
  /*
    if (!NoProfile) {
      createdir(SnapPath);
      createdir(TempPath);
      createdir(UserPath);
    }
  */
}

// ---------------------------------------------------------------------
void Config::init()
{
  Q_ASSERT(jcon);

  DefCmt="NB.";
  DefCCmt="NB.";
  DefExt=".ijs";
  DefIndent="   ";
  DefTypes << "Scripts ijs"
           << "Text txt"
           << "SourceText ijs,ijt,jproj,txt"
           << "All *";

  FilePatterns="Scripts (*.ijs);;All files (*.*)";
  ifGit = gitavailable();
  ProjExt = ".jproj";
  ProjInit = false;

#ifdef _WIN32
  ProFont.setFamily("\"MS Sans Serif\" 10");
#else
  ProFont.setFamily("SansSerif 10");
#endif

#ifndef QT_NO_PRINTER
  Printer=new QPrinter(QPrinter::HighResolution);
#endif

  Lang = "J";
  LineNos = false;
  LineWrap = false;
  ScriptFilter="*.ijs";

  Rxnna = "\\b";
  Rxnnz = "\\b";

  NoProfile="1"!=dors("\":'/profile.ijs'-:(}.~i:&'/')jpathsep>{.4!:3''");
  if (NoProfile) {
    noprofile();
    return;
  }

  folder_init();
  config_init();
  launch_init();
  winpos_init();

  if ("0"==dors("\":4!:0 <'XDiff_j_'"))
    XDiff=s2q(dors("XDiff_j_"));

  if ("0"==dors("\":4!:0 <'DirTreeX_j_'"))
    DirTreeX=s2q(dors("DirTreeX_j_")).split(" ",QString::SkipEmptyParts);

  initide();
  initstyle();

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

  QString font="Monospace";
  int fontsize=10;
  QFont::Weight fontweight=QFont::Normal;

  QString terminal="gnome-terminal";

#ifdef QT_OS_ANDROID
  font="Droid Sans Mono";
  fontsize=16;
  terminal="";
#endif
#ifdef __MACH__
  font="Menlo";
  fontsize=14;
  terminal="/Applications/Utilities/Terminal.app/Contents/MacOS/Terminal";
#endif
#ifdef _WIN32
  font="Lucida Console";
  terminal="cmd.exe";
#endif

#ifdef QT_OS_ANDROID
  BackButtonClose = s->value("Session/BackButtonClose",false).toBool();
  ScrollBarSize = s->value("Session/ScrollBarSize",25).toInt();
#endif
  Completion = s->value("Session/Completion",false).toBool();
  CompletionFile = s->value("Session/CompletionFile","stdlib.txt").toString();
  BoxForm = s->value("Session/BoxForm",0).toInt();
  ConfirmClose = s->value("Session/ConfirmClose",false).toBool();
  ConfirmSave = s->value("Session/ConfirmSave",false).toBool();
  EscClose = s->value("Session/EscClose",false).toBool();
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
  TermSyntaxHighlight = s->value("Session/TermSyntaxHighlight",false).toBool();
  TrimTrailingWS = s->value("Session/TrimTrailingWS",false).toBool();

  QStringList fx;
  fx = s->value("FindinFiles/Extensions","").toStringList();
  fx=qsltrimeach(fx);
  fx.removeAll("");
  if (fx.isEmpty())
    fx << "ijs ijt" << "c cfg cpp h ijs ijt jproj js sh txt" << "htm html" << "*";
  FifExt=fx;

  Terminal = s->value("Run/Terminal",terminal).toString();

  t = s->value("Position/Edit","600 100 750 750").toString();
  q2p(t,EditPos);
  t = s->value("Position/Term","0 0 500 600").toString();
  q2p(t,TermPos);

  if (s->allKeys().contains("Session/TrimTrailingWS")) return;

  delete s;
  w=(fontweight==QFont::Normal) ? "normal" : "bold";
  QTemporaryFile temp;
  temp.open();
  s=new QSettings(temp.fileName(),QSettings::IniFormat);
#ifdef QT_OS_ANDROID
  s->setValue("Session/BackButtonClose",BackButtonClose);
  s->setValue("Session/ScrollBarSize",ScrollBarSize);
#endif
  s->setValue("Session/Completion",Completion);
  s->setValue("Session/CompletionFile",CompletionFile);
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
  s->setValue("Session/TermSyntaxHighlight",TermSyntaxHighlight);
  s->setValue("Session/TrimTrailingWS",TrimTrailingWS);
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
#ifdef QT_OS_ANDROID
    "# BackButtonClose=false        if Back Button will close the Term (Android only)\n"
    "# ScrollBarSize=25             width or height of scrollbar (Android only)\n"
#endif
    "# BoxForm=0                    0=linedraw 1=ascii (overrides base cfg)\n"
    "# Completion=false             if enable tab completion\n"
    "# CompletionFile=stdlib.txt    tab completion word list\n"
    "# ConfirmClose=false           confirm session close\n"
    "# ConfirmSave=false            confirm script save\n"
    "# Edit=600 100 750 750         initial edit position\n"
    "# EscClose=false               if Esc will close a window\n"
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
    "# Terminal=gnome-terminal      show in terminal command\n"
    "# TermSyntaxHighlight=false    if term has syntax highlighting\n"
    "# TrimTrailingWS=false         if remove trailing whitespace on save\n"
    ;
  cfwrite(f,h + "\n" + t);
}

// ---------------------------------------------------------------------
void Config::noprofile()
{
#ifdef QT_OS_ANDROID
  BackButtonClose = false;
  ScrollBarSize = 25;
#endif
  Completion = false;
  CompletionFile = "stdlib.txt";
  ConfirmClose = false;
  ConfirmSave = false;
  Font.setStyleHint(QFont::TypeWriter);
#ifdef QT_OS_ANDROID
  Font.setFamily("Droid Sans Mono");
  Font.setPointSize(16);
#else
  Font.setFamily("Monospace");
  Font.setPointSize(12);
#endif
  TermBack.color = QColor("mistyrose");
  TermFore.color = QColor("black");
  TermHigh.color = QColor("gainsboro");
  EditHigh.color = QColor("gainsboro");
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
QSyntaxHighlighter *highlight(QTextDocument *d)
{
  return new Highj(d);
}

// ---------------------------------------------------------------------
QStringList state_about()
{
  QStringList r;
  QString msg,ver,year;
//  QString engine;
//  engine=s2q(dors("9!:14''"));
//  ver=qstaketo(engine,"/").toUpper();
  ver= "J" JDLLVER;
  year=QString::number(QDate::currentDate().year());
  msg=s2q(dors("JVERSION"));
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
int state_fini()
{
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
  return true;
}

// ---------------------------------------------------------------------
void state_init_args(int *argc, char *argv[])
{
  config.SingleWin=false;
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
int state_run(int argc, char *argv[],QString lib,bool fhs)
{
  FHS=fhs;
  LibName=lib;
#ifdef QT_OS_ANDROID
// assume LibName is in the following formats
//      /data/data/com.jsoftware.android.qtide/lib/libjqt.so
//  4.3 /data/app-lib/com.jsoftware.android.qtide-1/libjqt.so
//      /mnt/asec/com.jsoftware.android.qtide-2/lib/libjq.so
  qDebug() << "LibName" << LibName;
  QStringList p=LibName.split("/");
  qDebug()<<p;
  AndroidPackage = p.at(3);
  QChar p1=AndroidPackage.at(AndroidPackage.size()-2);
  if (p1=='-') AndroidPackage = AndroidPackage.left(AndroidPackage.size()-2);
  qDebug() << "AndroidPackage" << AndroidPackage;
  LibName="/data/data/" + AndroidPackage + "/lib/libjqt.so";
  android_getdisplaymetrics(0);
#endif
#ifdef QTWEBSOCKET
#ifdef QT48
  qsrand(QDateTime::currentMSecsSinceEpoch());
#else
  qsrand(QTime::currentTime().msec());
#endif
#endif
  state_init_resource();
  setlocale(LC_NUMERIC,"C");
  state_appname();
  term = new Term;
  bool rc = state_init(argc,argv);
  if (!rc) return 1;
#if !(defined(QT_NO_QUICKVIEW2)&&defined(QT_NO_QUICKWIDGET))
#ifdef QT50
  regQmlJE();
#endif
#endif
#ifdef QT_OS_ANDROID
  if (AndroidPackage=="com.jsoftware.android.qtide")
    showide(true);
  else
    showide(false);
#else
  if ((!ShowIde) && Forms.isEmpty()) return 0;
#endif
  term->fini();
  return state_fini();
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
void var_runs(QString s, bool show)
{
  QString f=show ? "0!:101" : "0!:100";
  sets("inputx_jrx_",q2s(s));
  jcon->immex(f + " inputx_jrx_");
}

// ---------------------------------------------------------------------
void var_set(QString s, QString t)
{
  jcon->set(s,t);
}

// ---------------------------------------------------------------------
void immexj(const char *s)
{
  sets("inputx_jrx_",string(s));
  jcon->immex("0!:100 inputx_jrx_");
}

