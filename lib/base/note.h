#ifndef NOTE_H
#define NOTE_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QSplitter>

class QAction;
class Menu;
class Nedit;
class Nmain;
class Nside;
class Ntabs;

class Note : public QWidget
{
  Q_OBJECT

public:
  Note();
  ~Note();

  void activate();
  int count();
  int editIndex();
  QString editFile();
  Nedit *editPage();
  QString editText();
  void fileclose(QString f);
  bool fileopen(QString,int n=-1);
  bool filereplace(QString,int n=-1);
  std::string gettabstate();
  void gitenable(bool b);
  void loadscript(QString s,bool show);
  void newtemp();
  void prettyprint();
  void projectenable();
  void projectopen(bool);
  void projectsave();
  void runlines(bool all, bool show=true);
  bool saveall();
  void savecurrent();
  void scriptenable();
  void scriptglobals();
  void setfont(QFont font);
  void setid();
  void setindex(int index);
  void setlinenos(bool);
  void setlinewrap(bool);
  void setmodified(int, bool);
  void setpos();
  void settext(QString s);
  void settitle(QString name, bool mod);
  void settitle2(bool);
  void siderefresh();
  void tabclose(int index);

  QString Id;
  Nmain *mainBar;
  Menu *menuBar;
  Nside *sideBar;
  Ntabs *tabs;

signals:

public slots:

  void on_cfgbaseAct_triggered();
  void on_cfgcaseAct_triggered();
  void on_cfgdirmAct_triggered();
  void on_cfgfoldersAct_triggered();
  void on_cfglaunchpadAct_triggered();
  void on_cfgopenallAct_triggered();
  void on_cfgcurposAct_triggered();
  void on_cfgqtideAct_triggered();
  void on_cfgstartupAct_triggered();
  void on_cfgstyleAct_triggered();
  void on_cfguserkeysAct_triggered();
  void on_cleartermAct_triggered();
  void on_clipcopyAct_triggered();
  void on_clipcutAct_triggered();
  void on_clippasteAct_triggered();
  void on_dfm2Act_triggered();
  void on_dfm4Act_triggered();
  void on_dfm5Act_triggered();
  void on_dfm6Act_triggered();
  void on_editfifAct_triggered();
  void on_editfiwAct_triggered();
  void on_editfontAct_triggered();
  void on_editinputlogAct_triggered();
  void on_editredoAct_triggered();
  void on_editundoAct_triggered();
  void on_filecloseAct_triggered();
  void on_filedeleteAct_triggered();
  void on_filecloseallAct_triggered();
  void on_filecloseotherAct_triggered();
  void on_filesaveallAct_triggered();
  void on_filesaveasAct_triggered();
  void on_filenewAct_triggered();
  void on_filenewtempAct_triggered();
  void on_fileopenAct_triggered();
  void on_fileopenaddonsAct_triggered();
  void on_fileopenallAct_triggered();
  void on_fileopencurrentAct_triggered();
  void on_fileopenhomeAct_triggered();
  void on_fileopenlastAct_triggered();
  void on_fileopensystemAct_triggered();
  void on_fileopentempAct_triggered();
  void on_fileopenuserAct_triggered();
#ifndef QT_NO_PRINTER
  void on_fileprintAct_triggered();
  void on_fileprintpreviewAct_triggered();
  void on_fileprintallAct_triggered();
#endif
  void on_filequitAct_triggered();
  void on_filerecentAct_triggered();
  void on_scriptrestoreAct_triggered();
  void on_filesaveAct_triggered();
  void on_helpaboutAct_triggered();
  void on_helpcontextAct_triggered();
  void on_helpconstantsAct_triggered();
  void on_helpcontrolsAct_triggered();
  void on_helpdemoqtAct_triggered();
  void on_helpdemowdAct_triggered();
  void on_helpstdlibAct_triggered();
  void on_helpforeignsAct_triggered();
  void on_helplabsAct_triggered();
  void on_helplabsadvanceAct_triggered();
  void on_helplabschaptersAct_triggered();
  void on_helprelnotesAct_triggered();
  void on_helpvocabAct_triggered();
  void on_helpwdAct_triggered();
  void on_helpwikiAct_triggered();

  void on_projectbuildAct_triggered();
  void on_projectcloseAct_triggered();
  void on_projectfilemanagerAct_triggered();
  void on_projectgitguiAct_triggered();
  void on_projectgitstatusAct_triggered();
  void on_projectlastAct_triggered();
  void on_projectnewAct_triggered();
  void on_projectopenAct_triggered();
  void on_projectsnapAct_triggered();
  void on_projectsnapmakeAct_triggered();
  void on_projectterminalAct_triggered();

  void on_runalllinesAct_triggered();
  void on_runalllines1Act_triggered();
  void on_runalllines2Act_triggered();
  void on_runclipAct_triggered();
  void on_rundebugAct_triggered();
  void on_runlineAct_triggered();
  void on_runlineadvanceAct_triggered();
  void on_runlineadvanceshowAct_triggered();
  void on_runlineshowAct_triggered();
  void on_runprojectAct_triggered();
  void on_runscriptAct_triggered();
  void on_runselectAct_triggered();
  void on_runtestAct_triggered();
  void on_scriptformatAct_triggered();
  void on_scriptglobalsAct_triggered();
  void on_scriptsnapAct_triggered();
  void on_toolsdirmAct_triggered();
  void on_toolsfkeysAct_triggered();
  void on_toolspacmanAct_triggered();
  void on_toselcommentAct_triggered();
  void on_tosellowerAct_triggered();
  void on_toselnoteAct_triggered();
  void on_toselplusline1Act_triggered();
  void on_toselplusline2Act_triggered();
  void on_toselsortAct_triggered();
  void on_toseltoggleAct_triggered();
  void on_toselupperAct_triggered();
  void on_toselviewlinewrapAct_triggered();

  void on_viewasciiAct_triggered();
  void on_viewfontminusAct_triggered();
  void on_viewfontplusAct_triggered();
  void on_viewlinenosAct_triggered();
  void on_viewlinewrapAct_triggered();
  void on_viewsidebarAct_triggered();
  void on_viewterminalAct_triggered();

  void on_winfileclosexAct_triggered();
  void on_winotherAct_triggered();
  void on_winprojAct_triggered();
  void on_winscriptsAct_triggered();
  void on_winsourceAct_triggered();
  void on_wintextAct_triggered();
  void on_winthrowAct_triggered();

// main toolbar:
  void on_lastprojectAct_triggered();
  void on_openprojectAct_triggered();
  void on_runallAct_triggered();

private:
  void createActions();
  void createMenus();

  void changeEvent(QEvent *event);
  void closeEvent(QCloseEvent *event);
  bool isMultiline(QString);
  bool isNoteline(QString);
  void keyPressEvent(QKeyEvent *e);
  bool maybeSave();
  QString readentry(Nedit *, int *);
  QString readnextentry(QStringList,QString,QString, int*);
  void replacetext(Nedit *e, QString txt);

  void runline(bool advance, bool show);
  void selectline(int linenum);
  void select_line(QString s);
  QStringList select_line1(QStringList mid,QString s,int *pos, int *len);
  QString select_line_note(QString txt,int *pos, int *len);
  void select_text(QString s);

  bool sideBarShow;
  QString EditText;
  QToolBar *editToolBar;
  QToolBar *fileToolBar;
  QSplitter *split;
};

extern Note *note;
extern Note *note2;

#endif
