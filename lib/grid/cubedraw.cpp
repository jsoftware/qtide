
#include "qgrid.h"
#include "wgrid.h"

// using namespace std;

// ---------------------------------------------------------------------
void QGrid::drawbuttons(QBoxLayout *layout,QList<int> axis)
{
  CubeButton *b;
  foreach(int i,axis) {
    b=new CubeButton(AxisNames[i]);
    layout->addWidget(b);
  }
  layout->addStretch(1);
}

// ---------------------------------------------------------------------
bool QGrid::drawcube()
{
  if (GGridInit) return true;
  if (!sa->g->getgrid()) return false;

  bool drawn=mslice != 0;
  if (drawn) {
    hbmain->removeWidget(mcols);
    vbmain->removeWidget(mslice);
    vbmain->removeWidget(mrows);
    mcols->deleteLater();
    mslice->deleteLater();
    mrows->deleteLater();
  }

  mslice=new CubeMargin(QBoxLayout::LeftToRight,this);
  mcols=new CubeMargin(QBoxLayout::TopToBottom,this);
  mrows=new CubeMargin(QBoxLayout::LeftToRight,this);

  drawcubeslice();
  drawcuberows();
  drawcubecols();

  if (!drawn) {
    hbmain->addWidget(mcols);
    vbmain->addWidget(mslice);
    vbmain->addLayout(hbmain);
    vbmain->addWidget(mrows);
  } else {
    hbmain->insertWidget(1,mcols);
    vbmain->insertWidget(0,mslice);
    vbmain->insertWidget(-1,mrows);
  }
  return GGridInit=true;
}

// ---------------------------------------------------------------------
void QGrid::drawcubecols()
{
  drawbuttons(mcols->layout,AxisCols);
}

// ---------------------------------------------------------------------
void QGrid::drawcuberows()
{
  drawbuttons(mrows->layout,AxisRows);
}

// ---------------------------------------------------------------------
void QGrid::drawcubeslice()
{
  noevents(1);
  int i,j;
  int n=AxisSlice.size();
  QStringList s;
  CubeCombo *cb;
  if (n==0) {
    QSpacerItem *w=new QSpacerItem(0,12);
    mslice->layout->addItem(w);
  } else {
    for (i=0; i<n; i++) {
      if (i>0)
        mslice->layout->addSpacing(6);
      j=AxisSlice[i];
      cb=new CubeCombo(AxisNames[j],this);
      cb->combobox->addItems(AxisLabels[j]);
      cb->combobox->setCurrentIndex(AxisIndex[j]);
      mslice->layout->addWidget(cb);
    }
    mslice->layout->addStretch(1);
  }
  noevents(0);
}

