#ifndef FIW_H
#define FIW_H

#include <QDialog>

class QCheckBox;
class QComboBox;
class QLabel;
class QListWidget;

class Bedit;

class Fiw : public QDialog
{
  Q_OBJECT

  static QStringList ReplaceList;
  static QStringList SearchList;

public:
  Fiw(int, QString);
  void initshow(QString);

private slots:
  void on_assigned_clicked();
  void on_findback_clicked();
  void on_findnext_clicked();
  void on_findtop_clicked();
  void on_nameonly_clicked();
  void on_replace_clicked();
  void on_replaceforward_clicked();
  void on_undolast_clicked();

private:
  bool eventFilter(QObject *, QEvent *);
  void finfo(QString s);
  void keyPressEvent(QKeyEvent *);
  void open_replace();
  void read();
  void readtext();
  void readwin();
  void refresh();
  void reject();
  void setmodified();

  void search(int dir);
  int search1(int dir);
  int searchback(QString s, QString txt);
  int searchforward(QString s, QString txt);
  void search_replace(int dir);
  QStringList setlist(QString s, QStringList t);
  void setsearchdirection(int d);
  void setsearchlist(QString s);
  void setreplacelist(QString s);

  void showhit();
  void showit();
  void write();
  void writebuttons();

  QCheckBox *matchcase;
  QCheckBox *assigned;
  QCheckBox *nameonly;
  QComboBox *searchfor;
  QComboBox *replaceby;
  QLabel *lsearchfor;
  QLabel *lreplaceby;
  QListWidget *flist;
  QPushButton *findback;
  QPushButton *findnext;
  QPushButton *findtop;
  QPushButton *replace;
  QPushButton *replaceforward;
  QPushButton *undolast;

  QList<int>Pos;

  bool Assign;
  bool ifReplace;
  int Inc;
  int LastPos;
  QString LastText;
  bool Matchcase;
  int Max;
  bool Name;
  int Parent;
  QString Search;
  QString Replace;
  QString Text;
  int TextPos;
  QWidget *Wid;
  Bedit *Win;
};

#endif
