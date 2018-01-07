#ifndef DIRM_H
#define DIRM_H

#include <QDialog>
#include <QPushButton>

#include "plaintextedit.h"

class Dirm;
class QTableWidget;
class QMenuBar;
class QCheckBox;
class QComboBox;
class QComboBox;
class QLabel;
class QListWidget;
class QTableWidget;

// ---------------------------------------------------------------------
class hPushButton : public QPushButton
{
  Q_OBJECT

public:
  hPushButton(QString s)
  {
    setText(s);
    installEventFilter(this);
    update();
  }
  bool eventFilter(QObject * obj,QEvent * ev)
  {
    return (obj != 0) && ev->type() == QEvent::Paint;
  }
};

// ---------------------------------------------------------------------
class Favs : public QDialog
{
  Q_OBJECT

public:
  Favs(Dirm *);

private slots:
  void cellActivated(int row);

private:
  Dirm *dirm;
  QTableWidget *wfav;

};

// ---------------------------------------------------------------------
class Dirm : public QDialog
{
  Q_OBJECT

public:
  Dirm(QString s);
  void dmsetdirs(QString,QString, bool);

  QComboBox *source;
  QComboBox *target;

private slots:
  void on_compare_clicked();
  void on_compareall_clicked();
  void on_copy_clicked();
  void on_ignore_clicked();
  void on_clip_clicked();
  void on_match_clicked();
  void on_open_clicked();
  void on_view_clicked();
  void on_exdiff_clicked();

  void on_source_currentIndexChanged(int);
  void on_target_currentIndexChanged(int);
  void on_type_currentIndexChanged(int);

  void on_subdir_stateChanged();

  void on_fileselAct_triggered();
  void on_filequitAct_triggered();
  void on_toswapAct_triggered();
  void on_tocopysrcAct_triggered();
  void on_tocopylaterAct_triggered();
  void on_tocopyallAct_triggered();

private:

  void compareallfiles();
  void comparefile();
  void comparexdiff();
  QStringList comparefile1(QString s);

  void copyall();
  void copyfile();
  void copyfiles(QStringList);
  void copylater();
  void copys2t(QString s);
  void copysource();

  QString dmgetname1();
  QString dmgetname2();
  QStringList dmgetnames();
  void dminfo(QString);
  int dmclipwrite();
  void dmread();
  void dmsaverecent();
  void dmshowfind();
  void dmwrite();
  void enablefound(bool);
  void favorites();
  void ignorefile();
  void init();
  void init_snp();
  void init_snp1(QString p);
  void init_std();
  void matches(bool);
  bool match_do();
  void match_fmt(bool);
  QString match_fmt1(QString s,QString d,int len);
  QStringList match_fmt2(QString s,int len);
  void match_refresh(int force);
  void reject();
  void savepos();

  QMenuBar *createmenu();
  QWidget *createpanel();
  QWidget *createview();
  QAction *makeact(QString id, QString text, QString shortcut);
  QPushButton *makebutton(QString id, QString text);

  bool Contents;
  bool Subdir;

  int Max;
  int matched;
  int TypeInx;
  int written;

  QAction *fileselAct;
  QAction *filequitAct;
  QAction *toswapAct;
  QAction *tocopysrcAct;
  QAction *tocopylaterAct;
  QAction *tocopyallAct;

  QCheckBox *timestamp;
  QCheckBox *subdir;

  QComboBox *type;

  QLabel *lsource;
  QLabel *ltarget;
  QLabel *ltype;

  QListWidget *found;
  QPushButton *match;
  hPushButton *filler;
  QPushButton *compareall;
  QPushButton *compare;
  QPushButton *exdiff;
  QPushButton *open;
  QPushButton *view;
  QPushButton *copy;
  QPushButton *ignore;
  QPushButton *clip;

  QString Project;
  QString SnapDir;
  QString Tab;
  QString Title;
  QString Source;
  QString Sourcex;
  QString Target;
  QString Targetx;

  QStringList Diff;
  QStringList Dirs;
  QStringList Found;
  QStringList NotInSource;
  QStringList NotInTarget;

};

#endif
