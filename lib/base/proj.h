#ifndef PROJ_H
#define PROJ_H

#include <QObject>
#include <QDir>

class Project : public QObject
{

  Q_OBJECT

public:
  Project() {};

  QString Folder;
  bool Git;
  QString Id;
  QString LastId;
  QString Path;

  void close();
  bool closeOK();
  QString id2qproj(QString);
  void init();
  void open(QString id);
  void save(QStringList);
  QStringList source();

  QString fullname(QString s);
  QString projfile();
  QString projectname(QString s);

private:
  QString projectname1(QString s,QStringList k,QStringList v);

};

extern Project project;

#endif
