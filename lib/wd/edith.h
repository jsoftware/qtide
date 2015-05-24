#ifndef EDITH_H
#define EDITH_H

#include "child.h"

class QPrinter;
class QTextEdit;
class Form;
class Pane;

// ---------------------------------------------------------------------
class Edith : public Child
{
  Q_OBJECT

public:
  Edith(string n, string s, Form *f, Pane *p);
  void cmd(string p, string v);
  string get(string p,string v);
  void set(string p,string v);
  string state();

#ifndef QT_NO_PRINTER
public slots:
  void printPreview(QPrinter * printer);
#endif

private slots:

private:
  void setselect(QTextEdit *w, int bgn, int end);

};

#endif
