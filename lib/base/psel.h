#ifndef PSEL_H
#define PSEL_H

#include <QDialog>
#include <QKeyEvent>
class QListWidget;

class Psel : public QDialog
{

  Q_OBJECT

public:
  Psel();

private slots:
  void fsel_changed(int row);
  void psel_changed(int row);
  void rsel_changed(int row);
  void itemActivated();

private:
  void getids(int);
  QStringList getrecent();
  void getrecentx();

  void init();
  void initsel();
  void initwrite();
  void prefresh();
  void reject();

  int Ftx,Rtx;
  QListWidget *panel(QString s);
  QListWidget *f,*p,*r;
  QStringList Folders,Paths,Projects;
  QStringList Recent,RecentFolders,RecentProjects;
  QList<QStringList> Ids;
  QList<int> Idx;
};

#endif
