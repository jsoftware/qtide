#include <QTabBar>

#include "tabwidget.h"

// ---------------------------------------------------------------------
TabWidget::TabWidget(QWidget *parent) : QTabWidget (parent)
{
  Q_UNUSED(parent);
}

// ---------------------------------------------------------------------
void TabWidget::nobar (bool v)
{
  tabBar()->setVisible(!v);
}
