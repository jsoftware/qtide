#ifndef SVR_H
#define SVR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <list>

class Jcon : public QObject

{
  Q_OBJECT

public:
  Jcon() {};
  void cmd(std::string s);
  void cmddo(std::string s);
  void cmddop(std::string s);
  QString cmdr(std::string s);
//  int exec();
  void immex(std::string s);
  int init(int argc, char* argv[], uintptr_t stackinit);
  void set(QString s,std::string t);

  std::list <std::string> Sentence;

public slots:
  void cmdSentences();

private:
  void cmddos();
  void cmdSentence(std::string s);

};

bool svr_init(int argc, char* argv[], uintptr_t stackinit);
extern Jcon *jcon;
extern bool jecallback;
extern std::string wdQuery;

#endif
