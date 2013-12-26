#ifndef RECENT_H
#define RECENT_H

#include <QObject>
#include <QStringList>

class QFile;

class Recent : public QObject
{

  Q_OBJECT

public:
  Recent() {};

  QStringList DirMatch;
  QStringList recentFif;
  QStringList Files;

  QString ProjectFile;
  QString RecentFile;

  bool ProjectOpen;
  QList<QStringList> Projects;

  void filesadd(QString s);
  void init();

  QStringList projectget(QString id);
  void projectset(QStringList s);

  void save_project();
  void save_recent();

};

extern Recent recent;

#endif
