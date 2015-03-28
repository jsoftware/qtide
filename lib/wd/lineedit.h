#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <string>
#include <QLineEdit>

#include "child.h"

class LineEdit : public QLineEdit
{
  Q_OBJECT

public:
  LineEdit(Child *c, QWidget *parent = 0);

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  Child *pchild;

};

#endif
