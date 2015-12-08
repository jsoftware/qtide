#ifndef QMLJE_H
#define QMLJE_H

#include <QObject>
#include <QString>

// Simple QML object for J engine interface

class QmlJE : public QObject
{
  Q_OBJECT

public:
  QmlJE(QObject* parent = 0);
  ~QmlJE();

  Q_INVOKABLE QString verb(QString v,QString y,bool ignoreResult=false);
  Q_INVOKABLE int dor(QString s);
  Q_INVOKABLE QString dors(QString s);
  Q_INVOKABLE QString getvar(QString n);
  Q_INVOKABLE void setvar(QString n,QString s);

};

#endif
