
#include "child.h"
#include "layout.h"
#include "pane.h"
#include "wd.h"

// ---------------------------------------------------------------------
Layout::Layout(QChar type, int stretch, Pane *p)
{
  ppane=p;
  this->type=type;
  razed=false;
  rmax=cmax=1;
  r=c=0;
  rs=cs=1;
  alignment=0;
  this->stretch=stretch;
  spacing=0;
  if ('h'==type) bin=(QLayout *) new QHBoxLayout;
  else if ('v'==type) bin=(QLayout *) new QVBoxLayout;
  else if ('g'==type) bin=(QLayout *) new QGridLayout;
}

// ---------------------------------------------------------------------
void Layout::addWidget(QWidget *b)
{
  if (type!='g') {
    if (b) ((QBoxLayout *) bin)->addWidget(b);
  } else {
    if (razed) {
      if ((0<=rmax && r>=rmax) || c>=cmax) {
        error ("grid size exceeded");
        return;
      }
      if (b) ((QGridLayout *)bin)->addWidget(b, r, c, (Qt::Alignment) alignment);
      c=c+1;
      if (c==cmax) {
        c=0;
        r=r+1;
      }
    } else {
      if (b) {
        if (rs==1 && cs==1) ((QGridLayout *)bin)->addWidget(b, r, c, (Qt::Alignment) alignment);
        else ((QGridLayout *)bin)->addWidget(b, r, c, rs, cs, (Qt::Alignment) alignment);
      }
    }
  }
}

// ---------------------------------------------------------------------
void Layout::addLayout(Layout *b)
{
  if (type!='g')
    ((QBoxLayout *) bin)->addLayout(b->bin,b->stretch);
  else {
    if (razed) {
      ((QGridLayout *)bin)->addLayout(b->bin, r, c, (Qt::Alignment) alignment);
      c=c+1;
      if (c==cmax) {
        c=0;
        r=r+1;
      }
      if (r==rmax) {
        c=0;
        r=0;
      }
    } else if (rs==1 && cs==1) ((QGridLayout *)bin)->addLayout(b->bin, r, c, (Qt::Alignment) alignment);
    else ((QGridLayout *)bin)->addLayout(b->bin, r, c, rs, cs, (Qt::Alignment) alignment);
  }
}

// ---------------------------------------------------------------------
void Layout::addSpacing(int n)
{
  if (type!='g') ((QBoxLayout *) bin)->addSpacing(n);
}

// ---------------------------------------------------------------------
void Layout::addStretch(int n)
{
  if (type!='g') ((QBoxLayout *) bin)->addStretch(n);
}

// ---------------------------------------------------------------------
void Layout::removeWidget(QWidget *b)
{
  ((QBoxLayout *) bin)->removeWidget(b);
}
