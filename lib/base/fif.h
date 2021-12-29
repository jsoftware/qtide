#ifndef FIF_H
#define FIF_H

#include <QDialog>

#if defined(QT60)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

class QCheckBox;
class QComboBox;
class QLabel;
class QListWidget;
class QListWidgetItem;

class Fif : public QDialog
{

  Q_OBJECT

public:
  Fif(QString s, bool b);
  void initshow(QString s, bool b);

private slots:
  void on_assigned_clicked();
  void on_fileonly_clicked();
  void on_find_clicked();
  void on_found_itemActivated(QListWidgetItem * item);
  void on_matchcase_clicked();
  void on_nameonly_clicked();
  void on_regex_clicked();
  void on_subdir_clicked();

private:

  void findit();
  void finfo(QString s);
  void init(QString s, bool b);
  QList<int> lineindex(QList<int> hit,const QString txt);
  QList<int> lineends(const QString txt);
  void read();
  void refresh();
  void reject();
  QList<int> removedups(QList<int> n);
  void research();
  void search();
  QStringList searchdo();
  QStringList searchfile(QString file);
  QList<int> searchfilex(QString txt);
  QStringList searchformat(QString f,QList<int>hit,QString txt);
  void setenable();
  void setpathlist(QString p);
  void setsearchlist(QString s);
  void setsearchmaxlength();
  void settypelist(QString t);
  void write();
  void writebuttons();

  QCheckBox *assigned;
  QCheckBox *fileonly;
  QCheckBox *matchcase;
  QCheckBox *nameonly;
  QCheckBox *regex;
  QCheckBox *subdir;

  QComboBox *searchfor;
  QComboBox *infolder;
  QComboBox *filetypes;

  QListWidget *found;
  QPushButton *find;

  QList<int>Pos;

  bool Assign;
  bool Fileonly;
  QString Filetypes;
  bool Matchcase;
  int Max;
  bool Name;
  QString Path;
  QStringList PathList;
  bool Regex;
  QString Search;
  QStringList SearchList;
  bool Subdir;
  QString Title;
  QString Type;
  QStringList Types;

  bool ifRegex;
  bool ifResults;
  QString what;
#if defined(QT60)
  QRegularExpression pat;
#else
  QRegExp pat;
#endif

};

#endif
