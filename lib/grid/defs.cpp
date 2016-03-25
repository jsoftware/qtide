
#include <QApplication>
#include <QStyle>
#include "../base/state.h"
#include "wgrid.h"

void WGrid::defs()
{
  font=app->font();

  sbx=app->style()->pixelMetric(QStyle::PM_ScrollBarExtent);

  CellText=QColor(0,0,0);
  CellBack=QColor(255,255,255);
  CellBackAlt = QColor(238,238,238);

  GridLines = QColor(199,196,193);
  GridBorder = QColor(0,0,0);
  GridLight = QColor(255,255,255);
  GridShadow = QColor(169,169,169);
  GridMark = QColor(47,79,79);

  HdrBack = QColor(244,240,236);

  Crws=Ccls=Hrws=Lcls=0;
  Srws=Scls=0;

  Mx=4;
  My=4;
  Mw=10;
  Mh=8;

  DataAlign << 2;

  DefAlign << 1;
  HdrAlign << 1;
  HdrAlignX << 1;
  LabAlign << 1;
  LabAlignX << 1;

  DefMerge << 1;
  HdrMerge << 1;
  LabMerge << 1;
}
