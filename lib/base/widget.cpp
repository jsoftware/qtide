
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include "state.h"

// ---------------------------------------------------------------------
QPushButton *makebutton(QString id)
{
  QPushButton *p=new QPushButton(id);
  p->setObjectName(id.remove(' ').remove('&').toLower());
  return p;
}

// ---------------------------------------------------------------------
QCheckBox *makecheckbox(QString text, QString id="")
{
  QCheckBox *p;
  p=new QCheckBox(text);

  if (id.size())
    p->setObjectName(id);
  else
    p->setObjectName(text.remove(' ').remove('&').toLower());
  return p;
}

// ---------------------------------------------------------------------
QComboBox *makecombobox(QString id)
{
  QComboBox *p=new QComboBox();
  p->setMaxVisibleItems(config.MaxVisibleItems);
  p->setEditable(true);
  p->setCompleter(0);
  p->setObjectName(id.remove(' ').remove('&').toLower());
  return p;
}

// ---------------------------------------------------------------------
QFrame *makehline()
{
  QFrame* line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  return line;
}
