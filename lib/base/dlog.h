#ifndef DLOG_H
#define DLOG_H

#include <QDialog>

class Slog : public QDialog
{
  Q_OBJECT

public:
  Slog();

public slots:
  void reject();
  void itemActivated();

private:
  void init();
  void savepos();
  void keyReleaseEvent(QKeyEvent *e);
};

void dlog_add(QString);
QString dlog_get();
void dlog_init();
QString dlog_scroll(int);
void dlog_set(QString);
void dlog_write();

#endif
