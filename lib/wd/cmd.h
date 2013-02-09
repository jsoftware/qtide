#ifndef CMD_H
#define CMD_H

#include <string>
#include <QStringList>

class Cmd
{
public:
  void end();
  string getid();
  string getline();
  string getparms();
  void init(char *);
  bool more();

  string getcmdstr();
  string remws(string s);

  QStringList bsplits();
  QStringList qsplits();

private:
  bool contains(string s,char c);
  void skippast(char c);
  void skips(string s);
  void skiptows();

  size_t bgn;
  size_t len;
  size_t pos;
  string str;
};

QStringList bsplit(string s);
QStringList qsplit(string s);

#endif
