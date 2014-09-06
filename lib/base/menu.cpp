#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QMenu>
#include <QUrl>

#include "base.h"
#include "menu.h"
#include "dialog.h"
#include "dirm.h"
#include "dlog.h"
#include "fif.h"
#include "fiw.h"
#include "note.h"
#include "nedit.h"
#include "nmain.h"
#include "note.h"
#include "nside.h"
#include "ntabs.h"
#include "pnew.h"
#include "proj.h"
#include "psel.h"
#include "recent.h"
#include "rsel.h"
#include "snap.h"
#include "spic.h"
#include "state.h"
#include "tedit.h"
#include "term.h"
#include "view.h"

#ifdef QT_OS_ANDROID
#include "../wd/form.h"
extern Fif *fif;
extern Fiw *fiw;
#endif

using namespace std;

// ---------------------------------------------------------------------
QAction *Menu::makeact(QString id, QString text, QString shortcut)
{
  QAction *r = new QAction(text,this);
  r->setObjectName(id);
  if (shortcut.size())
    r->setShortcut(shortcut);
  return r;
}

// ---------------------------------------------------------------------
QAction *Menu::makeuseract(QString text, QString shortcut)
{
  QAction *r = new QAction(text,this);
  r->setObjectName(shortcut);
  r->setShortcut(shortcut);
  connect(r, SIGNAL(triggered()), this, SLOT(on_userAct_triggered()));
  return r;
}

// ---------------------------------------------------------------------
void Menu::createActions()
{
  createhelpActions();
  cfgbaseAct = makeact("cfgbaseAct","Base","");
  cfgdirmAct = makeact("cfgdirmAct","Directory Match","");
  cfgfoldersAct = makeact("cfgfoldersAct","Folders","");
  cfglaunchpadAct = makeact("cfglaunchpadAct","&Launch Pad","");
  cfgopenallAct = makeact("cfgopenallAct","Open All","");
  cfgqtideAct = makeact("cfgqtideAct","Qt Ide","");
  cfgstartupAct = makeact("cfgstartupAct","Startup Script","");
  cfgstyleAct = makeact("cfgstyleAct","Styles","");
  cfguserkeysAct = makeact("cfguserkeysAct","User Keys","");
  cleartermAct = makeact("cleartermAct","&Clear Terminal","Ctrl+Shift+T");
  clipcopyAct = makeact("clipcopyAct","&Copy","Ctrl+C");
  clipcutAct = makeact("clipcutAct","Cu&t","Ctrl+X");
  clippasteAct = makeact("clippasteAct","&Paste","Ctrl+V");
  editfifAct = makeact("editfifAct","F&ind in Files","Ctrl+Shift+F");
  editfiwAct = makeact("editfiwAct","&Find","Ctrl+F");
  editfontAct = makeact("editfontAct","&Session Font","");
  editinputlogAct = makeact("editinputlogAct","Input &Log","Ctrl+D");
#ifdef QT_OS_ANDROID
  editwdformAct = makeact("editwdformAct","wd form","");
#endif
  editredoAct = makeact("editredoAct","&Redo","Ctrl+Y");
  editundoAct = makeact("editundoAct","&Undo","Ctrl+Z");
  filecloseAct = makeact("filecloseAct","&Close","Ctrl+W");
  filedeleteAct = makeact("filedeleteAct","&Delete","");
  filecloseallAct = makeact("filecloseallAct","C&lose All","");
  filecloseotherAct = makeact("filecloseotherAct","Close &Other","Ctrl+Shift+W");
  filesaveallAct = makeact("filesaveallAct","Save A&ll","Ctrl+Shift+S");
  filesaveasAct = makeact("filesaveasAct","Save &As","");
  filenewAct = makeact("filenewAct","&New","");
  filenewtempAct = makeact("filenewtempAct","&New temp","Ctrl+N");
  fileopenAct = makeact("fileopenAct","&Open","");
  fileopenaddonsAct = makeact("fileopenaddonsAct","Open &addons","");
  fileopenallAct = makeact("fileopenallAct","&Open All","");
  fileopentempAct = makeact("fileopentempAct","Open &temp","Ctrl+O");
  fileopenuserAct = makeact("fileopenuserAct","Open &user","");
  fileopensystemAct = makeact("fileopensystemAct","Open &system","");
#ifndef QT_NO_PRINTER
  fileprintAct = makeact("fileprintAct","&Print","");
  fileprintallAct = makeact("fileprintallAct","Print all","");
#endif
  filequitAct = makeact("filequitAct","&Quit","Ctrl+Q");
  filerecentAct = makeact("filerecentAct","&Recent","Ctrl+G");
  filereloadAct = makeact("filereloadAct","Reload q server","");
  filesaveAct = makeact("filesaveAct","&Save","Ctrl+S");

  projectbuildAct = makeact("projectbuildAct","&Build","Ctrl+F9");
  projectcloseAct = makeact("projectcloseAct","&Close","");
  projectgitguiAct = makeact("projectgitguiAct","Git Gui","");
  projectgitstatusAct = makeact("projectgitstatusAct","Git Status","");
  projectlastAct = makeact("projectlastAct","Open &Last","");
  projectnewAct = makeact("projectnewAct","&New","");
  projectopenAct = makeact("projectopenAct","&Open","");
  projectsnapAct = makeact("projectsnapAct","Project Snapshots","");
  projectsnapmakeAct = makeact("projectsnapmakeAct","Make Snapshot","");
  projectterminalAct = makeact("projectterminalAct","Open in Terminal","");

  runalllinesAct = makeact("runalllinesAct","All Lines","Ctrl+Shift+E");
  runalllines1Act = makeact("runalllines1Act","Run All Lines","F10");
  runalllines2Act = makeact("runalllines2Act","Clear Terminal, Run All Lines","Ctrl+Shift+F10");
  runclipAct = makeact("runclipAct","&Clipboard","F8");
  rundebugAct = makeact("rundebugAct","&Debug...","Ctrl+K");
  runlineAct = makeact("runlineAct","Line","Ctrl+R");
  runlineadvanceAct = makeact("runlineadvanceAct","Line and &Advance","Ctrl+Return");
  runlineadvanceshowAct = makeact("runlineadvanceshowAct","Line Advance &Show","Ctrl+Shift+Return");
  runlineshowAct = makeact("runlineshowAct","Line Show","Ctrl+Shift+R");
  runprojectAct = makeact("runprojectAct","Run &Project","F9");
  runscriptAct = makeact("runscriptAct","&Load Script","Ctrl+L");
  runselectAct = makeact("runselectAct","S&election","Ctrl+E");
  runtestAct = makeact("runtestAct","&Test","F5");

  scriptformatAct = makeact("scriptformatAct","&Format","Ctrl+Shift+P");
  scriptglobalsAct = makeact("scriptglobalsAct","Global Assignments","");
  scriptsnapAct = makeact("scriptsnapAct","&Script Snapshots","");
  scriptrestoreAct = makeact("scriptrestoreAct","R&estore","");

  toolsdirmAct = makeact("toolsdirmAct","&Directory Match","");
  toolspacmanAct = makeact("toolspacmanAct","&Package Manager","");
  toolsfkeysAct = makeact("toolsfkeysAct","Function Keys","");
  tosellowerAct = makeact("tosellowerAct","Lower Case","");
  toselminusAct = makeact("toselminusAct","Remove Comment","Ctrl+Shift+B");
  toselplusAct = makeact("toselplusAct","Add Comment","Ctrl+Shift+N");
  toselplusline1Act = makeact("toselplusline1Act","Add Comment ---","Ctrl+Shift+K");
  toselplusline2Act = makeact("toselplusline2Act","Add Comment ===","Ctrl+Shift+L");
  toselsortAct = makeact("toselsortAct","Sort","");
  toseltoggleAct = makeact("toseltoggleAct","&Toggle Case","");
  toselupperAct = makeact("toselupperAct","Upper Case","");
  toselviewlinewrapAct = makeact("toselviewlinewrapAct","Wrap","");

  viewasciiAct = makeact("viewasciiAct","&Toggle Ascii Box Drawing","");
  vieweditorAct = makeact("vieweditorAct","&Editor","Ctrl+M");
  viewfontminusAct = makeact("viewfontminusAct","Decrease Font","Ctrl+Shift+F12");
  viewfontplusAct = makeact("viewfontplusAct","Increase Font","Ctrl+F12");
  viewlinenosAct = makeact("viewlinenosAct","Toggle Line &Numbers","");
  viewlinewrapAct = makeact("viewlinewrapAct","Toggle Line &Wrapping","");
  viewsidebarAct = makeact("viewsidebarAct","&Sidebar","Ctrl+B");
  viewterminalAct = makeact("viewterminalAct","Terminal","Ctrl+T");

  viewasciiAct->setCheckable(true);
  viewlinenosAct->setCheckable(true);
  viewlinewrapAct->setCheckable(true);

  winfileclosexAct = makeact("winfileclosexAct","&Close non-Project Files","");
  winotherAct = makeact("winotherAct","&Other Window","Ctrl+Shift+M");
  winprojAct = makeact("winprojAct","Switch to &Project at Tab","");
  winscriptsAct = makeact("winscriptsAct","Open All Scri&pts","");
  winsourceAct = makeact("winsourceAct","Open All &Source","");
  wintextAct = makeact("wintextAct","Open All Te&xt","");
  winthrowAct = makeact("winthrowAct","&Throw Tab","");

  GitEnable << projectgitstatusAct << projectgitguiAct;

  ProjectEnable << projectbuildAct << projectcloseAct << projectsnapAct
                << projectsnapmakeAct << projectterminalAct
                << runprojectAct << winscriptsAct
                << winsourceAct << wintextAct << winfileclosexAct;

  ScriptEnable << clipcopyAct << clipcutAct << clippasteAct
               << editfiwAct << filecloseAct << filecloseallAct << filecloseotherAct
               << filedeleteAct
#ifndef QT_NO_PRINTER
               << fileprintAct << fileprintallAct
#endif
               << filesaveAct << filesaveallAct
               << filesaveasAct << runalllinesAct << runalllines1Act
               << runalllines2Act << runlineAct << runlineadvanceAct
               << runlineadvanceshowAct << runlineshowAct << runscriptAct
               << runselectAct << scriptformatAct << scriptglobalsAct
               << scriptsnapAct << scriptrestoreAct << winprojAct << winthrowAct;
}

// ---------------------------------------------------------------------
void Menu::createMenus(QString s)
{
  createfileMenu(s);
  createeditMenu(s);
  createviewMenu(s);
  createrunMenu(s);
  createtoolsMenu(s);
  if (s == "note")
    createscriptMenu();
  createprojectMenu(s);
  if (s == "note")
    createwindowMenu(s);
  createhelpMenu();
}

// ---------------------------------------------------------------------
void Menu::createMenus_fini(QString s)
{
  createlaunchMenu(s);
}

// ---------------------------------------------------------------------
void Menu::createcfgMenu()
{
  cfgMenu->addAction(cfgbaseAct);
  cfgMenu->addAction(cfgdirmAct);
  cfgMenu->addAction(cfgfoldersAct);
  cfgMenu->addAction(cfglaunchpadAct);
  if (config.AppName=="jqt")
    cfgMenu->addAction(cfgqtideAct);

  cfgMenu->addAction(cfgstyleAct);
  cfgMenu->addAction(cfguserkeysAct);
  if (config.AppName=="jqt") {
    cfgMenu->addSeparator();
    cfgMenu->addAction(cfgstartupAct);
  }
  cfgMenu->addSeparator();
  cfgMenu->addAction(cfgopenallAct);
}

// ---------------------------------------------------------------------
void Menu::createeditMenu(QString s)
{
  editMenu = addMenu("&Edit");
  if (s == "note") {
    editMenu->addAction(editundoAct);
    editMenu->addAction(editredoAct);
    editMenu->addSeparator();
  }
  editMenu->addAction(clipcutAct);
  editMenu->addAction(clipcopyAct);
  editMenu->addAction(clippasteAct);
  editMenu->addSeparator();
  editMenu->addAction(editfiwAct);
  editMenu->addAction(editfifAct);
  editMenu->addSeparator();
  if (s == "note") {
    selMenu = editMenu->addMenu("Selection");
    createselMenu();
  }
  editMenu->addSeparator();
  editMenu->addAction(editfontAct);
  cfgMenu = editMenu->addMenu("Configure");
  cfgMenu->menuAction()->setMenuRole(QAction::NoRole);
  createcfgMenu();
}

// ---------------------------------------------------------------------
void Menu::createfileMenu(QString s)
{
  fileMenu = addMenu("&File");
  if (s == "note") {
    fileMenu->addAction(filenewAct);
    fileMenu->addAction(fileopenAct);
    fileMenu->addSeparator();
    fileMenu->addAction(filenewtempAct);
    fileMenu->addAction(fileopentempAct);
    fileMenu->addSeparator();
    fileMenu->addAction(filerecentAct);
    fileMenu->addSeparator();
    fileMenu->addAction(filesaveAct);
    fileMenu->addAction(filesaveasAct);
    fileMenu->addAction(filesaveallAct);
    fileMenu->addSeparator();
#ifndef QT_NO_PRINTER
    fileMenu->addAction(fileprintAct);
    fileMenu->addAction(fileprintallAct);
#endif
    fileMenu->addSeparator();
    fileMenu->addAction(filecloseAct);
    fileMenu->addAction(filecloseotherAct);
    fileMenu->addAction(filecloseallAct);
    fileMenu->addSeparator();
    fileMenu->addAction(filedeleteAct);
  } else {
    fileMenu->addAction(filenewtempAct);
    fileMenu->addSeparator();
    fileMenu->addAction(fileopentempAct);
    fileMenu->addAction(fileopenuserAct);
    fileMenu->addAction(fileopenaddonsAct);
    fileMenu->addAction(fileopensystemAct);
    fileMenu->addSeparator();
    fileMenu->addAction(filerecentAct);
    if (config.AppName!="jqt") {
      fileMenu->addSeparator();
      fileMenu->addAction(filereloadAct);
    }
    fileMenu->addSeparator();
#ifndef QT_NO_PRINTER
    fileMenu->addAction(fileprintAct);
#endif
  }

  fileMenu->addSeparator();
  fileMenu->addAction(filequitAct);
}

// ---------------------------------------------------------------------
void Menu::createfkeyMenu(QString s)
{
#ifdef JQT
  fkeyMenu->addAction(helpvocabAct);
#endif
  fkeyMenu->addAction(helpcontextAct);
  fkeyMenu->addSeparator();
#ifdef JQT
  fkeyMenu->addAction(helpvocabnuvocAct);
  fkeyMenu->addAction(helpcontextnuvocAct);
  fkeyMenu->addSeparator();
#endif
  fkeyMenu->addAction(runtestAct);
  fkeyMenu->addSeparator();
  fkeyMenu->addAction(runclipAct);
  fkeyMenu->addSeparator();
  fkeyMenu->addAction(runprojectAct);
  fkeyMenu->addAction(projectbuildAct);
  if (s=="note") {
    fkeyMenu->addSeparator();
    fkeyMenu->addAction(runalllines1Act);
    fkeyMenu->addAction(runalllines2Act);
  }
  fkeyMenu->addSeparator();
  fkeyMenu->addAction(viewfontplusAct);
  fkeyMenu->addAction(viewfontminusAct);
}

// ---------------------------------------------------------------------
void Menu::createlaunchMenu(QString m)
{
  Q_UNUSED(m);
  if (config.LaunchPadKeys.isEmpty()) return;
  QAction *a;
  runMenu->addSeparator();
  lpadMenu = runMenu->addMenu("Launch Pad");
  foreach(QString s,config.LaunchPadKeys) {
    if(s=="=")
      lpadMenu->addSeparator();
    else {
      a=makeact(config.LaunchPadPrefix+s,s,"");
      lpadMenu->addAction(a);
    }
  }
  connect(lpadMenu,SIGNAL(triggered(QAction *)),
          term,SLOT(launchpad_triggered(QAction *)));
}

// ---------------------------------------------------------------------
void Menu::createprojectMenu(QString s)
{
  projMenu = addMenu("&Project");
  projMenu->addAction(projectnewAct);
  projMenu->addAction(projectopenAct);
  if (s == "note") {
    projMenu->addAction(projectlastAct);
    projMenu->addSeparator();
    projMenu->addAction(projectcloseAct);
    projMenu->addSeparator();
    projMenu->addAction(runprojectAct);
    projMenu->addAction(projectbuildAct);
    if (config.Snapshots) {
      projMenu->addSeparator();
      projMenu->addAction(projectsnapAct);
      projMenu->addAction(projectsnapmakeAct);
    }
    if (config.ifGit) {
      projMenu->addSeparator();
      projMenu->addAction(projectgitguiAct);
      projMenu->addAction(projectgitstatusAct);
    }
    projMenu->addSeparator();
    projMenu->addAction(projectterminalAct);
  } else
    projMenu->addAction(projectcloseAct);
}

// ---------------------------------------------------------------------
void Menu::createrunMenu(QString s)
{
  runMenu = addMenu("&Run");
  if (s == "note") {
    runMenu->addAction(runlineAct);
    runMenu->addAction(runlineadvanceAct);
    runMenu->addSeparator();
    runMenu->addAction(runlineshowAct);
    runMenu->addAction(runlineadvanceshowAct);
    runMenu->addSeparator();
    runMenu->addAction(runselectAct);
    runMenu->addAction(runalllinesAct);
    runMenu->addSeparator();
    runMenu->addAction(runclipAct);
    runMenu->addSeparator();
    runMenu->addAction(runscriptAct);
  } else {
    runMenu->addAction(runselectAct);
    runMenu->addSeparator();
    runMenu->addAction(runclipAct);
  }
  runMenu->addSeparator();
  runMenu->addAction(runtestAct);
  runMenu->addAction(runprojectAct);
  runMenu->addSeparator();
  runMenu->addAction(rundebugAct);
}

// ---------------------------------------------------------------------
void Menu::createselMenu()
{
  selMenu->addAction(toselminusAct);
  selMenu->addAction(toselplusAct);
  selMenu->addAction(toselplusline1Act);
  selMenu->addAction(toselplusline2Act);
  selMenu->addSeparator();
  selMenu->addAction(tosellowerAct);
  selMenu->addAction(toselupperAct);
  selMenu->addAction(toseltoggleAct);
  selMenu->addSeparator();
  selMenu->addAction(toselsortAct);
  selMenu->addSeparator();
  selMenu->addAction(toselviewlinewrapAct);
}

// ---------------------------------------------------------------------
void Menu::createscriptMenu()
{
  scriptMenu = addMenu("&Script");
#ifdef JQT
  scriptMenu->addAction(scriptformatAct);
  scriptMenu->addSeparator();
#endif
  scriptMenu->addAction(scriptrestoreAct);
  scriptMenu->addSeparator();
  scriptMenu->addAction(scriptsnapAct);
  scriptMenu->addSeparator();
  scriptMenu->addAction(scriptglobalsAct);
}

// ---------------------------------------------------------------------
void Menu::createtoolsMenu(QString s)
{
  if (s.size() == 0) return;
  toolsMenu = addMenu("&Tools");
  toolsMenu->addAction(toolsdirmAct);
  toolsMenu->addSeparator();
  toolsMenu->addAction(toolspacmanAct);
#ifndef QT_OS_ANDROID
  toolsMenu->addSeparator();
  fkeyMenu = toolsMenu->addMenu("Function Keys");
  toolsMenu->addSeparator();
  userkeyMenu = toolsMenu->addMenu("User Defined Keys");
  createfkeyMenu(s);
  createuserkeyMenu();
#endif
}

// ---------------------------------------------------------------------
void Menu::createuserkeyMenu()
{
  foreach (QStringList f, config.UserKeys)
  userkeyMenu->addAction(makeuseract(f[2],f[0]));
}

// ---------------------------------------------------------------------
void Menu::createviewMenu(QString s)
{
  viewasciiAct->setChecked(config.Ascii);
  viewlinenosAct->setChecked(config.LineNos);
  viewlinewrapAct->setChecked(config.LineWrap);

  viewMenu = addMenu("&View");
  viewMenu->addAction(editinputlogAct);
#ifdef QT_OS_ANDROID
  viewMenu->addAction(editwdformAct);
#endif
  viewMenu->addSeparator();
  if (s == "note") {
    viewMenu->addAction(viewsidebarAct);
    viewMenu->addAction(viewterminalAct);
  } else
    viewMenu->addAction(vieweditorAct);
  viewMenu->addSeparator();
  viewMenu->addAction(cleartermAct);
#ifdef JQT
  viewMenu->addSeparator();
  viewMenu->addAction(viewasciiAct);
#endif
  viewMenu->addAction(viewlinenosAct);
  if (s == "note") {
    viewMenu->addAction(viewlinewrapAct);
  }
}

// ---------------------------------------------------------------------
void Menu::createwindowMenu(QString s)
{
  if (s.size() == 0) return;
  winMenu = addMenu("&Window");
  winMenu->addAction(winotherAct);
  winMenu->addSeparator();
  winMenu->addAction(winthrowAct);
  winMenu->addSeparator();
  winMenu->addAction(winprojAct);
  winMenu->addSeparator();
  winMenu->addAction(winsourceAct);
  winMenu->addAction(winscriptsAct);
  winMenu->addAction(wintextAct);
  winMenu->addSeparator();
  winMenu->addAction(winfileclosexAct);
}

// ---------------------------------------------------------------------
void Menu::on_userAct_triggered()
{
  int i;
  QString n=sender()->objectName();
  for (i=0; i<config.UserKeys.size(); i++)
    if (config.UserKeys[i][0]==n) break;
  QString mode=config.UserKeys[i][1];
  QString jcmd=config.UserKeys[i][3];
  if (mode=="2")
    tedit->append("   "+jcmd);
  else
    var_runs(jcmd,mode=="1");
}

// ---------------------------------------------------------------------
void Note::on_cfgbaseAct_triggered()
{
  term->on_cfgbaseAct_triggered();

}

// ---------------------------------------------------------------------
void Note::on_cfgdirmAct_triggered()
{
  new Dirm("std");
}

// ---------------------------------------------------------------------
void Note::on_cfgfoldersAct_triggered()
{
  term->on_cfgfoldersAct_triggered();
}


// ---------------------------------------------------------------------
void Note::on_cfglaunchpadAct_triggered()
{
  term->on_cfglaunchpadAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_cfgopenallAct_triggered()
{
  term->on_cfgopenallAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_cfgqtideAct_triggered()
{
  term->on_cfgqtideAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_cfgstartupAct_triggered()
{
  term->on_cfgstartupAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_cfgstyleAct_triggered()
{
  term->on_cfgstyleAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_cfguserkeysAct_triggered()
{
  term->on_cfguserkeysAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_cleartermAct_triggered()
{
  term->on_cleartermAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_clipcopyAct_triggered()
{
  editPage()->copy();
}

// ---------------------------------------------------------------------
void Note::on_clipcutAct_triggered()
{
  editPage()->cut();
}

// ---------------------------------------------------------------------
void Note::on_clippasteAct_triggered()
{
  editPage()->paste();
}

// ---------------------------------------------------------------------
void Note::on_editfifAct_triggered()
{
  QString s;
  if (tabs->count())
    s=((Nedit *)tabs->currentWidget())->readselected();
#ifdef QT_OS_ANDROID
  if (!fif) fif = new Fif(s,false);
  else {
    fif->activateWindow();
    fif->raise();
  }
#else
  new Fif(s,false);
#endif
}

// ---------------------------------------------------------------------
void Note::on_editfiwAct_triggered()
{
  QString s;
  if (tabs->count())
    s=((Nedit *)tabs->currentWidget())->readselected();
#ifdef QT_OS_ANDROID
  if (!fiw) fiw = new Fiw(1,s);
  else {
    fiw->activateWindow();
    fiw->raise();
  }
#else
  new Fiw(1,s);
#endif
}

// ---------------------------------------------------------------------
void Note::on_editfontAct_triggered()
{
  dialogfont(this);
}

// ---------------------------------------------------------------------
void Note::on_editinputlogAct_triggered()
{
  new Slog();
}

#ifdef QT_OS_ANDROID
// ---------------------------------------------------------------------
void Note::on_editwdformAct_triggered()
{
  if (!Forms.isEmpty()) {
    form=Forms.last();
    wdactivateform();
  }
}
#endif

// ---------------------------------------------------------------------
void Note::on_editredoAct_triggered()
{
  editPage()->redo();
}

// ---------------------------------------------------------------------
void Note::on_editundoAct_triggered()
{
  editPage()->undo();
}

// ---------------------------------------------------------------------
void Note::on_filecloseAct_triggered()
{
  tabclose(editIndex());
}

// ---------------------------------------------------------------------
void Note::on_filedeleteAct_triggered()
{
  QString m="OK to delete: " + toprojectname(editPage()->sname) + "?";
  if(!queryOK("File Delete",m)) return;
  QString f=editPage()->fname;
  tabclose(editIndex());
  cfdelete(f);
  tabs->tabsetindex(editIndex());
}

// ---------------------------------------------------------------------
void Note::on_filecloseallAct_triggered()
{
  tabs->tabcloseall();
}

// ---------------------------------------------------------------------
void Note::on_filecloseotherAct_triggered()
{
  int count,i,index;
  count=tabs->count();
  index=editIndex();
  for(i=0; i<index; i++)
    tabclose(0);
  for(i=1; i<count-index; i++)
    tabclose(1);
}

// ---------------------------------------------------------------------
void Note::on_filenewAct_triggered()
{
  newfile(this);
}

// ---------------------------------------------------------------------
void Note::on_filenewtempAct_triggered()
{
  newtemp();
}

// ---------------------------------------------------------------------
void Note::on_fileopenAct_triggered()
{
  openfile(this,"open");
}

// ---------------------------------------------------------------------
void Note::on_fileopenallAct_triggered()
{
  notyet("fileopenallAct");
}

// ---------------------------------------------------------------------
void Note::on_fileopentempAct_triggered()
{
  openfile(this,"temp");
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void Note::on_fileprintAct_triggered()
{
  if(editIndex()>=0)
    dialogprint(note,editPage()->document());
}

// ---------------------------------------------------------------------
void Note::on_fileprintallAct_triggered()
{
  if(editIndex()>=0)
    tabs->tabprintall();
}
#endif

// ---------------------------------------------------------------------
void Note::on_filequitAct_triggered()
{
  term->filequit(false);
}

// ---------------------------------------------------------------------
void Note::on_filerecentAct_triggered()
{
  new Rsel();
}

// ---------------------------------------------------------------------
void Note::on_filesaveAct_triggered()
{
  tabs->tabsave(tabs->currentIndex());
}

// ---------------------------------------------------------------------
void Note::on_filesaveallAct_triggered()
{
  tabs->tabsaveall();
}

// ---------------------------------------------------------------------
void Note::on_filesaveasAct_triggered()
{
  tabs->tabsaveas(tabs->currentIndex());
}

// ---------------------------------------------------------------------
void Note::on_projectbuildAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());

  if (project.Id.isEmpty()) {
    info("Build Project","No project open");
    return;
  }
  QString s=project.Path + defext("/build");
  if (!cfexist(s)) {
    info("Build Project","No project build script found");
    return;
  }
  loadscript(s,true);
}

// ---------------------------------------------------------------------
void Note::on_projectcloseAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());
  projectclose();
}

// ---------------------------------------------------------------------
void Note::on_projectgitguiAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());
  gitgui(project.Path);
}

// ---------------------------------------------------------------------
void Note::on_projectgitstatusAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());
  textview(gitstatus(project.Path));
}

// ---------------------------------------------------------------------
void Note::on_projectlastAct_triggered()
{
  on_lastprojectAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_projectnewAct_triggered()
{
  Pnew *p=new Pnew();
  p->run();
}

// ---------------------------------------------------------------------
void Note::on_projectopenAct_triggered()
{
  new Psel();
}

// ---------------------------------------------------------------------
void Note::on_projectsnapAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());
  new Dirm("snp");
}

// ---------------------------------------------------------------------
void Note::on_projectsnapmakeAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());
  saveall();
  snapshot(true,project.Path);
}

// ---------------------------------------------------------------------
void Note::on_projectterminalAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());
  projectterminal();
}

// ---------------------------------------------------------------------
void Note::on_runalllinesAct_triggered()
{
  runlines(true);
}

// ---------------------------------------------------------------------
void Note::on_runalllines1Act_triggered()
{
  runlines(true);
}

// ---------------------------------------------------------------------
void Note::on_runalllines2Act_triggered()
{
  term->on_cleartermAct_triggered();
  runlines(true);
}

// ---------------------------------------------------------------------
void Note::on_runclipAct_triggered()
{
  term->on_runclipAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_rundebugAct_triggered()
{
  term->on_rundebugAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_runlineAct_triggered()
{
  runline(false,false);
}

// ---------------------------------------------------------------------
void Note::on_runlineadvanceAct_triggered()
{
  runline(true,false);
}

// ---------------------------------------------------------------------
void Note::on_runlineadvanceshowAct_triggered()
{
  runline(true,true);
}

// ---------------------------------------------------------------------
void Note::on_runlineshowAct_triggered()
{
  runline(false,true);
}

// ---------------------------------------------------------------------
void Note::on_runprojectAct_triggered()
{
  term->on_runprojectAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_runscriptAct_triggered()
{
  loadscript(editPage()->fname,true);
}

// ---------------------------------------------------------------------
void Note::on_runselectAct_triggered()
{
  runlines(false);
}

// ---------------------------------------------------------------------
void Note::on_runtestAct_triggered()
{
  term->on_runtestAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_scriptformatAct_triggered()
{
  note->prettyprint();
}

// ---------------------------------------------------------------------
void Note::on_scriptglobalsAct_triggered()
{
  note->scriptglobals();
}

// ---------------------------------------------------------------------
void Note::on_scriptsnapAct_triggered()
{
  new Picm();
}

// ---------------------------------------------------------------------
void Note::on_scriptrestoreAct_triggered()
{
  if (tabs->count()==0) return;
  tabs->tabrestore(tabs->currentIndex());
}

// ---------------------------------------------------------------------
void Note::on_toolsdirmAct_triggered()
{
  new Dirm("std");
}

// ---------------------------------------------------------------------
void Note::on_toolspacmanAct_triggered()
{
  term->on_toolspacmanAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_toolsfkeysAct_triggered()
{
  notyet("toolsfkeysAct");
}

// ---------------------------------------------------------------------
void Note::on_tosellowerAct_triggered()
{
  select_text("lower");
}

// ---------------------------------------------------------------------
void Note::on_toselminusAct_triggered()
{
  select_line("minus");
}

// ---------------------------------------------------------------------
void Note::on_toselplusAct_triggered()
{
  select_line("plus");
}

// ---------------------------------------------------------------------
void Note::on_toselplusline1Act_triggered()
{
  select_line("plusline1");
}

// ---------------------------------------------------------------------
void Note::on_toselplusline2Act_triggered()
{
  select_line("plusline2");
}

// ---------------------------------------------------------------------
void Note::on_toselsortAct_triggered()
{
  select_line("sort");
}

// ---------------------------------------------------------------------
void Note::on_toseltoggleAct_triggered()
{
  select_text("toggle");
}

// ---------------------------------------------------------------------
void Note::on_toselupperAct_triggered()
{
  select_text("upper");
}

// ---------------------------------------------------------------------
void Note::on_toselviewlinewrapAct_triggered()
{
  select_line("wrap");
}

//// ---------------------------------------------------------------------
//void Note::userAct_triggered()
//{
//term->userAct_triggered();
//}

// ---------------------------------------------------------------------
void Note::on_viewasciiAct_triggered()
{
  term->on_viewasciiAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_viewfontminusAct_triggered()
{
  fontdiff(-1);
}

// ---------------------------------------------------------------------
void Note::on_viewfontplusAct_triggered()
{
  fontdiff(1);
}

// ---------------------------------------------------------------------
void Note::on_viewlinenosAct_triggered()
{
  term->on_viewlinenosAct_triggered();
}

// ---------------------------------------------------------------------
void Note::on_viewlinewrapAct_triggered()
{
  config.togglelinewrap();
}

// ---------------------------------------------------------------------
void Note::on_viewsidebarAct_triggered()
{
  sideBarShow=!sideBarShow;
  if (sideBarShow)
    sideBar->show();
  else
    sideBar->hide();
}

// ---------------------------------------------------------------------
void Note::on_viewterminalAct_triggered()
{
  term->activate();
}

// ---------------------------------------------------------------------
void Note::on_winfileclosexAct_triggered()
{
  int i,n;
  QString f;
  Nedit *e;
  if (project.Id.isEmpty()) return;
  n=tabs->count();
  if (n==0) return;
  for (i=n-1; i>=0; i--) {
    e=(Nedit *)tabs->widget(i);
    if (!matchfolder(project.Path,e->fname))
      tabclose(i);
  }
}

// ---------------------------------------------------------------------
void Note::on_winotherAct_triggered()
{
  noevents(1);
  if (note2)
    setnote(note2);
  else {
    setnote(new Note());
  }
  note->setFocus();
  note->show();
  noevents(0);
}

// ---------------------------------------------------------------------
void Note::on_winprojAct_triggered()
{
  QString f=cfpath(tofoldername(editPage()->fname));
  if ("~"==f.at(0))
    f=f.mid(1);
  if (f==Id) return;
  project.open(f);
  projectopen(true);
}

// ---------------------------------------------------------------------
void Note::on_winscriptsAct_triggered()
{
  QString p=cpath("~"+Id);
  QStringList f=qslreverse(cflistfull(p,config.ScriptFilter));
  foreach (QString s,f)
  fileopen(s);
}

// ---------------------------------------------------------------------
void Note::on_winsourceAct_triggered()
{
  QString p=cpath("~"+Id);
  QString pf=p + "/" + cfsname(p) + config.ProjExt;
  QStringList f=qslreverse(cfreadx(pf));
  f=qslprependeach(p + "/",f);
  foreach (QString s,f)
  fileopen(s);
}

// ---------------------------------------------------------------------
void Note::on_wintextAct_triggered()
{
  QString p=cpath("~"+Id);
  QStringList f=qslreverse(cflisttext(p));
  foreach (QString s,f)
  fileopen(s);
}

// ---------------------------------------------------------------------
void Note::on_winthrowAct_triggered()
{
  if (tabs->count()==0) return;
  if (!tabs->tabsave(tabs->currentIndex())) return;
  QString f=editPage()->fname;
  on_winotherAct_triggered();
  note->fileopen(f);
}

// ---------------------------------------------------------------------
void Term::openconfig(QString s)
{
  vieweditor();
  note->fileopen(config.ConfigPath.filePath(s));
}

// ---------------------------------------------------------------------
void Term::on_cfgbaseAct_triggered()
{
  openconfig ("base.cfg");
}

// ---------------------------------------------------------------------
void Term::on_cfgdirmAct_triggered()
{
  openconfig ("dirmatch.cfg");
}

// ---------------------------------------------------------------------
void Term::on_cfgfoldersAct_triggered()
{
  openconfig ("folders.cfg");
}


// ---------------------------------------------------------------------
void Term::on_cfglaunchpadAct_triggered()
{
  openconfig ("launch.cfg");
}

// ---------------------------------------------------------------------
void Term::on_cfgopenallAct_triggered()
{
  openconfig ("style.cfg");
  openconfig ("qtide.cfg");
  openconfig ("launch.cfg");
  openconfig ("folders.cfg");
  openconfig ("dirmatch.cfg");
  openconfig ("base.cfg");
}

// ---------------------------------------------------------------------
void Term::on_cfgqtideAct_triggered()
{
  openconfig ("qtide.cfg");
}

// ---------------------------------------------------------------------
void Term::on_cfgstartupAct_triggered()
{
  QString s = "startup.ijs";
  cftouch(config.ConfigPath.filePath(s));
  openconfig(s);
}

// ---------------------------------------------------------------------
void Term::on_cfgstyleAct_triggered()
{
  openconfig ("style.cfg");
}

// ---------------------------------------------------------------------
void Term::on_cfguserkeysAct_triggered()
{
  openconfig ("userkeys.cfg");
}

// ---------------------------------------------------------------------
void Term::on_cleartermAct_triggered()
{
  tedit->clear();
  tedit->setprompt();
}

// ---------------------------------------------------------------------
void Term::on_clipcopyAct_triggered()
{
  tedit->copy();
}

// ---------------------------------------------------------------------
void Term::on_clipcutAct_triggered()
{
  tedit->cut();
}

// ---------------------------------------------------------------------
void Term::on_clippasteAct_triggered()
{
  tedit->paste();
}

// ---------------------------------------------------------------------
void Term::on_editfifAct_triggered()
{
#ifdef QT_OS_ANDROID
  if (!fif) fif = new Fif(tedit->readselected(),false);
  else {
    fif->activateWindow();
    fif->raise();
  }
#else
  new Fif(tedit->readselected(),false);
#endif
}

// ---------------------------------------------------------------------
void Term::on_editfiwAct_triggered()
{
#ifdef QT_OS_ANDROID
  if (!fiw) fiw = new Fiw(0,tedit->readselected());
  else {
    fiw->activateWindow();
    fiw->raise();
  }
#else
  new Fiw(0,tedit->readselected());
#endif
}

// ---------------------------------------------------------------------
void Term::on_editfontAct_triggered()
{
  dialogfont(this);
}

// ---------------------------------------------------------------------
void Term::on_editinputlogAct_triggered()
{
  new Slog();
}

#ifdef QT_OS_ANDROID
// ---------------------------------------------------------------------
void Term::on_editwdformAct_triggered()
{
  if (!Forms.isEmpty()) {
    form=Forms.last();
    wdactivateform();
  }
}
#endif

// ---------------------------------------------------------------------
void Term::on_filedeleteAct_triggered()
{
  notyet("filedeleteAct");
}

// ---------------------------------------------------------------------
void Term::on_filenewtempAct_triggered()
{
  note->newtemp();
}

// ---------------------------------------------------------------------
void Term::on_fileopenAct_triggered()
{
  openfile(this,"open");
}

// ---------------------------------------------------------------------
void Term::on_fileopenaddonsAct_triggered()
{
  openfile(this,"addons");
}

// ---------------------------------------------------------------------
void Term::on_fileopenallAct_triggered()
{
  notyet("fileopenallAct");
}

// ---------------------------------------------------------------------
void Term::on_fileopensystemAct_triggered()
{
  openfile(this,"system");
}

// ---------------------------------------------------------------------
void Term::on_fileopentempAct_triggered()
{
  openfile(this,"temp");
}

// ---------------------------------------------------------------------
void Term::on_fileopenuserAct_triggered()
{
  openfile(this,"user");
}

#ifndef QT_NO_PRINTER
// ---------------------------------------------------------------------
void Term::on_fileprintAct_triggered()
{
  dialogprint(term,tedit->document());
}
#endif

// ---------------------------------------------------------------------
void Term::on_filequitAct_triggered()
{
  filequit(true);
}

// ---------------------------------------------------------------------
void  Term::on_filerecentAct_triggered()
{
  new Rsel();
}

// ---------------------------------------------------------------------
void Term::on_filereloadAct_triggered()
{
  state_reinit();
}

// ---------------------------------------------------------------------
void Term::on_projectcloseAct_triggered()
{
  projectclose();
}

// ---------------------------------------------------------------------
void Term::on_projectnewAct_triggered()
{
  Pnew *p=new Pnew();
  if (p->run())
    term->vieweditor();
}

// ---------------------------------------------------------------------
void Term::on_projectopenAct_triggered()
{
  new Psel();
}

// ---------------------------------------------------------------------
void Term::on_runclipAct_triggered()
{
  tedit->docmds(QApplication::clipboard()->text().trimmed(),true);
}

// ---------------------------------------------------------------------
void Term::on_rundebugAct_triggered()
{
  tedit->docmds("dbg_z_ 1", false);
}

// ---------------------------------------------------------------------
void Term::on_runprojectAct_triggered()
{
  Q_ASSERT(!project.Id.isEmpty());
  if (project.Id.isEmpty()) {
    info("Run Project","No project open");
    return;
  }

  QString r=project.Path + defext("/run");
  if (!cfexist(r)) {
    info("Run Project","No project run script found");
    return;
  }

  if (note)
    note->saveall();

  QString b=project.Path + defext("/build");

  if (cfexist(b))
    tedit->loadscript(b,false);

  tedit->loadscript(r,true);
}

// ---------------------------------------------------------------------
void Term::on_runselectAct_triggered()
{
  runlines();
}

// ---------------------------------------------------------------------
void Term::on_runtestAct_triggered()
{
  var_run("test ''");
}

// ---------------------------------------------------------------------
void Term::on_toolsdirmAct_triggered()
{
  new Dirm("std");
}

// ---------------------------------------------------------------------
void Term::on_toolspacmanAct_triggered()
{
  term->pacman();
}

// ---------------------------------------------------------------------
void Term::on_toolsfkeysAct_triggered()
{
  notyet("toolsfkeysAct");
}

// ---------------------------------------------------------------------
void Term::on_tosellowerAct_triggered()
{
  notyet("tosellowerAct");
}

// ---------------------------------------------------------------------
void Term::on_toselminusAct_triggered()
{
  notyet("toselminusAct");
}

// ---------------------------------------------------------------------
void Term::on_toselplusAct_triggered()
{
  notyet("toselplusAct");
}

// ---------------------------------------------------------------------
void Term::on_toselplusline1Act_triggered()
{
  notyet("toselplusline1Act");
}

// ---------------------------------------------------------------------
void Term::on_toselplusline2Act_triggered()
{
  notyet("toselplusline2Act");
}

// ---------------------------------------------------------------------
void Term::on_toselsortAct_triggered()
{
  notyet("toselsortAct");
}

// ---------------------------------------------------------------------
void Term::on_toseltoggleAct_triggered()
{
  notyet("toseltoggleAct");
}

// ---------------------------------------------------------------------
void Term::on_toselupperAct_triggered()
{
  notyet("toselupperAct");
}

// ---------------------------------------------------------------------
void Term::on_toselviewlinewrapAct_triggered()
{
  notyet("toselviewlinewrapAct");
}

// ---------------------------------------------------------------------
void Term::on_viewasciiAct_triggered()
{
  config.toggleascii();
}

// ---------------------------------------------------------------------
void Term::on_vieweditorAct_triggered()
{
  vieweditor();
}

// ---------------------------------------------------------------------
void Term::on_viewfontminusAct_triggered()
{
  fontdiff(-1);
}

// ---------------------------------------------------------------------
void Term::on_viewfontplusAct_triggered()
{
  fontdiff(1);
}

// ---------------------------------------------------------------------
void Term::on_viewlinenosAct_triggered()
{
  config.togglelinenos();
}

// ---------------------------------------------------------------------
void Term::on_viewsidebarAct_triggered()
{
  notyet("viewsidebarAct");
}

