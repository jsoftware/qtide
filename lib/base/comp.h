#ifndef COMP_H
#define COMP_H

#include <QObject>
#include <QStringList>
#include <QVector>

class Compare : public QObject
{
  Q_OBJECT

public:
  Compare() {};
  QString comp(QStringList s,QStringList t);

private:
  QStringList XY;
  QVector<int> AX,AY,NX,NY,SX,SY,X,Y;
  bool compend();
  void complcs();
  QVector<int> seqlcs(QVector<int>,QVector<int>);
};

QString compare(QStringList,QStringList);
QString fcompare(QString,QString);

#endif
