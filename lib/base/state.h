#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QDir>
#include <QFont>
#include <QMap>

#include "base.h"

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
  void launch_init();
  void init(QString);
  void noprofile();

  void toggleascii();
  void togglelinenos();
  void togglelinewrap();
  void winpos_init();
  QList<int> winpos_read(QString id);
  void winpos_save(QWidget *w,QString id);
  void winpos_save1(QList<int>d,QString id);

  QStringList AllFolderKeys;
  QStringList AllFolderValues;
  QString AppName;
  bool Ascii;
  QDir ConfigPath;
  bool ConfirmClose;
  bool ConfirmSave;
  QString DefCmt;
  QString DefExt;
  QString DefIndent;
  QStringList DefTypes;
  QString FilePatterns;
  QStringList DMFavorites;
  QString DMType;
  int DMTypeIndex;
  QStringList DMTypes;
  QStringList DMTypex;
  int EditPos[4];
  bool EscClose;
  QStringList FifExt;
  QMap<QString,int> FilePos;
  QList<int> FKeyKeys;
  QList<QStringList> FKeyValues;
  QFont Font;
  QString Host;
  bool KeepInputLog;
  QStringList LaunchPadKeys;
  QStringList LaunchPadValues;
  QString LaunchPadPrefix;
  bool LineNos;
  bool LineWrap;
  int MaxRecent;
  bool NoProfile;
  QString Pass;
  int Port;
  QString ProjExt;
  QFont ProFont;
  QString Rxnna;
  QString Rxnnz;
  QString RunQ;
  QString ScriptFilter;
  QDir SnapPath;
  int Snapshots;
  QString Snapshotx;
  QDir SystemPath;
  QDir TempPath;
  QString Terminal;
  int TermPos[4];
  QString User;
  QDir UserPath;
  QStringList UserFolderKeys;
  QStringList UserFolderValues;
  QMap<QString,QList<int> >WinPos;
  QString XDiff;
};

extern Config config;

void state_appname();
QStringList state_about();
int state_fini(QApplication *);
bool state_init(int argc, char *argv[]);
void state_init_resource();
void state_quit();
void state_reinit();

void var_cmd(QString s);
QString var_load(QString s, bool d);
void var_run(QString s);
void var_runs(QString s);

#ifdef ANDROID
int state_run(int argc, char *argv[],QApplication *app,QString lib,void *,void *,void *);
#else
extern "C" {
Dllexport int state_run(int argc, char *argv[],QApplication *app,QString lib);
}
#endif

#endif
