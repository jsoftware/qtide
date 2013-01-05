#ifndef PNEW_H
#define PNEW_H

#include <QDialog>

class QCheckBox;
class QDir;
class QLineEdit;
class QPushButton;

class Pnew : public QDialog
{
  Q_OBJECT

public:
  Pnew();
  bool run();

private slots:
  void on_browse_clicked();
  void on_create_clicked();

private:
  QWidget *createfolderpanel();
  QWidget *createscriptspanel();
  QWidget *createotherpanel();
  QWidget *createbuttonpanel();

  QDir *Dir;
  QString Path;
  QString Title;

  QCheckBox *cbuild;
  QCheckBox *cinit;
  QCheckBox *crun;

  QLineEdit *folder;
  QLineEdit *other;
};


#endif
