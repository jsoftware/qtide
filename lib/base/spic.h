#ifndef SPIC_H
#define SPIC_H

#include <QDialog>

class QComboBox;
class QPushButton;
class QListWidget;
class QPlainTextEdit;
class Eview;

// ---------------------------------------------------------------------
class Picm : public QDialog
{
  Q_OBJECT

public:

  Picm();

private slots:
  void on_times_currentRowChanged(int);
  void on_externaldiff_clicked();
  void on_restore_clicked();
  void on_view_clicked();
  void reject();


private:

  void closeit();
  QWidget *createpanel();
  QWidget *createview();
  void init();
  QStringList pic_files();
  void tcompare(int n);
  QString unstamp(QString);
  void keyReleaseEvent(QKeyEvent *e);

  QComboBox *sfile;
  QString File;
  QString Path;
  QStringList Stamps;
  QStringList Temps;
  QString Text;
  QStringList Texts;
  QString Title;
  QPushButton *bview;
  QPushButton *bxdiff;
  QPushButton *brestore;
  QListWidget *times;
  Eview *tview;
};

void pic(QString f,QString s);

#endif
