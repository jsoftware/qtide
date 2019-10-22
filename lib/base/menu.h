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
  void createuserkeyMenu();

  QAction *projectcloseAct;
  QAction *rundebugAct;
  QAction *runlineadvanceshowAct;
  QAction *runlineshowAct;
  QAction *runprojectAct;
  QAction *viewasciiAct;
  QAction *viewlinenosAct;
  QAction *viewlinewrapAct;
  QAction *helpaboutAct;

  QMenu *selMenu;
  QMenu *viewMenu;

  QList<QAction *> GitEnable;
  QList<QAction *> ProjectEnable;
  QList<QAction *> ScriptEnable;

private slots:
  void on_userAct_triggered();

private:
  void createcfgMenu();
  void createdfmMenu();
  void createeditMenu(QString s);
  void createfileMenu(QString s);
  void createfopenMenu();
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
  QAction *makeuseract(QString text, QString shortcut);

  QMenu *cfgMenu;
  QMenu *dfmMenu;
  QMenu *editMenu;
  QMenu *fileMenu;
  QMenu *fkeyMenu;
  QMenu *fopenMenu;
  QMenu *helpMenu;
  QMenu *lpadMenu;
  QMenu *projMenu;
  QMenu *runMenu;
  QMenu *scriptMenu;
  QMenu *toolsMenu;
  QMenu *userkeyMenu;
  QMenu *winMenu;

  QAction *cfgbaseAct;
  QAction *cfgcaseAct;
  QAction *cfgdirmAct;
  QAction *cfgfoldersAct;
  QAction *cfglaunchpadAct;
  QAction *cfgopenallAct;
  QAction *cfgcurposAct;
  QAction *cfgqtideAct;
  QAction *cfgstartupAct;
  QAction *cfguserkeysAct;
  QAction *cfgstyleAct;
  QAction *cleartermAct;
  QAction *clipcopyAct;
  QAction *clipcutAct;
  QAction *clippasteAct;
  QAction *dfm2Act;
  QAction *dfm4Act;
  QAction *dfm5Act;
  QAction *dfm6Act;
  QAction *editfifAct;
  QAction *editfiwAct;
  QAction *editfontAct;
  QAction *editinputlogAct;
  QAction *editredoAct;
  QAction *editundoAct;
  QAction *filecloseAct;
  QAction *filecloseallAct;
  QAction *filecloseotherAct;
  QAction *filedeleteAct;
  QAction *filenewAct;
  QAction *filenewtempAct;
  QAction *fileopenAct;
  QAction *fileopenaddonsAct;
  QAction *fileopenallAct;
  QAction *fileopencurrentAct;
  QAction *fileopenhomeAct;
  QAction *fileopenlastAct;
  QAction *fileopensystemAct;
  QAction *fileopentempAct;
  QAction *fileopenuserAct;
#ifndef QT_NO_PRINTER
  QAction *fileprintAct;
  QAction *fileprintpreviewAct;
  QAction *fileprintallAct;
#endif
  QAction *filequitAct;
  QAction *filerecentAct;
  QAction *filereloadAct;
  QAction *filesaveAct;
  QAction *filesaveallAct;
  QAction *filesaveasAct;
  QAction *projectbuildAct;
  QAction *projectfilemanagerAct;
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
  QAction *toselcommentAct;
  QAction *tosellowerAct;
  QAction *toselnoteAct;
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

  static QAction *sharedHelpAboutAct; // QTBUG-17941
  QAction *helpcontextAct;
  QAction *helpconstantsAct;
  QAction *helpcontrolsAct;
  QAction *helpdemoqtAct;
  QAction *helpdemowdAct;
  QAction *helpstdlibAct;
  QAction *helpforeignsAct;
  QAction *helplabsAct;
  QAction *helplabsadvanceAct;
  QAction *helplabschaptersAct;
  QAction *helprelnotesAct;
  QAction *helpvocabAct;
  QAction *helpwdAct;
  QAction *helpwikiAct;
};

#endif
