#ifndef TABLE_H
#define TABLE_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Table : public Child
{
  Q_OBJECT

public:
  Table(string n, string s, Form *f, Pane *p);
  void set(string p);
  void setcolnames(string v);
  void setp(string p, string v);
  string state();

private slots:

private:
  QList<int> coltypes;

};

#endif
