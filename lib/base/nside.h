#ifndef NSIDE_H
#define NSIDE_H

#include <QFileIconProvider>
#include <QTabWidget>

class QFileDialog;
class QFileSystemModel;
class QFileIconProvider;
class QLineEdit;
class QListView;
class QListWidget;
class QListWidgetItem;
class QModelIndex;
class QToolBar;
class Note;

// ---------------------------------------------------------------------
class Nicon : public QFileIconProvider
{
public:
  Nicon() {};
  virtual QIcon icon(const QFileInfo &info) const;
};

// ---------------------------------------------------------------------
class Nside : public QTabWidget
{
  Q_OBJECT

public:
  Nside();
  QString Path;
  void refresh();
  int width;

private slots:
  void currentChanged(int index);
  void defs_activated(QListWidgetItem*);
  void file_activated(const QModelIndex &);
  void path_returnPressed();
  void source_activated(QListWidgetItem*);
  void on_defsTB_actionTriggered();
  void on_fileTB_actionTriggered(QAction *action);
  void on_sourceTB_actionTriggered();

private:
  QWidget *createdefs();
  QToolBar *createdefsTB();
  QWidget *createfiles();
  QToolBar *createfileTB();
  QWidget *createsource();
  QToolBar *createsourceTB();

  int defs_doselect(QString name,int dpos,QString txt);
  QAction *makeact(QString icon, QString text);

  void refresh1(int index);
  void defs_refresh();
  void file_refresh();
  void source_refresh();

  QAction *homeAct;
  QAction *refreshAct;
  QAction *refreshdefsAct;
  QAction *refreshsourceAct;
  QAction *setpathAct;

  QListWidget *dlw;
  QStringList dsl;
  int drow;
  int dpos;
  QString dext;
  QFileSystemModel *fm;
  QListView *flv;
  QLineEdit *path;
  QListWidget *slw;
  QStringList ssl;
};

#endif
