#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QDir>
#include <QFont>
#include <QMap>
#include "base.h"
#include "style.h"

#ifndef QT_NO_PRINTER
class QPrinter;
#endif
class QSyntaxHighlighter;
class QTextDocument;
class QApplication;

class Config : public QObject
{

  Q_OBJECT

public:
  Config() {};

  QString config_path(QString);
  void config_init();
  void dirmatch_init();
  int filecur_get(QString f);
  void filecur_set(QString f, int p);
  int filetop_get(QString f);
  void filetop_set(QString f, int p);
  void fkeys_init();
  void folder_init();
  QStringList getuserkeys();
  void ini0();
  void init();
  void initide();
  QList<int> initposX(QList<int>);
  bool initide1(QString f);
  void initstyle();
  void launch_init();
  void noprofile();
  void setfolders();
  void toggleascii();
  void togglelinenos();
  void togglelinewrap();
  void userkeys_init();
  QStringList userkeys_split(QString s);
  void winpos_init();
  QList<int> winpos_read(QString id);
  void winpos_save(QWidget *w,QString id);
  void winpos_save1(QList<int>d,QString id);

  QDir AddonsPath;
  QStringList AllFolderKeys;
  QStringList AllFolderValues;
  QString AppName;
  bool Ascii;
  QDir BinPath;
  int BoxForm;
#ifdef TABCOMPLETION
  bool Completion;
  QString CompletionFile;
#endif
  QDir ConfigPath;
  bool ConfirmClose;
  bool ConfirmSave;
  QList<int> DebugPos;
  QList<int> DebugPosX;
  QString DefCmt;
  QString DefCCmt;
  QString DefExt;
  QString DefIndent;
  QStringList DefTypes;
  QStringList DirTreeX;
  QStringList DMFavorites;
  QString DMType;
  int DMTypeIndex;
  QStringList DMTypes;
  QStringList DMTypex;
  QList<int>EditPos;
  QList<int>EditPosX;
  bool EscClose;
  QStringList FifExt;
  QString FilePatterns;
  QMap<QString,int> FileCur;
  QMap<QString,int> FileTop;
  QList<int> FKeyKeys;
  QList<QStringList> FKeyValues;
  QFont Font;
  QDir HomePath;
  QString Host;
  bool ifGit;
  bool KeepCursorPosOnRecall;
  QString Lang;
  QString LastFileOpen;
  QStringList LaunchPadKeys;
  QStringList LaunchPadValues;
  QString LaunchPadPrefix;
  bool LineNos;
  bool LineWrap;
  int MaxInputLog;
  int MaxRecent;
  bool NoProfile;
  int OpenTabAt;
  QString Pass;
  int Port;
#ifndef QT_NO_PRINTER
  QPrinter *Printer;
#endif
  QString ProjExt;
  bool ProjInit;
  QString Rxnna;
  QString Rxnnz;
  QString RunQ;
  int ScreenHeight;
  int ScreenWidth;
  QString ScriptFilter;
  bool SingleWin;
  QDir SnapPath;
  int Snapshots;
  QString Snapshotx;
  QDir SystemPath;
  QDir TempPath;
  QString Terminal;
  QList<int> TermPos;
  QList<int> TermPosX;
  bool TermSyntaxHighlight;
  bool TrimTrailingWS;
  QString User;
  QDir UserPath;
  QStringList UserFolderKeys;
  QStringList UserFolderValues;
  QList<QStringList> UserKeys;
  QMap<QString,QList<int> >WinPos;
  QString XDiff;

  Style EditFore;
  Style EditBack;
  Style EditHigh;
  Style EditSelb;
  Style EditSelt;
  Style TermFore;
  Style TermBack;
  Style TermHigh;
  Style TermSelb;
  Style TermSelt;
  Style ViewFore;
  Style ViewBack;
  Style ViewHigh;
  Style ViewSelb;
  Style ViewSelt;

  Style adverbStyle;
  Style commentStyle;
  Style conjunctionStyle;
  Style controlStyle;
  Style functionStyle;
  Style keywordStyle;
  Style nounStyle;
  Style noundefStyle;
  Style numberStyle;
  Style stringStyle;
  Style verbStyle;
};

extern QList<QWidget*> ActiveWindows;
extern Config config;

QSyntaxHighlighter *highlight(QTextDocument *);

void delactivewindow(QWidget*);
QWidget* getactivewindow();
void setactivewindow(QWidget*);

extern QApplication *app;
void state_appname();
QStringList state_about();
int state_fini();
bool state_init(int argc, char *argv[], void *jproc);
void state_init_args(int *,char *argv[]);

void state_init_resource();
void state_quit();
void state_reinit();

extern "C" {
  Dllexport int state_run(int argc, char *argv[], char *lib, bool fhs, int fshowide, void *jproc, void *jt, void **jdll, void **jst);
  Dllexport void immexj(const char *s);
}

#endif
