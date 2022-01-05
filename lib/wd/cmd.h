#ifndef CMD_H
#define CMD_H

#include <string>
#include <QStringList>
#include <vector>

// using namespace std;

class Cmd
{
public:
  void end();
  bool delimLF(std::string);
  std::string getid();
  std::string getline();
  std::string getparms(bool s=false);
  bool ifstarred(std::string);
  void init(char *,int);
  bool more();

  std::string remws(std::string s);
  void markpos();
  void rewindpos();

  QStringList bsplits();
  QStringList qsplits();
  std::vector<std::string> ssplits();

private:
  void skippast(char c);
  void skips(std::string s);
  void skiptows();
  QStringList qsplitstd();
  std::vector<std::string> ssplitstd();
  QStringList qsplitby(char c);
  std::vector<std::string> ssplitby(char c);

  size_t bgn;
  size_t len;
  size_t pos;
  size_t pos0;
  std::string str;
};

QStringList bsplit(std::string s);
QStringList qsplit(std::string s,bool p=false);
std::vector<std::string> ssplit(std::string s);

#endif
