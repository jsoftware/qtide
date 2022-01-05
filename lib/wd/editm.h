#ifndef EDITM_H
#define EDITM_H

#include "child.h"
#include "../base/plaintextedit.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Editm : public Child
{
  Q_OBJECT

public:
  Editm(std::string n, std::string s, Form *f, Pane *p);
  void cmd(std::string p, std::string v);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:

private:
  void setselect(PlainTextEdit *w, int bgn, int end);

};

// ---------------------------------------------------------------------
class EditmPTE : public PlainTextEdit
{
  Q_OBJECT

public:
  EditmPTE(QWidget *parent=0);

  Child *pchild;

protected:
  void keyPressEvent(QKeyEvent *event);

};

#endif
