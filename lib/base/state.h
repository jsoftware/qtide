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

class Config : public QObject
{

  Q_OBJECT

public:
  Config() {};

  QString config_path(QString);
  void config_init();
  void dirmatch_init();
  int filepos_get(QString f);
  void filepos_set(QString f, int p);
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
#ifdef QT_OS_ANDROID
  QString FontFile;
  int ScrollBarSize;
#endif
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
  QString FilePatterns;
  QStringList DMFavorites;
  QString DMType;
  int DMTypeIndex;
  QStringList DMTypes;
  QStringList DMTypex;
  QList<int>EditPos;
  QList<int>EditPosX;
  bool EscClose;
  QStringList FifExt;
  QMap<QString,int> FilePos;
  QList<int> FKeyKeys;
  QList<QStringList> FKeyValues;
  QFont Font;
  QString Host;
  bool ifGit;
  QString Lang;
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
  QFont ProFont;
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
  Style TermFore;
  Style TermBack;
  Style TermHigh;

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

extern Config config;
#ifdef QT_OS_ANDROID
extern int androidVfuncPos;
#endif

QSyntaxHighlighter *highlight(QTextDocument *);

void state_appname();
QStringList state_about();
int state_fini();
bool state_init(int argc, char *argv[], void *jproc);
void state_init_args(int *,char *argv[]);

void state_init_resource();
void state_quit();
void state_reinit();

void var_cmd(QString s);
void var_cmddo(QString s);
QString var_cmdr(QString s);
QString var_load(QString s, bool d);
void var_run(QString s);
void var_runs(QString s, bool show);
void var_set(QString s, QString t);

extern "C" {
  Dllexport int state_run(int argc, char *argv[],char *lib,bool fhs, void *jproc);
  Dllexport void immexj(const char *s);
}

#endif
