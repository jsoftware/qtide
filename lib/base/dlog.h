#ifndef DLOG_H
#define DLOG_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>

// ---------------------------------------------------------------------
class Slog : public QDialog
{
  Q_OBJECT

public:
  Slog();
  void addfilter(QString);
  void delfilter(bool);

public slots:
  void reject();
  void itemActivated();

private:
  bool hasuppercase(QString);
  void init();
  void keyPressEvent(QKeyEvent *e);
  void savepos();
  void showlist();

  QString Filter;
  QList<QStringList> LogList;
  QListWidget *s;
  QLabel *t;
};

// ---------------------------------------------------------------------
class SlogList : public QListWidget
{
  Q_OBJECT

public:
  SlogList(Slog *);
  Slog *pform;

protected:
  virtual void keyPressEvent(QKeyEvent *e);
};

// ---------------------------------------------------------------------
void dlog_add(QString);
QString dlog_get();
void dlog_init();
QString dlog_scroll(int);
void dlog_set(QString);
void dlog_write();

#endif
