#ifndef TABLE_H
#define TABLE_H

#include "child.h"

class Form;

// ---------------------------------------------------------------------
class Table : public Child
{
  Q_OBJECT

public:
  Table(string s, string p, Form *f);
  void set(string p);
  void setcolnames(string v);
  void setp(string p, string v);
  string state();

private slots:

private:
  QList<int> coltypes;

};

#endif
