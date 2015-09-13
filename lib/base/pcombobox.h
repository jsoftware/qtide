#ifndef PCOMBOBOX_H
#define PCOMBOBOX_H

#include <QComboBox>
#include <QKeyEvent>

// ---------------------------------------------------------------------
class PComboBox: public QComboBox
{
  Q_OBJECT

public:
  PComboBox(QWidget *parent = 0);
  bool acceptKeyBack;

};

#endif
