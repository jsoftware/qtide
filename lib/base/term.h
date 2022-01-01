#ifndef TERM_H
#define TERM_H

#include <QListWidgetItem>
#include <QToolBar>

class QAction;
class QListWidgetItem;
class QSplitter;
class QTimer;
class QToolBar;
class Menu;
class Tedit;
class Note;
class QHBoxLayout;
#ifdef TABCOMPLETION
class QCompleter;
#endif

// ---------------------------------------------------------------------
class OneWin : public QWidget
{
  Q_OBJECT

public:
  OneWin();
  QFrame *makeframe(QWidget *w);
  QSplitter *split;

private:
  void closeEvent(QCloseEvent *event);
};

// ---------------------------------------------------------------------
class Term : public QWidget
{
  Q_OBJECT

public:
  Term();

  void activate();
  void cleantemp();
  void displayform(int p);
  bool filequit(bool ignoreconfirm);
  void fini();
  void load(QString s, bool display);
  void openconfig(QString s);
  void pacman();
  void projectenable();
  void removeprompt();
  void runlines();
  void smact();
  void smprompt(QString s);
  void vieweditor();
#ifdef _WIN32
#if defined QT60
  bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
#else
  bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif
#endif

  Menu *menuBar;

public slots:

  void launchpad_triggered();
  void systimer();

  void on_cfgbaseAct_triggered();
  void on_cfgcaseAct_triggered();
  void on_cfgqtideAct_triggered();
  void on_cfgdirmAct_triggered();
  void on_cfgfoldersAct_triggered();
  void on_cfglaunchpadAct_triggered();
  void on_cfgopenallAct_triggered();
  void on_cfgcurposAct_triggered();
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

  void on_filedeleteAct_triggered();
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
#endif
  void on_filequitAct_triggered();
  void on_filerecentAct_triggered();
  void on_filereloadAct_triggered();

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

  void on_projectcloseAct_triggered();
  void on_projectnewAct_triggered();
  void on_projectopenAct_triggered();
  void on_runclipAct_triggered();
  void on_rundebugAct_triggered();
  void on_runprojectAct_triggered();
  void on_runselectAct_triggered();
  void on_runtestAct_triggered();
  void on_toolsdirmAct_triggered();
  void on_toolsfkeysAct_triggered();
  void on_toolspacmanAct_triggered();

  void on_viewasciiAct_triggered();
  void on_vieweditorAct_triggered();
  void on_viewfontminusAct_triggered();
  void on_viewfontplusAct_triggered();
  void on_viewsidebarAct_triggered();

private:
  void changeEvent(QEvent *e);
  void keyPressEvent(QKeyEvent *e);
  void resizeEvent(QResizeEvent *event);
  void closeEvent(QCloseEvent *event);

  QToolBar *editToolBar;
  QToolBar *fileToolBar;
  Note *wNote;
  Note *wNote2;

};

extern Term *term;
extern QTimer *timer;
#ifdef TABCOMPLETION
extern QCompleter *completer;
#endif

#endif
