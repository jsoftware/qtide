#ifndef SVR_H
#define SVR_H

#include <QObject>
#include <QString>
#include <QStringList>

class Jcon : public QObject

{
  Q_OBJECT

public:
  Jcon() {};
  void cmd(QString s);
  QString dora(QString s);
  int exec();
  void immex(QString s);
  int init(int argc, char* argv[]);
  void quit();

  QStringList Sentence;

public slots:
  void input();

private:

};

bool svr_init(int argc, char* argv[]);
extern Jcon *jcon;

#endif
