/* J state */
#define NOMINMAX
#include <QApplication>
#include <QDate>
#if defined(QT57)
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
#include <QFont>
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#else
#include <QPrinter>
#endif
#endif
#ifdef QT50
#include <QLoggingCategory>
#endif
#include <QSettings>
#include <QSyntaxHighlighter>
#include <QTemporaryFile>
#include <QPoint>
#include <QSize>
#include <QScreen>
#include <QRect>

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
#include "tedit.h"
#include "term.h"
#include "../high/high.h"
#include "../wd/form.h"
#include "../wd/drawobj.h"
#include <algorithm>

Drawobj *drawobj;
#ifndef QT_NO_PRINTER
#include "../wd/prtobj.h"
Prtobj *prtobj;
#endif

#if !(defined(QT_NO_QUICKVIEW1)&&defined(QT_NO_QUICKVIEW2)&&defined(QT_NO_QUICKWIDGET))
#include "qmlje.h"
QmlJE qmlje;
#endif
#ifndef QT_NO_QUICKVIEW2
extern void regQmlJE();
#endif

extern bool FHS;
extern void wdreset();
extern Tedit *tedit;

// using namespace std;

QList<QWidget*> ActiveWindows;
Config config;
QString LibName;
QApplication *app=0;

const char *jqtver=JQTVERSION;
QEventLoop *evloop;
QEventLoop *jevloop;

int readTabWidth(QString);
QString writeTabWidth(int);

// ---------------------------------------------------------------------
void Config::case_init()
{
#ifdef _WIN32
  CasePaths=s2q(dors("'',}.;',',&.>CasePaths_j_")).split(",");
#else
  CasePaths=QStringList();
#endif
}

// ---------------------------------------------------------------------
// copy over configs if necessary
void Config::config_init()
{
  QStringList c;
  QString s,t;
  c << "dirmatch.cfg" << "launch.cfg" << "userkeys.cfg";
#ifdef TABCOMPLETION
  c << "stdlib.txt";
#endif
  s=cpath("~addons/ide/qt/config/");
  foreach (QString f,c)
    if ((!cfexist(ConfigPath.filePath(f)) && cfexist(s+f))) {
      t=cfread(s+f);
      cfwrite(ConfigPath.filePath(f),t);
    }

  c.empty();
  c << "base.cfg" << "case.cfg" << "folders.cfg";
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

  AddonsPath.setPath(cpath("~addons"));
  BinPath.setPath(cpath("~bin"));
  ConfigPath.setPath(cpath("~config"));
  HomePath.setPath(cpath("~home"));
  SnapPath.setPath(cpath("~snap"));
  SystemPath.setPath(cpath("~install"));
  TempPath.setPath(cpath("~temp"));
  UserPath.setPath(cpath("~user"));
}

// ---------------------------------------------------------------------
// run before svr init
void Config::ini0()
{
#if defined(QT57)
  QRect screenGeometry = app->primaryScreen()->geometry();
#else
  QDesktopWidget* dw=app->desktop();
  QRect screenGeometry = dw->screenGeometry(-1);
#endif
  ScreenWidth=screenGeometry.width();
  ScreenHeight=screenGeometry.height();
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

  drawobj=new Drawobj();

#ifndef QT_NO_PRINTER
  Printer=new QPrinter(QPrinter::HighResolution);
  prtobj=new Prtobj();
  Printer->setColorMode(QPrinter::Color);
  Printer->setPageMargins(QMarginsF(10.0, 10.0, 10.0, 10.0), QPageLayout::Millimeter);
#endif

  Lang = "J";
  LineWrap = false;
  ScriptFilter="*.ijs";

  Rxnna = "\\b";
  Rxnnz = "\\b";

  NoProfile="1"!=dors("\":(<'/profile.ijs')e.((}.~i:&'/')@('/'&(('\\' I.@:= ])})))&.>4!:3''");
  if (NoProfile) {
    noprofile();
    return;
  }

  case_init();
  folder_init();
  config_init();
  launch_init();
  userkeys_init();
  winpos_init();

  if ("0"==dors("\":4!:0 <'XDiff_j_'"))
    XDiff=s2q(dors("XDiff_j_"));

  if ("0"==dors("\":4!:0 <'DirTreeX_j_'"))
    DirTreeX=s2q(dors("DirTreeX_j_")).split(" ",_SkipEmptyParts);

  initide();
  initstyle();

  DefFont=Font;
  DefProFont=app->font();

  Ascii="+"==dors("{.9!:6$0");
  if (Ascii!=(BoxForm==1)) {
    jcon->cmd("0 0$boxdraw_j_ "+q2s(QString::number(BoxForm)));
    Ascii=!Ascii;
  }

  term->cleantemp();

}

// ---------------------------------------------------------------------
// add comments and updates qtide.cfg file
void initidec(QString f, QString t)
{
  QString h;

  h = "# Qt IDE config\n"
      "# This file is read and written by the Qt IDE.\n"
      "# Make changes in the same format as the original.\n"
      "# \n"
      "# BoxForm=0                    0=linedraw 1=ascii (overrides base cfg)\n"
#ifdef TABCOMPLETION
      "# Completion=false             if enable tab completion\n"
      "# CompletionFile=stdlib.txt    tab completion word list\n"
#endif
      "# ConfirmClose=false           confirm session close\n"
      "# ConfirmSave=false            confirm script save\n"
      "# CursorWidth=1                cursor width in session windows\n"
      "# DebugDissect=true            if Dissect included in Debug\n"
      "# FileManager=explorer         show in file manager\n"
      "# EscClose=false               if Esc will close a window\n"
      "# Extensions=ijs, c cfg...     FIF file extension lists\n"
      "# FontFamily=Menlo             term/edit font family\n"
      "# FontSize=10                  font size\n"
      "# FontWeight=normal            font weight: normal or bold\n"
      "# KeepCursorPosOnRecall=false  if keep cursor position on line recall\n"
      "# LineNumbers=false            if show line numbers\n"
      "# MaxInputLog=100              max number in input log, 0 for none\n"
      "# MaxRecent=15                 max number in recent files\n"
      "# MaxVisibleItems=10           max visible items in combobox\n"
      "# OpenTabAt=0                  open tab 0=left,1=insert,2=right\n"
      "# Snapshots=5                  number of project snapshots kept\n"
      "# Snapshotx=                   snapshots exclusion list\n"
      "# TabWidth=                    tab width (Qt default if empty)\n"
      "# Terminal=cmd                 show in terminal command\n"
      "# TermSyntaxHighlight=false    if term has syntax highlighting\n"
      "# TrimTrailingWS=false         if remove trailing whitespace on save\n"
      "#\n"
      "# Initial xywh positions, with negative xy from opposite edge:\n"
      "# Debug=-590 50 540 500        debug position\n"
      "# Edit=600 100 750 750         edit position\n"
      "# Term=0 0 500 600             term position\n"
      ;
  cfwrite(f, h + "\n" + t);

}

// ---------------------------------------------------------------------
// reads and updates qtide.cfg file
void Config::initide()
{
  QString f=ConfigPath.filePath("qtide.cfg");
  QSettings *s=new QSettings(f,QSettings::IniFormat);
  QString t,w;

  QString font="Monospace";
  int fontsize=10;
  QFont::Weight fontweight=QFont::Normal;

  QString filemanager="nautilus";
  QString terminal="gnome-terminal";

#ifdef __MACH__
  filemanager="open";
  font="Menlo";
  fontsize=14;
  terminal="/Applications/Utilities/Terminal.app/Contents/MacOS/Terminal";
#endif
#ifdef _WIN32
  filemanager="explorer";
  font="Lucida Console";
  terminal="cmd";
#endif

#ifdef TABCOMPLETION
  Completion = s->value("Session/Completion",false).toBool();
  CompletionFile = s->value("Session/CompletionFile","stdlib.txt").toString();
#endif
  BoxForm = s->value("Session/BoxForm",0).toInt();
  ConfirmClose = s->value("Session/ConfirmClose",false).toBool();
  ConfirmSave = s->value("Session/ConfirmSave",false).toBool();
  CursorWidth = s->value("Session/CursorWidth",1).toInt();
  if (tedit) tedit->setCursorWidth(CursorWidth);
  DebugDissect = s->value("Session/DebugDissect",true).toBool();
  EscClose = s->value("Session/EscClose",false).toBool();
  Font.setFamily(s->value("Session/FontFamily",font).toString());
  Font.setPointSize(s->value("Session/FontSize",fontsize).toInt());

  w=s->value("Session/FontWeight","normal").toString().toLower();
  if (w=="bold") fontweight=QFont::Bold;
  Font.setWeight(fontweight);

  KeepCursorPosOnRecall = s->value("Session/KeepCursorPosOnRecall",false).toBool();
  LineNos = s->value("Session/LineNumbers",false).toBool();

  int len=s->value("Session/MaxInputLog",-1).toInt();
  if (len<0)
    len=(s->value("Session/KeepInputLog",true).toBool()) ? 100 : 0;
  MaxInputLog=len;

  MaxVisibleItems = s->value("Session/MaxVisibleItems",10).toInt();
  MaxRecent = s->value("Session/MaxRecent",15).toInt();
  OpenTabAt=s->value("Session/OpenTabAt",0).toInt();
  Snapshots = s->value("Session/Snapshots",true).toInt();
  Snapshotx = s->value("Session/Snapshotx","").toString();
  TabWidth = readTabWidth(s->value("Session/TabWidth","").toString());
  TermSyntaxHighlight = s->value("Session/TermSyntaxHighlight",false).toBool();
  TrimTrailingWS = s->value("Session/TrimTrailingWS",false).toBool();

  QStringList fx;
  fx = s->value("FindinFiles/Extensions","").toStringList();
  fx=qsltrimeach(fx);
  fx.removeAll("");
  if (fx.isEmpty())
    fx << "ijs ijt" << "c cfg cpp h ijs ijt jproj js sh txt" << "htm html" << "*";
  FifExt=fx;

  FileManager = s->value("Run/FileManager",filemanager).toString();
  Terminal = s->value("Run/Terminal",terminal).toString();

  t = s->value("Position/Debug","-590 50 540 500").toString();
  QString dp("-590 50 540 500");
  DebugPos=q2p(t);
  DebugPosX=initposX(DebugPos, q2p(dp));

  t = s->value("Position/Edit","600 100 750 750").toString();
  QString ep("600 100 750 750");
  EditPos=q2p(t);
  EditPosX=initposX(EditPos, q2p(ep));

#ifdef __linux__
  t = s->value("Position/Term","0 20 500 600").toString();
  QString tp("0 20 500 600");
#else
  t = s->value("Position/Term","0 0 500 600").toString();
  QString tp("0 0 500 600");
#endif
  TermPos=q2p(t);
  TermPosX=initposX(TermPos, q2p(tp));

  if (s->allKeys().contains("Session/MaxVisibleItems")) return;

  delete s;
  w=(fontweight==QFont::Normal) ? "normal" : "bold";
#ifdef _WIN32
  QFile temp(ConfigPath.filePath("qtide.cfg.0"));
#else
  QTemporaryFile temp;
  temp.open();
  temp.close();
#endif
  s=new QSettings(temp.fileName(),QSettings::IniFormat);
#ifdef TABCOMPLETION
  s->setValue("Session/Completion",Completion);
  s->setValue("Session/CompletionFile",CompletionFile);
#endif
  s->setValue("Session/BoxForm",BoxForm);
  s->setValue("Session/ConfirmClose",ConfirmClose);
  s->setValue("Session/ConfirmSave",ConfirmSave);
  s->setValue("Session/CursorWidth",CursorWidth);
  s->setValue("Session/DebugDissect",DebugDissect);
  s->setValue("Session/EscClose",EscClose);
  s->setValue("Session/FontFamily",Font.family());
  s->setValue("Session/FontSize",Font.pointSize());
  s->setValue("Session/FontWeight",w);
  s->setValue("Session/KeepCursorPosOnRecall",KeepCursorPosOnRecall);
  s->setValue("Session/LineNumbers",LineNos);
  s->setValue("Session/MaxInputLog",MaxInputLog);
  s->setValue("Session/MaxRecent",MaxRecent);
  s->setValue("Session/MaxVisibleItems",MaxVisibleItems);
  s->setValue("Session/OpenTabAt",OpenTabAt);
  s->setValue("Session/Snapshots",Snapshots);
  s->setValue("Session/Snapshotx",Snapshotx);
  s->setValue("Session/TabWidth",writeTabWidth(TabWidth));
  s->setValue("Session/TermSyntaxHighlight",TermSyntaxHighlight);
  s->setValue("Session/TrimTrailingWS",TrimTrailingWS);
  s->setValue("FindinFiles/Extensions",FifExt);
  s->setValue("Position/Debug",p2q(DebugPos));
  s->setValue("Position/Edit",p2q(EditPos));
  s->setValue("Position/Term",p2q(TermPos));
  s->setValue("Run/FileManager",FileManager);
  s->setValue("Run/Terminal",Terminal);
  s->sync();
#ifdef _WIN32
  t=cfread(ConfigPath.filePath("qtide.cfg.0"));
#else
  t=cfread(temp.fileName());
#endif
  initidec(f, t);
}

// ---------------------------------------------------------------------
// get Term or Edit position and size
QString getxywh(QPoint p, QSize s)
{
  QString r;

  r = QString::number(p.x()) + " " + QString::number(p.y()) + " " + QString::number(s.width()) + " " + QString::number(s.height());
  return r;
}

// ---------------------------------------------------------------------
// save current positions in qtide.cfg file
void curposide()
{
  QString f = config.ConfigPath.filePath("qtide.cfg");
  QSettings *s = new QSettings(f, QSettings::IniFormat);
  QString t, w;

  QString font = "Monospace";
  QFont::Weight fontweight = QFont::Normal;

  QString filemanager = "nautilus";
  QString terminal = "gnome-terminal";

  QString nt("");
  if (note) nt = getxywh(note->pos(), note->size());

  QString tt("");
  tt = getxywh(term->pos(), term->size());

  t = s->value("Position/Debug", "-590 50 540 500").toString();
  QString dp("-590 50 540 500");
  config.DebugPos = q2p(t);
  config.DebugPosX = config.initposX(config.DebugPos, q2p(dp));

  t = s->value("Position/Edit", "600 100 750 750").toString();
  QString ep("600 100 750 750");
  config.EditPos = q2p(t);
  config.EditPosX = config.initposX(config.EditPos, q2p(ep));

#ifdef __linux__
  t = s->value("Position/Term", "0 20 500 600").toString();
  QString tp("0 20 500 600");
#else
  t = s->value("Position/Term", "0 0 500 600").toString();
  QString tp("0 0 500 600");
#endif
  config.TermPos = q2p(t);
  config.TermPosX = config.initposX(config.TermPos, q2p(tp));

  delete s;
  w = (fontweight == QFont::Normal) ? "normal" : "bold";
#ifdef _WIN32
  QFile temp(config.ConfigPath.filePath("qtide.cfg.0"));
#else
  QTemporaryFile temp;
  temp.open();
  temp.close();
#endif
  s = new QSettings(temp.fileName(), QSettings::IniFormat);
#ifdef TABCOMPLETION
  s->setValue("Session/Completion", Completion);
  s->setValue("Session/CompletionFile", CompletionFile);
#endif
  s->setValue("Session/BoxForm", config.BoxForm);
  s->setValue("Session/ConfirmClose", config.ConfirmClose);
  s->setValue("Session/ConfirmSave", config.ConfirmSave);
  s->setValue("Session/CursorWidth", config.CursorWidth);
  s->setValue("Session/DebugDissect", config.DebugDissect);
  s->setValue("Session/EscClose", config.EscClose);
  s->setValue("Session/FontFamily", config.Font.family());
  s->setValue("Session/FontSize", config.Font.pointSize());
  s->setValue("Session/FontWeight", w);
  s->setValue("Session/KeepCursorPosOnRecall", config.KeepCursorPosOnRecall);
  s->setValue("Session/LineNumbers", config.LineNos);
  s->setValue("Session/MaxInputLog", config.MaxInputLog);
  s->setValue("Session/MaxRecent", config.MaxRecent);
  s->setValue("Session/MaxVisibleItems", config.MaxVisibleItems);
  s->setValue("Session/OpenTabAt", config.OpenTabAt);
  s->setValue("Session/Snapshots", config.Snapshots);
  s->setValue("Session/Snapshotx", config.Snapshotx);
  s->setValue("Session/TabWidth", writeTabWidth(config.TabWidth));
  s->setValue("Session/TermSyntaxHighlight", config.TermSyntaxHighlight);
  s->setValue("Session/TrimTrailingWS", config.TrimTrailingWS);
  s->setValue("FindinFiles/Extensions", config.FifExt);
  s->setValue("Position/Debug", p2q(config.DebugPos));
  if (note) {
    s->setValue("Position/Edit", nt);
  }
  s->setValue("Position/Term", tt);
  s->setValue("Run/FileManager", config.FileManager);
  s->setValue("Run/Terminal", config.Terminal);
  s->sync();
#ifdef _WIN32
  t = cfread(config.ConfigPath.filePath("qtide.cfg.0"));
#else
  t = cfread(temp.fileName());
#endif
  initidec(f, t);
}

// ---------------------------------------------------------------------
QList<int> Config::initposX(QList<int> p, QList<int> d)
{
  QList<int> r = p;
#ifndef QT50
  Q_UNUSED(d);
#else
  int x1 = 10000000;
  int x2 = -10000000;
  int y1 = 10000000;
  int y2 = -10000000;
  QList<QScreen*> screens = QGuiApplication::screens();
  int qsc = screens.count();
  QScreen *screen;
  QRect screenGeometry;
  for (int i = 0; i < qsc; i++) {
    screen = screens[i];
    screenGeometry = screen->geometry();
    x1 = std::min(x1, screenGeometry.left());
    x2 = std::max(x2, screenGeometry.right());
    y1 = std::min(y1, screenGeometry.top());
    y2 = std::max(y2, screenGeometry.bottom());
  }
  if ((r.at(0) < x1 || r.at(1) < y1) || (r.at(0) > x2 || r.at(1) > y2)) {
    r.replace(0, d.at(0));
    r.replace(1, d.at(1));
  }
#endif
  return r;
}

// ---------------------------------------------------------------------
void Config::noprofile()
{
#ifdef TABCOMPLETION
  Completion = false;
  CompletionFile = "stdlib.txt";
#endif
  ConfirmClose = false;
  ConfirmSave = false;
  DebugDissect = false;
  Font.setStyleHint(QFont::TypeWriter);
  Font.setFamily("Monospace");
  Font.setPointSize(12);
  TermBack.color = QColor("mistyrose");
  TermFore.color = QColor("black");
  TermHigh.color = QColor("gainsboro");
  TermSelb.color = QColor(0,162,232);
  TermSelt.color = QColor("white");
  EditHigh.color = QColor("gainsboro");
  TermPos=q2p("0 0 500 500");
  term->menuBar->hide();
}

// ---------------------------------------------------------------------
void Config::toggleascii()
{
  Ascii=!Ascii;
  term->menuBar->viewasciiAct->setChecked(Ascii);
  if (note) {
    note->menuBar->viewasciiAct->setChecked(Ascii);
    if (note2)
      note2->menuBar->viewasciiAct->setChecked(Ascii);
  }
  std::string s="0 0$boxdraw_j_ ";
  s+=Ascii?"1":"0";
  jcon->cmd(s);
}

// ---------------------------------------------------------------------
void delactivewindow(QWidget* w)
{
  ActiveWindows.removeOne(w);
}

// ---------------------------------------------------------------------
QWidget* getactivewindow()
{
  if (ActiveWindows.size())
    return ActiveWindows.first();
  else
    return term;
}

// ---------------------------------------------------------------------
void setactivewindow(QWidget* w)
{
  ActiveWindows.removeOne(w);
  ActiveWindows.prepend(w);
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
  ver=s2q(dors("9!:14''"));
  ver=ver.mid(0,ver.indexOf("/")).toUpper();
  year=QString::number(qMax(2019,QDate::currentDate().year()));
  msg=s2q(dors("JVERSION"));
  if (msg.contains("www.jsoftware.com"))
    msg+= "\n\nCopyright 1994-"+year+" Jsoftware\n";
  r.append("About " + ver);
  r.append(msg);
  return r;
}

// ---------------------------------------------------------------------
void state_appname()
{
  Q_UNUSED(jqtver);
  config.AppName="jqt";
}

// ---------------------------------------------------------------------
int state_fini()
{
  int rc=evloop->exec(QEventLoop::AllEvents|QEventLoop::WaitForMoreEvents);
#ifndef _WIN32
  jefree();
  app->quit();
#endif
  return rc;
}

// ---------------------------------------------------------------------
static bool state_init(int argc, char *argv[], uintptr_t stackinit)
{
  if (!jdllproc && (void*)-1==jdlljt) {
    state_init_args(&argc,argv);
    config.ini0();
    svr_init(argc,argv,stackinit);
    config.init();
    dlog_init();
    recent.init();
  } else {
    state_init_args(&argc,argv);
    if ((void*)-1!=jdlljt) svr_init(argc,argv,stackinit);
  }
  return true;
}

// ---------------------------------------------------------------------
void state_init_args(int *argc, char *argv[])
{
  config.SingleWin=false;
  int n=*argc;
  for (int i=1; i<n; i++)
    if (!strcmp(argv[i],"-singlewin")) {
      config.SingleWin=true;
      *argc=n-1;
      for (int j=i+1; j<n; j++)
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
  term->cleantemp();
}

// ---------------------------------------------------------------------
void state_reinit()
{
  config.NoProfile="1"!=dors("\":(<'/profile.ijs')e.((}.~i:&'/')@('/'&(('\\' I.@:= ])})))&.>4!:3''");
  if (!config.NoProfile) {
    term->menuBar->show();
    config.ini0();
    config.init();
    dlog_init();
    recent.init();
    term->fini();
  }
}

// ---------------------------------------------------------------------
int state_run(int argc, char *argv[], char *lib, bool fhs, int fshowide, void *jproc, void *jt0, void **jdll, void **jst, uintptr_t stackinit)
{
  if (-1==argc) {
    return state_fini();  // the 2nd time state_run is called
  } else if (-2==argc) {
    if (tedit) tedit->showcmd(QString::fromUtf8(lib));  // olecomh display Do cmd if Log 1
    return 0;
  } else if (-3==argc) {
    showide(!!fshowide);  // olecom Show
    return 0;
  } else if (-4==argc) {
    if (term) term->filequit(true);  // olecom Quit
    return 0;
  } else if (-100==argc) {
    state_reinit();
    return 0;
  } else if (0>argc) {
    return 0;
  }

  ShowIde=!!fshowide;
#ifdef QT50
  QLoggingCategory::setFilterRules("*.debug=true\nqt.*.debug=false");
#endif
#ifdef _WIN32
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
  app = new QApplication(argc, argv);
  evloop=new QEventLoop();
  jevloop=new QEventLoop();

  jdllproc=jproc;
  jdlljt=jt0;

  FHS=fhs;
  LibName=QString::fromUtf8(lib);
#if !defined(QT515)
#ifdef QTWEBSOCKET
  qsrand(QDateTime::currentMSecsSinceEpoch());
#endif
#endif
  state_init_resource();
  state_appname();
  term = new Term;
  bool rc = state_init(argc,argv,stackinit);
  if (!rc) return 1;
  if (jst) *jst=jt;
  if (jdll) *jdll=hjdll;
#if !(defined(QT_NO_QUICKVIEW2)&&defined(QT_NO_QUICKWIDGET))
#ifdef QT50
  regQmlJE();
#endif
#endif
  if (jdllproc || (!jdllproc && (void*)-1!=jdlljt)) showide(false);
  if ((!jdllproc) && (!ShowIde) && Forms.isEmpty()) return 0;
  term->fini();
  return 0;
}

// ---------------------------------------------------------------------
int readTabWidth(QString s)
{

  if (s.isEmpty()) return -1;
  return s.toInt();
}

// ---------------------------------------------------------------------
QString writeTabWidth(int n)
{
  if (n<0) return "";
  return QString::number(n);
}

// ---------------------------------------------------------------------
void immexj(const char *s)
{
  sets("inputx_jrx_",std::string(s));
  jcon->immex("0!:100 inputx_jrx_");
}

