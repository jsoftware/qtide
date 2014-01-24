#ifndef MENU_H
#define MENU_H

#include <QMenuBar>

class QAction;
class QMenu;

class Menu : public QMenuBar
{
  Q_OBJECT

public:
  Menu() {};

  void createActions();
  void createhelpActions();
  void createMenus(QString);
  void createMenus_fini(QString s);

  QAction *projectcloseAct;
  QAction *rundebugAct;
  QAction *runprojectAct;
  QAction *viewasciiAct;
  QAction *viewlinenosAct;
  QAction *viewlinewrapAct;

  QMenu *selMenu;
  QMenu *viewMenu;

  QList<QAction *> GitEnable;
  QList<QAction *> ProjectEnable;
  QList<QAction *> ScriptEnable;

private:
  void createcfgMenu();
  void createeditMenu(QString s);
  void createfileMenu(QString s);
  void createfkeyMenu(QString s);
  void createhelpMenu();
  void createlaunchMenu(QString s);
  void createprojectMenu(QString s);
  void createrunMenu(QString s);
  void createscriptMenu();
  void createselMenu();
  void createtoolsMenu(QString s);
  void createviewMenu(QString s);
  void createwindowMenu(QString s);

  QAction *makeact(QString id, QString text, QString shortcut);

  QMenu *cfgMenu;
  QMenu *editMenu;
  QMenu *fileMenu;
  QMenu *fkeyMenu;
  QMenu *helpMenu;
  QMenu *lpadMenu;
  QMenu *projMenu;
  QMenu *runMenu;
  QMenu *scriptMenu;
  QMenu *toolsMenu;
  QMenu *winMenu;

  QAction *cfgbaseAct;
  QAction *cfgdirmAct;
  QAction *cfgfoldersAct;
  QAction *cfglaunchpadAct;
  QAction *cfgopenallAct;
  QAction *cfgqtideAct;
  QAction *cfgstartupAct;
  QAction *cfgstyleAct;
  QAction *cleartermAct;
  QAction *clipcopyAct;
  QAction *clipcutAct;
  QAction *clippasteAct;
  QAction *editfifAct;
  QAction *editfiwAct;
  QAction *editfontAct;
  QAction *editinputlogAct;
#ifdef QT_OS_ANDROID
  QAction *editwdformAct;
#endif
  QAction *editredoAct;
  QAction *editundoAct;
  QAction *filecloseAct;
  QAction *filecloseallAct;
  QAction *filecloseotherAct;
  QAction *filedeleteAct;
  QAction *filenewAct;
  QAction *filenewtempAct;
  QAction *fileopenAct;
  QAction *fileopenallAct;
  QAction *fileopensystemAct;
  QAction *fileopentempAct;
  QAction *fileopenuserAct;
#ifndef QT_NO_PRINTER
  QAction *fileprintAct;
  QAction *fileprintallAct;
#endif
  QAction *filequitAct;
  QAction *filerecentAct;
  QAction *filereloadAct;
  QAction *filesaveAct;
  QAction *filesaveallAct;
  QAction *filesaveasAct;
  QAction *projectbuildAct;
  QAction *projectgitguiAct;
  QAction *projectgitstatusAct;
  QAction *projectlastAct;
  QAction *projectnewAct;
  QAction *projectopenAct;
  QAction *projectsnapAct;
  QAction *projectsnapmakeAct;
  QAction *projectterminalAct;
  QAction *runalllines1Act;
  QAction *runalllines2Act;
  QAction *runalllinesAct;
  QAction *runclipAct;
  QAction *runlineAct;
  QAction *runlineadvanceAct;
  QAction *runlineadvanceshowAct;
  QAction *runlineshowAct;
  QAction *runscriptAct;
  QAction *runselectAct;
  QAction *runtestAct;
  QAction *scriptformatAct;
  QAction *scriptglobalsAct;
  QAction *scriptrestoreAct;
  QAction *scriptsnapAct;
  QAction *toolsdirmAct;
  QAction *toolsfkeysAct;
  QAction *toolslaunchpadAct;
  QAction *toolspacmanAct;
  QAction *tosellowerAct;
  QAction *toselminusAct;
  QAction *toselplusAct;
  QAction *toselplusline1Act;
  QAction *toselplusline2Act;
  QAction *toselsortAct;
  QAction *toseltoggleAct;
  QAction *toselupperAct;
  QAction *toselviewlinewrapAct;
  QAction *vieweditorAct;
  QAction *viewfontminusAct;
  QAction *viewfontplusAct;
  QAction *viewsidebarAct;
  QAction *viewterminalAct;
  QAction *winfileclosexAct;
  QAction *winotherAct;
  QAction *winprojAct;
  QAction *winscriptsAct;
  QAction *winsourceAct;
  QAction *wintextAct;
  QAction *winthrowAct;

  QAction *helpaboutAct;
  QAction *helpcontextAct;

#ifdef JQT
  QAction *helpconstantsAct;
  QAction *helpcontrolsAct;
  QAction *helpdictionaryAct;
  QAction *helpforeignsAct;
  QAction *helpgeneralAct;
  QAction *helphelpAct;
  QAction *helplabsAct;
  QAction *helplabsadvanceAct;
  QAction *helplabschaptersAct;
  QAction *helplabsauthorAct;
  QAction *helpreleaseAct;
  QAction *helprelnotesAct;
  QAction *helpvocabAct;
#else
  QAction *helpbriefAct;
  QAction *helpintercAct;
  QAction *helpreferenceAct;
  QAction *helpwikiAct;
#endif

};

#endif
