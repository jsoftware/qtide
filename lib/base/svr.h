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
  void cmddo(QString s);
  void cmddo(std::string s);
  QString cmdr(QString s);
  int exec();
  void immex(QString s);
  int init(int argc, char* argv[]);
  void quit();
  void set(QString s,QString t);

  QStringList Sentence;

public slots:
  void input();

};

bool svr_init(int argc, char* argv[]);
extern Jcon *jcon;
extern bool jeinput;

#endif
