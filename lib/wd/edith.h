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
  Edith(std::string n, std::string s, Form *f, Pane *p);
  void cmd(std::string p, std::string v);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

#ifndef QT_NO_PRINTER
public slots:
  void printPreview(QPrinter * printer);
#endif

private slots:

private:
  void setselect(QTextEdit *w, int bgn, int end);

};

#endif
